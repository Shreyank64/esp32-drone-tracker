
# ESP32 Drone-Deployed Surveillance & GPS Tracker

This project implements a **compact surveillance and tracking device** designed for **law enforcement drones**. Built around the ESP32-CAM, it provides live video streaming and real-time GPS tracking when deployed at a target location.

---

## 🚁 Use Case: Drone-Deployed Tracker for Law Enforcement

The device is carried by a drone equipped with FPV. Once a visual target is confirmed, the drone **drops the device**. Upon release, a **jumper pin disconnects**, activating a **relay-controlled payload** (e.g., an electromagnet or beacon). Meanwhile, the system begins GPS logging and live camera streaming for situational awareness.

---

## 🔧 Features

- 📍 **GPS Tracking** using an M10/NEO-6M GPS module
- 🎥 **Live Camera Feed** using ESP32-CAM module
- ⚡ **Automatic Activation** of relay/payload upon drone drop (via jumper pin detection)
- 🌐 **Built-in Web Server** to serve:
  - GPS coordinates, date, and time
  - One-click Google Maps tracking link
  - Live camera feed (optional)
- 💡 **Compact, drone-deployable footprint**

---

## 🧰 Hardware Requirements

- [x] ESP32-CAM (AI Thinker or ESP-EYE with PSRAM)
- [x] M10 GPS Module (or NEO-6M)
- [x] Electromagnet or Relay Module
- [x] Jumper wire for activation trigger
- [x] Power source (5V LiPo or step-down module)

---

## 📎 Pin Connections

| Component         | ESP32 Pin |
|------------------|-----------|
| GPS RX (from GPS)| GPIO 2     |
| GPS TX (to GPS)  | GPIO 14    |
| Jumper trigger   | GPIO 13    |
| Relay module     | GPIO 4     |

---

## 🔌 Setup Instructions

1. Connect the hardware as per the table above.
2. Flash the code in `esp32_drone_tracker.ino` using Arduino IDE.
3. Power on the ESP32 device.
4. Connect to the same Wi-Fi network as specified in the code (`ssid`, `password`).
5. Open the IP address shown in the Serial Monitor to access GPS logs.

---

## 🌍 Web Interface

The built-in web server provides:
- ✅ Current Latitude & Longitude
- ✅ Date and Time (UTC+5 adjusted)
- ✅ Google Maps link to live location
- ✅ Camera stream (optional, depending on config)

---

## 🔐 Security Suggestions

- Restrict Wi-Fi access to secure network only.
- Add password protection for the camera or GPS page.
- Use HTTPS proxy tunneling for external access.

---

## 📄 License

This project is provided for educational and experimental law enforcement use only. Unauthorized or commercial use must comply with local surveillance laws.

---

## 📷 Screenshots / Demos

Coming soon!

---

## 📬 Future Improvements

- Add SD card for local logging
- Enable SIM/LTE or LoRa-based long-range comms
- Remote deactivation of relay/payload
