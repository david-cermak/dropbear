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
| Before `dropbear_setup()` | 266,148 | — | 263,424 |
| After `dropbear_setup()` | 265,724 | −0.4 KB | 263,424 |
| After session accepted | 264,732 | −1.0 KB | 262,828 |
| Session ready (auth + channel + shell) | 234,432 | −30.3 KB | 233,956 |

- **Total heap cost of one SSH session: ~32 KB**
- `dropbear_setup()` is cheap (~0.4 KB); key exchange + channel + shell task is the expensive part (~30 KB)

## Runtime Memory (Stack)

Main task stack (`CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192`):

| Stage | Stack HWM (words free) | Used |
|---|---:|---:|
| Before `dropbear_setup()` | 5,484 | ~2.7 KB |
| After session accepted | 5,292 | ~2.9 KB |
| Session ready (auth + channel + shell) | 3,868 | ~4.3 KB |

Peak stack usage is ~4.3 KB, leaving ~3.9 KB headroom with the default 8 KB stack.

## Task List (during active session)

The shell spawns one additional task (`esp_shell`, 4 KB stack) when a session channel is opened.

| Task | Description | Stack HWM | Prio |
|---|---|---:|---:|
| main | Dropbear server (application) | 3,868 | 1 |
| esp_shell | Embedded shell (per session) | ~3 KB | 5 |
| IDLE0 / IDLE1 | FreeRTOS idle (dual-core) | ~680 | 0 |
| tiT | lwIP TCP/IP | ~1,400 | 18 |
| Tmr Svc | FreeRTOS timer service | ~1,300 | 1 |
| wifi | WiFi driver | ~3,400 | 23 |
