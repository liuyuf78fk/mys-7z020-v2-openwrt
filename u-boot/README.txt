U-Boot for MYS-7Z020-V2

This directory contains the U-Boot source code for the MYS-7Z020-V2 board, customized with specific boot logic to ensure compatibility with uEnv.txt and the system kernel.  

Build Instructions

To compile the U-Boot binary, run the following commands in this directory:

# Configure U-Boot for MYS-7Z020-V2
make CROSS_COMPILE=arm-linux-gnueabihf- xilinx_zynq_virt_defconfig

# Compile with 8 parallel jobs
make CROSS_COMPILE=arm-linux-gnueabihf- -j8

Deployment
1. After compilation, copy the generated u-boot.elf to the ../scripts/ directory.
2. Run the build script to generate the BOOT.BIN image:
    cd ../scripts/
    ./make_boot.sh
3. Copy the resulting BOOT.BIN to the first FAT32 partition of your SD card.

Troubleshooting

If the board fails to boot automatically or defaults to the wrong environment, you need to manually set the boot command. Once the board reaches the U-Boot command prompt (Zynq>), execute the following commands:

    setenv bootcmd "run default_bootcmd"
    saveenv
    reset

Note: This configuration ensures that U-Boot prioritizes reading uEnv.txt from the SD card before proceeding with the standard kernel boot process.

Changing the Ethernet MAC Address
The default Ethernet MAC address is stored in the U-Boot environment variable `ethaddr`.
To change the MAC address, stop at the U-Boot prompt and run:

    env set -f ethaddr 00:0a:35:01:02:03
    env save

Verify the change:
    printenv ethaddr
