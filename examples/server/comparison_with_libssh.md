# Dropbear vs libssh — ESP-IDF Comparison

Side-by-side comparison of the Dropbear and libssh SSH server examples on ESP-IDF.

**Note:** libssh was measured on ESP32-S3 with IDF v5.5; Dropbear on ESP32 with IDF v5.4. Platform and IDF version differences may affect some numbers.

## Flash Size

| Metric | Dropbear | libssh |
|---|---:|---:|
| **Binary size** | 917 KB | 1,057 KB |
| **Partition free** | 10% (1 MB partition) | 31% |
| **SSH library** | libdropbear.a: 119 KB | liblibssh.a: 233 KB |
| **Crypto (SSH)** | (included in libdropbear) | libmbedcrypto.a: 78 KB |
| **Total SSH stack** | **~119 KB** | **~311 KB** |

Dropbear uses **~140 KB less flash** overall. The SSH stack (library + crypto) is **~192 KB smaller** with Dropbear, which bundles its own crypto (libtomcrypt) rather than using mbedTLS.

## Runtime Memory (Heap)

| Stage | Dropbear | libssh |
|---|---:|---:|
| Before init/setup | 264 KB | 258 KB |
| After init/setup | 263 KB (−0.4 KB) | 255 KB (−2.6 KB) |
| After accept | 262 KB (−1.4 KB) | 250 KB (−5.5 KB) |
| Session ready | 240 KB (−24 KB total) | 230 KB (−28 KB total) |

| Metric | Dropbear | libssh |
|---|---:|---:|
| **Heap per session** | ~24 KB | ~28 KB |
| **Init cost** | ~0.4 KB | ~2.6 KB |

Dropbear uses **~4 KB less heap** per session than libssh (24 KB vs 28 KB). Dropbear’s init is lighter (~0.4 KB vs ~2.6 KB).

## Runtime Memory (Stack)

| Stage | Dropbear (main) | libssh (main) |
|---|---:|---:|
| Before init | ~2.7 KB used | ~2.7 KB used |
| After accept | ~2.9 KB used | ~3.3 KB used |
| Session ready | ~4.3 KB used | ~4.7 KB used |

Both stay within the default 8 KB main stack. libssh uses slightly more main-task stack at peak (~4.7 KB vs ~4.3 KB).

## Tasks

| | Dropbear | libssh |
|---|---|---|
| **Extra tasks** | 0 | 0 |
| **Shell execution** | Main task | Main task |

Both run the shell in the main task context.

## Summary

| Aspect | Dropbear | libssh |
|---|---|---|
| **Flash** | Smaller (~140 KB less binary, ~192 KB less SSH stack) | Larger |
| **Heap per session** | ~24 KB | ~28 KB |
| **Init heap** | ~0.4 KB | ~2.6 KB |
| **Extra tasks** | 0 | 0 |
| **Crypto** | libtomcrypt (bundled) | mbedTLS |

**When to prefer Dropbear:** Tighter flash budget, lower heap per session (~24 KB vs ~28 KB), simpler crypto integration (no separate mbedTLS for SSH).

**When to prefer libssh:** Shared mbedTLS if already used elsewhere.
