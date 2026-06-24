#!/bin/sh

# ==================== CONFIGURATION ====================
TEMP_LIMIT=60000        # 60 degrees Celsius
# =======================================================

EXPECTED_SERIAL="0x48fcb33a"

alarm_beep() {
    local count=$1
    local delay=$2
    for i in $(seq 1 "$count"); do
        beep
        sleep "$delay"
    done
}

# --- 1. Temperature Check ---
if [ -f /sys/class/hwmon/hwmon0/temp1_input ]; then
    CURRENT_TEMP=$(cat /sys/class/hwmon/hwmon0/temp1_input)
    if [ "$CURRENT_TEMP" -ge "$TEMP_LIMIT" ]; then
        alarm_beep 5 1
        exit 1
    fi
fi

# --- 2. SD Card Hardware Identity Check ---
if [ -f /sys/block/mmcblk0/device/serial ]; then
    CURRENT_SERIAL=$(cat /sys/block/mmcblk0/device/serial)
    if [ "$CURRENT_SERIAL" != "$EXPECTED_SERIAL" ]; then
        # Wrong SD Card / Card swapped: Beep 3 times
        alarm_beep 3 1
        exit 2
    fi
else
    # SD Card completely missing or hardware dead: Beep 3 times
    alarm_beep 3 1
    exit 2
fi

# --- 3. SD Card / Filesystem Check ---
# Get mount line for "/" directly to avoid alias device name confusion
ROOT_LINE=$(awk '$2=="/" {print $0}' /proc/mounts)

if [ -z "$ROOT_LINE" ]; then
    # Empty mount info means major system failure
    alarm_beep 3 1
    exit 2
fi

# Extract the status (rw/ro) from the mount line
ROOT_STATUS=$(echo "$ROOT_LINE" | awk '{print $4}' | cut -d, -f1)

if [ "$ROOT_STATUS" = "ro" ]; then
    # SD card error or file system downgrade to read-only
    alarm_beep 3 1
    exit 2
fi

# --- 4. Chrony NTP Status Check ---
if command -v chronyc >/dev/null 2>&1; then
    CHRONY_INFO=$(chronyc tracking)
    LEAP_STATUS=$(echo "$CHRONY_INFO" | awk -F': ' '/Leap status/ {print $2}' | tr -d ' ')
    STRATUM=$(echo "$CHRONY_INFO" | awk -F': ' '/Stratum/ {print $2}' | tr -d ' ')

    if [ -z "$LEAP_STATUS" ] || [ -z "$STRATUM" ]; then
        alarm_beep 2 1
        exit 3
    fi

    if [ "$LEAP_STATUS" != "Normal" ] || [ "$STRATUM" -le 0 ] || [ "$STRATUM" -ge 4 ]; then
        alarm_beep 2 1
        exit 3
    fi
fi

exit 0
