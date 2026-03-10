# ♻️ Waste Battery Recovery & Quotation System

> ⚠️ **This project is currently under active development** and is submitted as a graduation design project for academic thesis purposes, called Waste Battery Recycling Quotation System. Features and architecture may change as development progresses.

---

## 📌 Overview

This system is developed as a **graduation design project** for academic thesis submission, called Waste Battery Recycling Quotation System. It is a smart quotation platform for waste battery recycling enterprises, built with **Qt / C++** using a **Client-Server (C/S) architecture**. The system supports real-time metal price synchronization, battery material composition modeling, and automated quotation calculation.

The platform covers the full business workflow: **quotation evaluation → order submission → server-side review → status synchronization**.

The project consists of two sub-applications:

| Sub-project                       | Description                                                                                   |
| --------------------------------- | --------------------------------------------------------------------------------------------- |
| `WasteMetalRecoverySystem`        | **Client** — used by recycling staff to evaluate quotations and submit orders                 |
| `WasteMetalRecoverySystem_server` | **Server** — used by administrators to process orders, manage parameters, and monitor clients |

---

## ✨ Features

### Client

- **Live Metal Market Prices**: Displays real-time prices for lithium carbonate, cobalt sulfate, nickel sulfate, manganese sulfate, and copper (CNY/ton)
- **Smart Quotation Engine**: Automatically calculates a recycling quotation based on battery type, weight, SOH (State of Health), and energy density
- **Cascade / Metal Recovery Classification**: Automatically determines whether a battery qualifies for cascade reuse (SOH ≥ 80% and energy density > 0) or metal-only recovery
- **Residual Capacity Estimation**: Automatically estimates and displays the battery's remaining usable electricity (kWh)
- **Order Submission**: Supports two selling methods — _in-person pickup_ and _online mail-in_
- **Order History**: Browse all local historical orders with multiple sort options and individual order deletion
- **Server Connection Management**: Auto-reconnect, heartbeat detection, and real-time connection status display

### Server

- **Order Management**: Separate lists for _pending_ and _completed_ orders; supports accept/reject with results pushed to the client in real time
- **Metal Price Management**: Manually update metal market prices and broadcast them to all connected clients
- **Battery Type Management**: Add, delete, and edit battery material data (cathode material ratio, metal content, recovery rates, etc.)
- **Quotation Parameter Management**: Configure per-battery-type pricing parameters including cascade unit prices, processing cost per kg, profit margin, and metal conversion rates
- **Online Client Monitoring**: View connected clients' IP addresses, login timestamps, and UUIDs; supports heartbeat detection to remove zombie connections
- **Runtime Logging**: Automatically generates daily log files locally

---

## 🏗️ Architecture

```
┌──────────────────────────────────────┐
│              Client                  │
│  Quotation → Order Submission        │
│  → History Lookup                    │
└──────────────────┬───────────────────┘
                   │ TCP Socket
                   │ QDataStream Protocol
┌──────────────────┴───────────────────┐
│              Server                  │
│  Order Review → Parameter Mgmt       │
│  → Client Broadcast                  │
└──────────────────────────────────────┘
```

### Communication Protocol (Message Type Enum)

| Value | Name                 | Description                                               |
| ----- | -------------------- | --------------------------------------------------------- |
| 0     | `HANDSHAKE`          | Connection handshake; server delivers initialization data |
| 1     | `NEW_TRANSACTION`    | Client submits a new order                                |
| 2     | `TRANSACTION_STATUS` | Server pushes order processing result                     |
| 3     | `METAL_PRICE`        | Server broadcasts metal price update                      |
| 4     | `QUOTATION_DATA`     | Server pushes new or modified quotation parameters        |
| 5     | `HEART_BEAT`         | Heartbeat detection                                       |
| 6     | `BATTERY_REMOVED`    | Server notifies deletion of a battery type                |
| 7     | `BATTERY_CHANGED`    | Server notifies renaming of a battery type                |

---

## 💡 Quotation Model

The quotation algorithm weighs both **cascade reuse value** and **metal recovery value**, taking the higher of the two and applying a profit deduction.

**Metal Recovery Quotation:**

```
Metal Content    = Battery Weight × Cathode Ratio × Compound Ratio
                   × Metal Ratio × Recovery Rate
Metal Quotation  = Metal Content × Conversion Factor × Market Price
                   × Lab Conversion Rate
Final Price      = Σ(Metal Quotations) + Cu Quotation
                   − Processing Cost/kg × Weight
```

**Cascade Reuse Quotation (SOH ≥ 80% and Energy Density > 0):**

```
Cascade Price = Weight × Energy Density × SOH
                × Unit Price (tiered: SOH ≥ 90% or 80%~90%)
```

**Output Quotation = max(Metal Recovery Price, Cascade Price) × (1 − Profit Margin)**

**Molar Mass Conversion Factors:**

- Li → Li₂CO₃ (Lithium Carbonate): 73.89 / (6.94 × 2)
- Ni → NiSO₄·6H₂O (Nickel Sulfate): 262.87 / 58.69
- Co → CoSO₄·7H₂O (Cobalt Sulfate): 281.10 / 58.93
- Mn → MnSO₄·H₂O (Manganese Sulfate): 169.02 / 54.94

---

## 📁 Project Structure

```
WasteMetalRecoverySystem_C&S.pro            # Root project file (subdirs)
├── WasteMetalRecoverySystem/               # Client sub-project
│   ├── main.cpp
│   ├── mainwindow.cpp / .h / .ui           # Main window
│   ├── quotation.cpp / .h                  # Quotation model
│   ├── transaction.cpp / .h                # Transaction data structure
│   ├── batteryMaterialConcentration.*      # Battery material composition
│   ├── recoveryCost.*                      # Recovery cost parameters
│   ├── metalPrice.*                        # Metal price data
│   ├── interactableFrame.*                 # Custom interactive Frame widget
│   ├── transactionHistoryDialog.*          # Order history dialog
│   ├── settingDialog.*                     # Settings dialog
│   ├── clientSetting.*                     # Client configuration
│   ├── MainWindow.qss                      # Main window stylesheet
│   └── rec.qrc                             # Qt resource file
│
└── WasteMetalRecoverySystem_server/        # Server sub-project
    ├── main.cpp
    ├── mainwindow.cpp / .h / .ui           # Main window
    ├── quotation.cpp / .h                  # Quotation model (with default data init)
    ├── transaction.cpp / .h                # Transaction data structure
    ├── batteryMaterialConcentration.*      # Battery material composition
    ├── recoveryCost.*                      # Recovery cost parameters
    ├── metalPrice.*                        # Metal price data
    ├── clientInfo.*                        # Client session information
    ├── interactableFrame.*                 # Custom interactive Frame widget
    ├── dealDialog.*                        # Order review dialog
    ├── batteryDialog.*                     # Battery management dialog
    ├── quotationDialog.*                   # Quotation parameter dialog
    ├── metalPriceDialog.*                  # Metal price dialog
    └── onlineClientsDialog.*               # Online clients dialog
```

---

## 🗄️ Local Data Storage

All data is persisted locally in binary format using `QDataStream`:

| Path                                     | Content                                                         |
| ---------------------------------------- | --------------------------------------------------------------- |
| `bin/quotation_model/metalPrice_CNY.dat` | Metal market prices                                             |
| `bin/quotation_model/battery/*.dat`      | Battery material data (filename is Base64-encoded battery name) |
| `bin/quotation_model/recoveryCost/*.dat` | Per-battery quotation parameters                                |
| `bin/transactions/*.dat`                 | Historical transaction orders                                   |
| `bin/setting/setting.dat`                | Client configuration (client only)                              |
| `bin/logs/*.log`                         | Runtime logs (server only)                                      |
| `config.ini`                             | Device UUID (client; auto-generated on first run)               |

---

## ⚙️ Requirements

| Dependency   | Version                                           |
| ------------ | ------------------------------------------------- |
| Qt           | 5.x (tested) / 6.x requires adaptation            |
| C++ Standard | C++11 or later                                    |
| Qt Modules   | `core` `gui` `widgets` `network`                  |
| Compiler     | MSVC / MinGW (Windows); GCC / Clang (Linux/macOS) |

---

## 🚀 Getting Started

### Build

```bash
# Clone the repository
git clone <repo_url>
cd WasteMetalRecoverySystem_C&S

# Open WasteMetalRecoverySystem_C&S.pro in Qt Creator
# Or build via command line:
qmake WasteMetalRecoverySystem_C&S.pro
make
```

### Running

1. **Start the server first** — `WasteMetalRecoverySystem_server`, which listens on `0.0.0.0:8888` by default
2. **Then start the client** — `WasteMetalRecoverySystem`, which connects to `127.0.0.1:8888` by default
3. To change the server address or port, open the **⚙️ Settings** panel in the client

### Server Default Seed Data

On first launch, the server automatically initializes the following defaults:

- **Battery Type**: LCO (Lithium Cobalt Oxide), with full material composition and recovery parameters
- **Metal Prices**: Li₂CO₃ ¥136,038/t · CoSO₄ ¥96,664/t · NiSO₄ ¥31,645/t · MnSO₄ ¥6,940/t · Cu ¥99,605/t

---

## 🖥️ UI Overview

### Client Main Window

- **Top**: Metal market price cards (Li / Co / Ni / Mn / Cu)
- **Middle**: Battery input area (type selector, weight, energy density, SOH slider)
- **Bottom**: Final quotation display, usage classification, residual capacity, sell buttons
- **Right panel**: Personal data (order records, settings, server connection status)

### Server Main Window

- **Left**: Pending / Completed order lists (tab-switched)
- **Right**: Server message log + quick-access action buttons (metal prices, battery management, online clients, quotation parameters)

---

## 🔧 Client Configuration Options

| Option             | Default            | Description                                  |
| ------------------ | ------------------ | -------------------------------------------- |
| Server IP          | `127.0.0.1`        | IP address of the server                     |
| Port               | `8888`             | Server port number                           |
| Window Size        | `1320 × 820`       | Initial window dimensions on startup         |
| Order Save Path    | `bin/transactions` | Local directory for order files              |
| Connection Timeout | `5s`               | Abort connection attempt after this duration |

---

## 📝 Notes

- Both client and server use `QDataStream` version `Qt_5_14`; data compatibility must be checked when upgrading Qt versions
- Order IDs are composed of a timestamp plus a random 4-digit suffix, ensuring uniqueness on a single device
- The device UUID is stored in `config.ini` and auto-generated on first run; it is used by the server to identify each client session
- The heartbeat timeout threshold is **15 seconds**; the server administrator can manually trigger heartbeat detection from the Online Clients dialog

---

## 📄 License

This project is developed for academic purposes as a graduation design thesis. It is not licensed for commercial use. Please contact the author for any other usage.
