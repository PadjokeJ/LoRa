# LoRa Ping Protocol

This project is a lightweight demonstration program for testing **LoRa point-to-point communication** using the **Dragino LoRa Shield v1.3** (based on SX1276) on Arduino.

It is designed to:
- Periodically send `PING` messages
- Respond to `PING`s with `PONG`
- Optionally resolve human-readable names using `RAR`/`RARA` (Reverse Address Resolution)

> ⚠️ **Before flashing** the sketch (`ping.ino`), assign a unique `myId` in `config.h`.

Additional configuration (frequency, power, modem settings, logging) is also available in `config.h`.

---

### 🧩 Platform selection

Edit `config.h` to choose the target board:

```c
//#define DRAGINO_SHIELD
#define DRAGINO_DEV
//#define M5STACK


## 🛠 Requirements

- Arduino Uno (or compatible board)
- Dragino LoRa Shield v1.3 (SX1276-based)
- [RadioHead library](http://www.airspayce.com/mikem/arduino/RadioHead/) (RH_RF95)
- USB cable for flashing and optional serial debugging

---

## 📡 Protocol Overview

### 🔢 Addresses

Each node uses a **2-byte address**, for example: `10.42`.

Special address:
- `0.0` is reserved for **broadcasts** (everyone receives it).

---

### 📨 Message Types

| Code | Type   | Description                                                   |
|------|--------|---------------------------------------------------------------|
| `1`  | `PING` | Sent to query presence or measure latency                     |
| `2`  | `PONG` | Reply to a `PING`, echoes the original timestamp              |
| `3`  | `RAR`  | Reverse Address Resolution request (asks for a readable name) |
| `4`  | `RARA` | Reply to a `RAR`, containing the readable name                |

---

### 🧾 Message Format

All **multi-byte fields** are in **big-endian** format (most significant byte first).

| Bytes     | Field                 | Description                                       |
|-----------|-----------------------|---------------------------------------------------|
| 0         | Message type          | `PING`, `PONG`, `RAR`, or `RARA`                 |
| 1–2       | Sender address        | 2 bytes (e.g., `10.42`)                          |
| 3–4       | Destination address   | 2 bytes (or `0.0` for broadcast)                 |
| 5–6       | Packet ID             | Randomly generated ID for matching replies       |
| 7–10      | Timestamp             | `millis()` at send time                          |
| 11        | Payload length        | In bytes (0 for no payload)                      |
| 12+       | Payload (optional)    | Used only in `RARA` to send the node name        |

---

## ⚙️ Configurable Parameters

In `config.h`, you can adjust:

- `myId` → your node's unique 2-byte address (e.g., `{10,42}`)
- `myName` → readable name for the node (sent in `RARA`)
- `TX_POWER` → transmit power in dBm (e.g., `20`)
- `FREQUENCY` → operating frequency (e.g., `868 MHz` for Europe)
- `MODEM_CONFIG` → LoRa modulation settings for range vs speed
- `DEBUG_LEVEL` → controls serial logging verbosity

---

## 🔄 Packet Flow Example

```text
                +---------+                             +---------+
                | Node A  |                             | Node B  |
                +---------+                             +---------+
                     |                                       |
                     |-------- PING (to B or broadcast) ---->|
                     |                                       |
                     |<-------- PONG (to A) -----------------|
                     |                                       |
                     |-------- RAR (Who are you?) ---------->|
                     |                                       |
                     |<-------- RARA ("Node B") -------------|
                     |                                       |
```

---

### 🔁 Example Transmissions

#### ✅ PING broadcast:

```text
Type: PING
From: 10.42
To: 0.0 (broadcast)
ID: 184.23
Timestamp: 1234567 ms
```

→ All nodes receiving this will respond with `PONG`.

---

#### ✅ PONG reply:

```text
Type: PONG
From: 10.99
To: 10.42
ID: 184.23
Timestamp: 1234567 ms (copied from original ping)
```

→ Latency is calculated as `millis() - timestamp`.

---

#### ✅ RAR request:

```text
Type: RAR
From: 10.42
To: 10.99
ID: 201.85
Timestamp: 2234567 ms
```

→ Queries Node 10.99 for its name.

---

#### ✅ RARA reply:

```text
Type: RARA
From: 10.99
To: 10.42
ID: 201.85
Timestamp: 2234567 ms (same as RAR)
Payload: "NODE_B"
```

→ The readable name of Node B is returned in the payload.

---

## 📍 Notes

- Timestamps are local (`millis()` at the sender).
- Replies reuse original `packetId` and `timestamp` from the request.
- Payload is optional and used only in `RARA`.
- The system is designed for simplicity and experimentation.

---

© Philippe Rochat – *eduvaud.ch*  
Written with help from ChatGPT  
Designed for experimentation and learning with low-level LoRa communications.
