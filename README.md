# Coffee Shop Sales System

A desktop application built with **Qt6 (C++)** for managing coffee shop sales, inventory, and user authentication.

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Prerequisites](#prerequisites)
- [Installation & Build](#installation--build)
- [File Structure](#file-structure)
- [Data Storage](#data-storage)
- [Usage Guide](#usage-guide)
- [Technical Details](#technical-details)
- [Future Improvements](#future-improvements)

## 🎯 Overview

The **Coffee Shop Sales System** is a comprehensive management tool that allows administrators to:
- Manage coffee menu items (add, edit, delete)
- Track sales revenue
- Visualize sales data with charts
- Authenticate users with secure password hashing

This project was developed as part of a C++ programming course assignment.

## ✨ Features

### Authentication System
- **Login** with username/password (SHA-256 hashed)
- **Sign Up** for new admin accounts
- Persistent account storage in CSV format

### Admin Dashboard
- **View** all coffee items in an editable table
- **Add** new coffee items with validation:
  - Unique ID auto-generation
  - Single size selection (S/M/L)
  - Price and quantity validation
- **Edit** existing items (double-click to edit)
- **Delete** items with confirmation dialog

### Analytics & Reporting
- **Total Revenue Calculation** – Displays sum of (price × quantity sold)
- **Best Selling Coffee** – Identifies item with highest quantity sold
- **Revenue Chart** – Horizontal bar chart showing top 8 coffee items by revenue

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    MainWindow (Login)                   │
│                      │         │                        │
│            ┌─────────┘         └─────────┐             │
│            ▼                             ▼             │
│         SignUp                         Dialog2         │
│    (Account Creation)              (Admin Dashboard)   │
│                                          │             │
│                              ┌───────────┼───────────┐ │
│                              ▼           ▼           ▼ │
│                         addItem    CoffeeTableModel   │
│                        (Add Form)   (Data Model)     │
│                                          │             │
│                                          ▼             │
│                                     CSVReader         │
│                                    (File I/O)         │
└─────────────────────────────────────────────────────────┘
```

### Key Classes

| Class | Responsibility |
|-------|----------------|
| `MainWindow` | Login interface, account validation |
| `SignUp` | New account registration |
| `Dialog2` | Admin dashboard, CRUD operations, analytics |
| `addItem` | Form for adding new coffee items |
| `CoffeeTableModel` | Qt table model, CSV persistence, revenue calculations |
| `Coffee` | Data structure for coffee items |
| `CSVReader` | CSV parsing utility |
| `HashUtils` | SHA-256 password hashing |

## 📦 Prerequisites

- **Qt6 Framework** (6.5 or later)
- **CMake** (3.19 or later)
- C++17 compatible compiler

### Installing Qt6 on Ubuntu/Debian
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-charts-dev cmake build-essential
```

## 🔧 Installation & Build

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/CoffeeShopSystemMadeWithQT.git
cd CoffeeShopSystemMadeWithQT
```

2. **Create build directory**
```bash
mkdir build
cd build
```

3. **Configure with CMake**
```bash
cmake ..
```

4. **Build the project**
```bash
make -j$(nproc)
```

5. **Run the application**
```bash
./CoffeeShopSystemMadeWithQT
```

## 📁 File Structure

```
CoffeeShopSystemMadeWithQT/
├── CMakeLists.txt              # CMake build configuration
├── main.cpp                    # Application entry point
│
├── mainwindow.h/cpp/ui         # Login window
├── signup.h/cpp/ui             # Sign-up window
├── dialog2.h/cpp/ui            # Admin dashboard
├── additem.h/cpp/ui            # Add item form
│
├── coffee.h/cpp                # Coffee data class
├── coffeetablemodel.h/cpp      # Table model with CSV I/O
├── csvreader.h/cpp             # CSV parser
├── hashutils.h                 # SHA-256 hashing
│
├── database/                   # Data storage (created at runtime)
│   ├── dataSet.csv            # Coffee menu data
│   └── account.txt            # User accounts
│
└── README.md                   # This file
```

## 💾 Data Storage

### Coffee Data (`database/dataSet.csv`)
CSV format with columns:
```
name,id,price,size,quantity_sold,explanation
Espresso,1,3.50,S,150,Classic Italian coffee
Latte,2,4.50,M,200,Creamy milk coffee
...
```

### Account Data (`database/account.txt`)
```
username,hashed_password_sha256
admin,8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
```

## 📖 Usage Guide

### First Time Setup
1. Launch the application
2. Click **"Sign up"** to create an admin account
3. Log in with your new credentials

### Managing Coffee Items

#### Adding an Item
1. Click **"Add Item"** button
2. Fill in:
   - Item Name
   - Price (numeric, 0-999999)
   - Size (select **only one**: S, M, or L)
   - Quantity Sold
   - Explanation (optional)
3. Click **"Save"** – data auto-saves to CSV

#### Editing an Item
- **Double-click** any cell in the table
- Edit the value (ID column is read‑only)
- Changes are auto-saved to CSV

#### Deleting an Item
1. Select a row by clicking on it
2. Click **"Delete Item"**
3. Confirm deletion

### Analytics Dashboard
- **Total Sales Revenue** – Auto-updates when data changes
- **Best Selling Coffee** – Shows item with highest quantity sold
- **Revenue Chart** – Horizontal bar chart of top 8 items by revenue

## 🔧 Technical Details

### Password Security
- Passwords are hashed using **SHA-256** (`QCryptographicHash`)
- Plaintext passwords never stored
- Hash stored in `account.txt`

### CSV Handling
- Custom `CSVReader` handles quoted fields and commas
- `CoffeeTableModel::saveToCSV()` writes all changes immediately
- Automatic backup not implemented (future feature)

### Chart Implementation
- Uses **QtCharts** module (`QHorizontalBarSeries`)
- Dynamically updates when data changes
- Sorts items by revenue (ascending), shows highest 8

### Validations
| Field | Validation |
|-------|------------|
| Price | >0, ≤999999, up to 2 decimals |
| Quantity | ≥0, integer |
| Size | Exactly one of S, M, L |
| Username | Not empty, unique |

## 🚀 Future Improvements

- [ ] **Edit functionality** in Add Item dialog
- [ ] **Search/filter** coffee items
- [ ] **Export reports** to PDF/Excel
- [ ] **User roles** (admin, cashier, viewer)
- [ ] **Sales transaction logging**
- [ ] **Backup/restore** database
- [ ] **Dark mode** theme
- [ ] **Multi-language** support
- [ ] **Date-based** sales filtering

## 👨‍💻 Author

**Alfath Rafiuddin**  
*C++ Programming Course Project*

---

*This project was created for educational purposes as part of a university programming course.*
