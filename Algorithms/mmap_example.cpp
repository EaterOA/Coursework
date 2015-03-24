#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

/*
 * Example usage of mmap
 * 
 * Computes the xor sum of every aligned 4 bytes in a given file. The work is
 * split amongst multiple subprocesses. All children read from the same mmap'd
 * region of the file, and their result is written to a separate anonymously
 * mmap'd shared memory region. The parent then reads from the shared region
 * and reduces them to a final answer.
 * 
 */
int main(int argc, char** argv)
{
    // Arguments
    if (argc < 2) {
        cerr << argv[0] << " <file>\n";
        return 1;
    }

    // Constants
    const int NUM_WORKERS = 4;

    // Open file descriptor
    int fd = open(argv[1], O_RDONLY);

    // Find filesize
    struct stat sb;
    fstat(fd, &sb);
    off_t size = sb.st_size;

    // mmap file and close fd
    char* data = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    // Anonymous shared memory region for reduction
    int* res = (int*)mmap(NULL, NUM_WORKERS * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    // Split work amongst processes and spawning children
    off_t intmask = sizeof(int)-1; // alignment
    off_t chunk = (size / NUM_WORKERS + intmask) & (~intmask);
    off_t offset = 0;
    off_t boundary = 0;
    for (int i = 0; i < NUM_WORKERS; i++) {
        int pid = fork();
        if (pid == 0) {
            offset = i * chunk;
            boundary = offset + chunk;
            if (boundary > size)
                boundary = size;
            break;
        }
    }

    int sum = 0;
    // worker
    if (boundary != 0) {
        for (off_t i = offset; i+4 <= boundary; i += 4)
            sum ^= *(int*)(data+i);
        res[offset/chunk] = sum;
    }
    // parent
    else {
        for (int i = 0; i < NUM_WORKERS; i++)
            wait(NULL);
        for (int i = 0; i < NUM_WORKERS; i++)
            sum ^= res[i];
        cout << sum << "\n";
    }

    // Unmap file and shared memory
    munmap(data, size);
    munmap(res, NUM_WORKERS*sizeof(int));
}
