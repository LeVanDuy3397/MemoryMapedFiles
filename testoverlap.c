#include <stdio.h> 

#include <stdlib.h> 

#include <unistd.h> 

#include <sys/types.h> 

#include <sys/stat.h> 

#include <fcntl.h> 

#include <sys/mman.h> 

 

int main() { 

    int fd; 

    size_t file_size = 1024 ; // 1MB 

    char *file_name = "testfile.txt"; 

    void *mapped1, *mapped2; 

    void *desired_address = (void *)0x10000000; // Địa chỉ cụ thể 

     
    
    // Mở file 

    fd = open(file_name, O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR)); 

    if (fd == -1) { 

        perror("open"); 

        exit(EXIT_FAILURE); 

    } 

     

    // Ánh xạ file vào bộ nhớ tại địa chỉ mong muốn 

    mapped1 = mmap(desired_address, file_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0); 

    if (mapped1 == MAP_FAILED) { 
        

        perror("mmap"); 

        exit(EXIT_FAILURE); 

    } 

     

    // Thử ánh xạ một phần của file vào cùng địa chỉ mong muốn 

    mapped2 = mmap(desired_address, file_size/2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS, -1, 0); 

    if (mapped2 == MAP_FAILED) { 

        perror("mmap"); 

        exit(EXIT_FAILURE); 

    } 

     

    printf("Anh xạ 1: [%p, %p]\n", mapped1, mapped1 + file_size); 

    printf("Anh xạ 2: [%p, %p]\n", mapped2, mapped2 + file_size/2); 

     

    // Kiểm tra xem ánh xạ có bị overlap không 

    if ((mapped2 >= mapped1 && mapped2 < mapped1 + file_size) || (mapped1 >= mapped2 && mapped1 < mapped2 + file_size/2)) { 

        printf("Overlap detected!\n"); 

    } else { 

        printf("No overlap detected.\n"); 

    } 

 

    // Hủy ánh xạ file khỏi bộ nhớ 

    if (munmap(mapped1, file_size) == -1) { 

        perror("munmap"); 

        exit(EXIT_FAILURE); 

    } 

    if (munmap(mapped2, file_size/2) == -1) { 

        perror("munmap"); 

        exit(EXIT_FAILURE); 

    } 

     

    // Đóng file 

    if (close(fd) == -1) { 

        perror("close"); 

        exit(EXIT_FAILURE); 

    } 

     

    return 0; 

} 