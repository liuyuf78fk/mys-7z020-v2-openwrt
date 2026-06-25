# Key Monitor for OpenWrt/Linux

A background daemon for OpenWrt/Linux systems that monitors hardware key events (`gpio-keys`) and dismisses system alarms.

## Features
- Uses `epoll` for event-driven key monitoring.
- Keeps LED sysfs file descriptors open to avoid repeated open/close operations.
- Intended to run with low scheduling priority (`nice 19`) to minimize impact on other system tasks.

## Deployment
1. Compile the source: `$(CC) -O2 src/key_monitor.c -o key_monitor`
2. Move binary to `/usr/bin/key_monitor`
3. Copy `files/key_monitor.init` to `/etc/init.d/key_monitor`
4. Run `/etc/init.d/key_monitor enable && /etc/init.d/key_monitor start`
