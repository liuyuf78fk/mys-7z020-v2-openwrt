# MYS-7Z020-V2 Board Support
This repository contains the necessary configuration, device tree files, and build instructions for supporting the MYS-7Z020-V2 development board.

## Project Structure

- `kernel/`: Linux kernel source code and board-specific patches.
- `OpenWrt/`: Build configuration files for the root filesystem.
- `docs/`: Validation logs, screenshots, and hardware test results.
- `boot/`: Boot artifacts (BOOT.BIN, uEnv.txt, dtb, zImage).

---

## 1. Kernel Compilation

### 1.1 Initial Configuration

To apply the default configuration for the MYS-7Z020-V2 board:

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- mys7z020_router_defconfig
```

### 1.2 Modifying Configuration

If you need to change the kernel configuration:

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```

Then save defconfig:

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- savedefconfig
cp defconfig arch/arm/configs/mys7z020_router_defconfig
```

### 1.3 Build Kernel

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j$(nproc)
```

### 1.4 Build Device Tree

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- xilinx/zynq-mys-7z020-v2.dtb
```

---

## 2. OpenWrt Root Filesystem Build

### 2.1 Clone Source

```bash
git clone https://git.openwrt.org/openwrt/openwrt.git
cd openwrt
./scripts/feeds update -a
./scripts/feeds install -a
```

### 2.2 Apply Configuration

```bash
cp ../mys-7z020-v2.config-openwrt .config
make defconfig
```

### 2.3 Build

```bash
make V=s -j$(nproc)
```

---

## 3. SD Card Deployment

### Partition 1: FAT32 (Boot)

Copy files:
- BOOT.BIN
- uEnv.txt
- zImage
- zynq-mys-7z020-v2.dtb

### Partition 2: EXT4 (RootFS)

Extract rootfs:

```bash
tar -xzf openwrt-zynq-generic-xlnx_zynq-xxxx-targz-rootfs.tar.gz -C /mnt/rootfs
```

---

### LuCI Dashboard Preview
![LuCI Dashboard](docs/screenshots/home_page.png)

---

## License

GPLv2

