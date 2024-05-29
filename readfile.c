#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>


int main(int argc, char *argv[]) {

    const char * filename="/home/duy/MemoryMapedFiles/sometext.txt";// duong dan cua file
    void *data = (void *)0x01000000; /* Address at which to map. */

    printf("dia chi tren ram %d\n",data);

    int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR); /* Open file. */
    if (fd == -1) {
        printf("khong mo dc file");
        perror("open");
        return 1;   
    }

    struct stat st;

    if(fstat(fd,&st)==-1){ // dua vao bo dem
      perror("stat");
      return 1;
    }
    size_t file_size = st.st_size;

    printf("file size is %ld : da dua vao bo dem \n",file_size);
    
    
    void *map = mmap(data, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /* Map file. */
    
    if (map == MAP_FAILED) {
        perror("mmap");
        printf("loi anh xa\n");
        close(fd);
        return 1;
    }
    
    // char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
    // printf("Printting...\n");
  
    // for(int i=0; i < sb.st_size; i++ ){
    //   //file_in_memory[i]=toupper(file_in_memory[i]);
    //   file_in_memory[i]=file_in_memory[i]+1;
    //   printf("%c",file_in_memory[i]);
    // }
  
    // printf("\n");
  

    ssize_t bytes = write(STDOUT_FILENO, map, file_size);
    printf("\n");
    printf("so bytes %zd duoc viet ra console \n",bytes);

    if (munmap(map, file_size) == 0) { /* Unmap file (optional). */
        perror("munmap");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
