#include <stdio.h> 

#include <stdlib.h> 

#include <sys/mman.h> 

#include <string.h> 

#include <fcntl.h> 

#include <unistd.h> 

 

#define FILE_SIZE 100 

 

struct file_mapping_entry { 

    char file_name[50]; 

    void *mapped_address; 

}; 

 

struct file_mapping_entry file_mapping_table[10]; 

int table_index = 0; 

 

void add_file_mapping_entry(const char *file_name, void *mapped_address) { 

    strcpy(file_mapping_table[table_index].file_name, file_name); 

    file_mapping_table[table_index].mapped_address = mapped_address; 

    table_index++; 

} 

 

int main() { 

    int fd; 

    off_t offset = 0; 

    void *addr; 

    char *file_name = "filemapped.txt"; 

 

    fd = fopen(file_name, "w");

    if(fd == -1) { 

        perror("open"); 

        exit(1); 

    } 

 

    addr = mmap(NULL, FILE_SIZE,PROT_WRITE, MAP_SHARED, fd, offset); 

    if(addr == MAP_FAILED) { 

        perror("mmap"); 

        exit(1); 

    } 

 

    add_file_mapping_entry(file_name, addr); 

 

    // Access the mapped file 

    sprintf(addr, "Hello, this is a test message!"); 

 

    // Unmap the file 

    munmap(addr, FILE_SIZE); 

 

    close(fd); 

 

    return 0; 

} 