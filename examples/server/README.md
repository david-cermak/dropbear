# Dropbear SSH server (ESP-IDF)

Minimal SSH server example using Dropbear on ESP-IDF, with an embedded shell.

For detailed flash and runtime memory footprint, see [footprint.md](footprint.md). For a comparison with libssh, see [comparison_with_libssh.md](comparison_with_libssh.md).

## Memory usage

Typical heap usage on ESP32 (from memory stats at key points):

| Stage | Free heap | Heap used (delta) | Main task stack HWM |
|-------|-----------|--------------------|---------------------|
| **before dropbear_setup** (WiFi connected) | ~266 KB | — | ~5.5K words free |
| **after dropbear_setup** (crypto, hostkey) | ~266 KB | ~0.4 KB | ~5.3K words free |
| **after session accepted** | ~265 KB | ~1 KB total | ~5.3K words free |
| **session ready** (auth + channel + shell) | ~234 KB | ~32 KB total | ~3.9K words free |

### Summary

- **Dropbear setup (~0.4 KB):** crypto init and hostkey loading.
- **Session accept (~1 KB):** per-connection session state.
- **Session ready (~30 KB):** full SSH handshake, auth, channel setup, and shell task (4 KB stack).

The largest allocation occurs when the session becomes ready (auth + channel + shell). Plan for ~32 KB additional heap per active SSH session.

## Build and run

```bash
cd examples/server && idf.py build flash monitor
```

Connect from host:

```bash
ssh user@<device-ip> -p 2222
```

Default credentials: `user` / `password`.

## Shell commands

- `help` — list commands
- `hello` — print greeting
- `uptime` — show uptime in ms
- `heap` — show free heap
- `stats` — show task and heap stats (when `ENABLE_MEMORY_STATS` is 1)
- `reset` — restart ESP32
- `exit` — close session

## Memory stats

Set `ENABLE_MEMORY_STATS` to 1 in `main/mem_stats.h` to enable heap and task logging at startup and after each connection. The `stats` shell command will print per-task stack high-water marks and heap summary.
