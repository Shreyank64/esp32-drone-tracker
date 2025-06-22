
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "Pavillion";
const char* password = "venky2004";

void startCameraServer();
void setupLedFlash(int pin);

TinyGPSPlus gps;
SoftwareSerial SerialGPS(2, 14); // GPS RX, TX

const int jumper_pin = 13; // Jumper for deployment detection
const int relay_pin = 4;   // Relay control pin

float Latitude, Longitude;
int year, month, date, hour, minute, second;
String DateString, TimeString, LatitudeString, LongitudeString;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (psramFound()) {
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  s->set_framesize(s, FRAMESIZE_QVGA);

  setupLedFlash(LED_GPIO_NUM);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  pinMode(jumper_pin, INPUT_PULLUP);
  pinMode(relay_pin, OUTPUT);
  SerialGPS.begin(9600);

  server.begin();
}

void loop() {
  delay(10000);

  if (digitalRead(jumper_pin) != HIGH) {
    digitalWrite(relay_pin, LOW); // Relay ON
  } else {
    digitalWrite(relay_pin, HIGH); // Relay OFF
  }

  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
      }
      if (gps.date.isValid()) {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10) DateString = '0';
        DateString += String(date) + " / ";
        if (month < 10) DateString += '0';
        DateString += String(month) + " / ";
        DateString += String(year);
      }
      if (gps.time.isValid()) {
        TimeString = "";
        hour = gps.time.hour() + 5;
        minute = gps.time.minute();
        second = gps.time.second();
        if (hour < 10) TimeString = '0';
        TimeString += String(hour) + " : ";
        if (minute < 10) TimeString += '0';
        TimeString += String(minute) + " : ";
        if (second < 10) TimeString += '0';
        TimeString += String(second);
      }
    }

  WiFiClient client = server.available();
  if (!client) return;

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>GPS Tracker</title><style>";
  s += "table, th, td {border: 1px solid blue;} </style></head><body><h1 ALIGN=CENTER style='font-size:300%;'>Drone GPS Tracking</h1>";
  s += "<p ALIGN=CENTER style='font-size:150%;'><b>Location Details</b></p><table ALIGN=CENTER style='width:50%'>";
  s += "<tr><th>Latitude</th><td ALIGN=CENTER>" + LatitudeString + "</td></tr>";
  s += "<tr><th>Longitude</th><td ALIGN=CENTER>" + LongitudeString + "</td></tr>";
  s += "<tr><th>Date</th><td ALIGN=CENTER>" + DateString + "</td></tr>";
  s += "<tr><th>Time</th><td ALIGN=CENTER>" + TimeString + "</td></tr></table>";

  if (gps.location.isValid()) {
    s += "<p align=center><a style='color:RED;font-size:125%;' href='http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += LatitudeString + "+" + LongitudeString + "' target='_top'>Click here</a> to view in Google Maps.</p>";
  }
  s += "</body></html>";

  client.print(s);
  delay(100);
}

void setupLedFlash(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void startCameraServer() {
  // Add server handlers here (or leave empty if using default stream URL)
}
