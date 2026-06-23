#!/bin/sh

sleep 30

PPS_PID=$(ps | grep -E "irq/.*pps" | grep -v grep | awk '{print $1}')
if [ -n "$PPS_PID" ]; then
    /usr/bin/setrt $PPS_PID 99
    logger -t RT_INIT "Finally bumped PPS IRQ (PID: $PPS_PID) to RT 99"
fi

CHRONY_PID=$(pidof chronyd)
if [ -n "$CHRONY_PID" ]; then
    /usr/bin/setrt $CHRONY_PID 85
    logger -t RT_INIT "Finally bumped Chronyd (PID: $CHRONY_PID) to RT 85"
fi

