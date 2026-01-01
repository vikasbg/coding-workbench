# Single Instance Locking Techniques

Two robust methods to ensure only one instance of an application runs at a time on Linux.

## 1. File Locking (`single_instance_flock.c`)
Uses the filesystem `flock()` API.

* **Mechanism:** Locks a physical file (e.g., `/tmp/app.lock`).
* **Pros:** Easy to debug (you can verify the file exists with `ls`).
* **Cons:** Vulnerable if a user manually deletes the lock file while the app is running.
* **Crash Safety:** ✅ Excellent. The kernel releases the lock automatically if the process dies.

## 2. Abstract Namespace Socket (`single_instance_socket.c`)
Uses Unix Domain Sockets in the abstract namespace.

* **Mechanism:** Binds a name in the kernel's memory (e.g., `@modbus_app_lock`).
* **Pros:** **Invisible and Invulnerable.** No file exists on disk, so users cannot accidentally delete it.
* **Cons:** Harder to debug (requires `netstat -xp` or `ss -x` to see).
* **Crash Safety:** ✅ Excellent. The kernel closes the socket automatically if the process dies.

---
**Recommendation:** Use **Method 2 (Sockets)** for production industrial applications where reliability is critical. Use **Method 1 (File)** for simple tools where you need visibility.