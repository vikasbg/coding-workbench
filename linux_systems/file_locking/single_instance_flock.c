#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>  // Required for flock()
#include <errno.h>
#include <string.h>

// Define the lock file path globally so functions can access it
#define LOCK_FILE "/tmp/iobroker_service.lock"
static int lock_file_fd = -1; 

/*
 * NOTE ON STABILITY AND "FILE DELETION VULNERABILITY":
 * * This implementation uses filesystem-based locking (flock).
 * * Advantage:
 * - Simple to implement.
 * - Easy to debug (you can check if the file exists using 'ls' or 'cat').
 * - Crash Safe: If the app crashes/power fails, the OS automatically releases 
 * the lock, allowing the app to restart successfully on reboot.
 *
 * Vulnerability:
 * - If a user MANUALLY deletes the lock file while the app is running
 * (e.g., 'rm /tmp/modbus_master_app.lock'), the protection breaks.
 * A second instance will create a new file and lock it, resulting in two
 * running instances.
 *
 * Solution to Vulnerability:
 * - If protection against malicious/accidental file deletion is required, 
 * use "Linux Abstract Namespace Sockets" instead.
 * - Abstract sockets bind a name in kernel memory (starting with \0), 
 * creating no physical file on disk to delete.
 */

// ---------------------------------------------------------------------------
// FUNCTIONS
// ---------------------------------------------------------------------------

/**
 * Function: init_application_lock
 * -------------------------------
 * Purpose: 
 * 1. Creates/Opens the lock file.
 * 2. Acquires an Exclusive Lock to prevent duplicates.
 * 3. Writes the current PID for debugging.
 * 4. FATAL EXIT: If the lock fails (app already running), this function 
 * terminates the program immediately.
 * * Flags Used:
 * O_CREAT | O_RDWR: Create file if missing, open for Read/Write.
 * LOCK_EX: Exclusive Lock (Only one process can hold this).
 * LOCK_NB: Non-Blocking (Don't wait; fail immediately if locked).
 *
 * Returns: 
 * The file descriptor (fd) which MUST be kept open.
 */
int init_application_lock()
{
    int fd;
    char pid_str[16];

    // 1. Open the file
    fd = open(LOCK_FILE, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("FATAL: Could not open lock file");
        exit(EXIT_FAILURE); // Exit immediately
    }

    // 2. Try to Lock
    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            fprintf(stderr, "ERROR: Application is already running! (Lock held)\n");
        } else {
            perror("ERROR: Failed to acquire lock");
        }
        close(fd);
        exit(EXIT_FAILURE); // Exit immediately so main() doesn't run
    }

    // 3. Write PID (Optional but recommended)
    //    ftruncate clears the file first so we don't append to old data
    ftruncate(fd, 0);
    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    write(fd, pid_str, strlen(pid_str));

    printf("[System] Lock acquired. Application started (PID: %d).\n", getpid());
    
    return fd;
}

/**
 * Function: cleanup_application_lock
 * ----------------------------------
 * Purpose:
 * 1. Releases the lock explicitly.
 * 2. Closes the file descriptor.
 * 3. Deletes the file from the filesystem.
 *
 * Note: 
 * Even if this function is NOT called (e.g., power loss), 
 * the OS will automatically release the lock, ensuring safety.
 * 
 * Automatic Crash Recovery:
 * If we unplug the power, cleanup_application_lock will not run. The file /tmp/modbus_application.lock will remain on the disk.
 * However, when you reboot and run the app again, flock will succeed. Why? Because file locks are held by the kernel for a specific process. 
 * When the process dies (from crash or reboot), the kernel invalidates the lock, even if the file is still there.
 */
void cleanup_application_lock(int fd)
{
    if (fd != -1) {
        // 1. Explicitly unlock (Optional, closing the FD does this too)
        flock(fd, LOCK_UN);

        // 2. Close the file descriptor
        close(fd);
        
        // 3. Remove the file from the filesystem(/tmp) (Optional)
        // Even if you DON'T do this, the mechanism still works because
        // the next instance will just lock the existing file.
        // However, removing it keeps the /tmp directory clean.
        unlink(LOCK_FILE);
        
        printf("[System] Lock released. Cleanup complete.\n");
    }
}

// --- MAIN FUNCTION ---
int main()
{
    // 1. Initialization Section
    //    If this fails, the program exits inside the function.
    lock_file_fd = init_application_lock();

    // 2. Main Loop / Work Section
    //    (Waiting for user input to simulate a running process)
    printf("Press Enter to stop the application...\n");
    getchar();

    // 3. Cleanup Section
    cleanup_application_lock(lock_file_fd);

    return 0;
}