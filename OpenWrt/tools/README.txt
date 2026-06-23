===================================================================
                  PREEMPT_RT & NTP VERIFICATION GUIDE
===================================================================

This document describes how to verify that the PPS hardware interrupt
and the Chrony daemon (chronyd) have been successfully elevated to 
their respective Real-Time (RT) scheduling priorities upon boot.

In a fully functional PREEMPT_RT kernel environment, the time-sync 
hierarchy must follow this exact order: 
  PPS IRQ (Highest) > Chrony (High) > Normal Processes (Standard).

-------------------------------------------------------------------
1. VERIFICATION COMMANDS
-------------------------------------------------------------------

Run the following commands in the OpenWrt terminal to inspect the
low-level kernel scheduler state (/proc/[PID]/sched):

A. Verify Chrony (Should be set to Near-Maximum Priority: RT 85)
-----------------------------------------------------------------
Command:
  cat /proc/$(pidof chronyd)/sched | grep -E "policy|prio"

Expected Output:
  policy                                       :                    1
  prio                                         :                   14

* Note: 'policy: 1' indicates SCHED_FIFO (Real-Time). 'prio: 14' 
  is the kernel-space representation of user-space RT priority 85 
  (99 - 85 = 14).


B. Verify PPS Hardware Interrupt (Should be set to Absolute Maximum: RT 99)
-----------------------------------------------------------------
Command:
  cat /proc/$(ps | grep "irq/.*pps" | grep -v grep | awk '{print $1}')/sched | grep -E "policy|prio"

Expected Output:
  policy                                       :                    1
  prio                                         :                    0

* Note: 'policy: 1' indicates SCHED_FIFO. 'prio: 0' represents the 
  absolute highest real-time priority in the Linux kernel, ensuring 
  zero-jitter hardware timestamping.

-------------------------------------------------------------------
2. TROUBLESHOOTING
-------------------------------------------------------------------
If the output shows 'policy: 0' and 'prio: 120', it means the processes 
have fallen back to standard SCHED_OTHER scheduling. 

Ensure that the '/usr/bin/rt_optimize.sh' script is properly triggered 
by '/etc/rc.local' with a sufficient boot delay (e.g., sleep 30) to 
prevent the priority from being overridden during hotplug or network 
service reinitialization.
===================================================================
