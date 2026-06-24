#!/bin/sh

TEMP_FILE="/sys/class/hwmon/hwmon0/temp1_input"

if [ ! -f "$TEMP_FILE" ]; then
    echo "Error: temperature file not found"
    exit 1
fi

i=1

while [ $i -le 3 ]; do
	RAW=$(cat "$TEMP_FILE")
	# sysfs usually reports millidegree Celsius
	TEMP_C=$(awk "BEGIN {printf \"%.1f\", $RAW/1000}")

	echo "Temperature: ${TEMP_C} °C"

 	i=$((i + 1))
    	sleep 1
done
