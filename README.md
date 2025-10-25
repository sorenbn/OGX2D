# OGX2D

A small **XNA-inspired framework** for making **OG Xbox** games using **RXDK**.

## Prerequisites

### Required
- **[RXDK](https://github.com/XboxDev/nxdk)** or a compatible **Xbox Development Kit (XDK)** setup  
  Ensure your build environment is configured correctly.

---


### If you want to play on an emulator
- [**xemu**](https://xemu.app/) — an open-source OG Xbox emulator

### If you want to play on real Xbox hardware
- A **softmodded Xbox**  
  (see [Rocky5’s Xbox Softmodding Tool](https://www.youtube.com/watch?v=FqgQWe_r5I4))
- An **FTP client**, such as [WinSCP](https://winscp.net/) or FileZilla

---

## How to Build & Run

### 1. Build the Solution
Compile the project in **Debug** or **Release** mode as usual.

---

### 2. Run in xemu (Emulator)

1. Open **xemu**  
2. Go to **Machine → Load Disc**  
3. Navigate to:  
   ```
   <project_path>/Debug|Release/OGX2D.iso
   ```
4. The game should start running in the emulator.

---

### 3. Run on Original Xbox Hardware

1. Open your FTP client and connect to your Xbox:
   ```
   Host: <Xbox IP address> (shown on your softmod dashboard)
   Protocol: FTP
   Port: 21
   Username: xbox
   Password: xbox
   ```

2. Navigate to the build folder:
   ```
   <project_path>/Debug|Release/Build/OGX2D
   ```

3. Copy the entire **OGX2D** folder to your Xbox:
   ```
   E:/Applications/
   ```

4. Soft-restart your Xbox.  
   The application will appear in your **Applications** list on the dashboard.

---