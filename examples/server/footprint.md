# Dropbear ESP-IDF Footprint

Tested on **ESP32**, IDF v5.4, server example with Ed25519 host key.

## Flash Size (`idf.py size-components`)

Binary size: **917 KB** (10% of 1 MB partition free)

Key components:

| Archive | Total | Flash Code | Flash Data (.rodata) | DRAM (.data+.bss) |
|---|---:|---:|---:|---:|
| **libdropbear.a** | **119 KB** | 106 KB | 10 KB | 3 KB |
| libmain.a (app) | 3 KB | 3 KB | < 1 KB | 0 |
| libespressif__sock_utils.a | 1 KB | 1 KB | < 1 KB | 0 |

For reference, largest non-SSH components: libnet80211.a (141 KB), liblwip.a (99 KB),
libc.a (67 KB), libwpa_supplicant.a (63 KB), libmbedcrypto.a (60 KB).

## Runtime Memory (Heap)

Heap consumption during an SSH session lifecycle:

| Stage | Free Heap | Delta | Min Free Ever |
|---|---:|---:|---:|
| Before `dropbear_setup()` | 263,660 | — | 260,916 |
| After `dropbear_setup()` | 263,236 | −0.4 KB | 260,916 |
| After session accepted | 262,244 | −1.4 KB | 260,340 |
| Session ready (auth + channel + shell) | 239,636 | −24 KB total | 231,448 |

- **Total heap cost of one SSH session: ~24 KB**
- `dropbear_setup()` is cheap (~0.4 KB); key exchange + channel + shell is the expensive part (~24 KB)

## Runtime Memory (Stack)

Main task stack (`CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192`):

| Stage | Stack HWM (words free) | Used |
|---|---:|---:|
| Before `dropbear_setup()` | 5,476 | ~2.7 KB |
| After session accepted | 5,284 | ~2.9 KB |
| Session ready (auth + channel + shell) | 3,860 | ~4.3 KB |

Peak stack usage is ~4.3 KB, leaving ~3.9 KB headroom with the default 8 KB stack.

## Task List (during active session)

No additional tasks are spawned. The shell runs in the main task context (same as libssh).

| Task | Description | Stack HWM | Prio |
|---|---|---:|---:|
| main | Dropbear server + shell (application) | 3,860 | 1 |
| IDLE0 / IDLE1 | FreeRTOS idle (dual-core) | ~680 | 0 |
| tiT | lwIP TCP/IP | ~1,400 | 18 |
| Tmr Svc | FreeRTOS timer service | ~1,300 | 1 |
| wifi | WiFi driver | ~3,400 | 23 |
