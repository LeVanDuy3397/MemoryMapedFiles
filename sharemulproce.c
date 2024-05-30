#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_PATH "example.txt"
#define FILE_SIZE 4096

int main() {
    // Mở file
    int fd = open(FILE_PATH, O_RDWR | O_CREAT, (mode_t)0600);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Đặt kích thước file
    if (lseek(fd, FILE_SIZE - 1, SEEK_SET) == -1) {
        close(fd);
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    if (write(fd, "", 1) == -1) {
        close(fd);
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Ánh xạ file vào bộ nhớ
    char *map_shared = mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map_shared == MAP_FAILED) {
        close(fd);
        printf("111");
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Đóng file
    close(fd);

    // Ghi dữ liệu vào vùng nhớ đã ánh xạ
    sprintf(map_shared, "Hello, World! (Shared)");
    fd = open(FILE_PATH, O_RDWR | O_CREAT, (mode_t)0600);
    // Ánh xạ file lại với cờ MAP_PRIVATE (copy-on-write)
    char *map_private = mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map_private == MAP_FAILED) {
        printf("222");
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Ghi dữ liệu vào vùng nhớ đã ánh xạ
    sprintf(map_private, "Hello, World! (Private)");

    // Hiển thị dữ liệu từ vùng nhớ đã ánh xạ
    printf("Data from shared mapping: %s\n", map_shared);
    printf("Data from private mapping: %s\n", map_private);

    // Giải phóng bộ nhớ đã ánh xạ
    if (munmap(map_shared, FILE_SIZE) == -1) {
        perror("munmap");
    }
    if (munmap(map_private, FILE_SIZE) == -1) {
        perror("munmap");
    }

    return 0;
}