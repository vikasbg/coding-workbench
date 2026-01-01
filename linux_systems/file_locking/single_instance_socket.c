#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>

// Unique name for your application. 
// The leading '@' (or null byte) makes it "Abstract" (invisible on filesystem).
#define LOCK_NAME "modbus_master_app_lock" 

/**
 * Function: init_application_lock
 * -------------------------------
 * Uses a Linux Abstract Namespace Socket to ensure single instance.
 * No file is created on disk, so it cannot be deleted by users.
 */
int init_application_lock()
{
    int fd;
    struct sockaddr_un addr;

    // 1. Create a socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("FATAL: Could not create lock socket");
        exit(EXIT_FAILURE);
    }

    // 2. Prepare the address
    // We use the abstract namespace (first byte is \0)
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    // We copy the name to sun_path, starting at index 1. 
    // Index 0 must remain 0 (null) to indicate abstract namespace.
    strncpy(addr.sun_path + 1, LOCK_NAME, sizeof(addr.sun_path) - 2);

    // 3. Bind the socket
    // If this specific name is already bound by another process, bind() will fail.
    if (bind(fd, (struct sockaddr*)&addr, sizeof(sa_family_t) + strlen(LOCK_NAME) + 1) == -1) {
        if (errno == EADDRINUSE) {
            fprintf(stderr, "ERROR: Application is already running! (Socket bound)\n");
        } else {
            perror("ERROR: Failed to bind lock socket");
        }
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("[System] Lock acquired via Abstract Socket. Application started.\n");
    
    // We must keep 'fd' open. If we close it, the lock is lost.
    return fd;
}

/**
 * Function: cleanup_application_lock
 * ----------------------------------
 * Simply closes the socket. The kernel handles the rest.
 */
void cleanup_application_lock(int fd)
{
    if (fd != -1) {
        close(fd);
        printf("[System] Socket closed. Lock released.\n");
    }
}

int main()
{
    // 1. Initialization
    int lock_fd = init_application_lock();

    // 2. Main Loop
    printf("Press Enter to stop the application...\n");
    getchar();

    // 3. Cleanup
    cleanup_application_lock(lock_fd);

    return 0;
}