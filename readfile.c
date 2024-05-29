#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[])
{
    int          fd,i;
    void         *map;
    off_t        offset, pa_offset;
    size_t       length;
    ssize_t      s;
    struct stat  sb;
   char *fakeArgs[] = {"readfile", "FileIn.txt", "FileOut.txt", "Demo.txt", NULL};
   // Gán giá trị mới cho argc và argv
   argc = sizeof(fakeArgs) / sizeof(fakeArgs[0])-1;
   argv = fakeArgs;
   // In ra tất cả các đối số dòng lệnh
   for (i = 0; i < argc; i++) {
       fprintf("argv[%d]: %s\n", i, argv[i]);
   }
   fprintf("%d \n", argc);
    if (argc < 3 || argc > 4) {
        fprintf("%s file offset \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");
    if (fstat(fd, &sb) == -1)           /* To obtain file size */
        handle_error("fstat");
    offset = 0x2400;
    printf("kich thuoc trang %d \n",sysconf(_SC_PAGE_SIZE));// co dinh kich thuoc cua mot trang 4096 bytes ~ 4KB=4MB
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
        /* offset for mmap() must be page aligned, tuc lam cho offset vi tri bat dau cua file luon phai la boi so cua trang*/
        
    printf("offset truoc khi can chinh %d\n", offset);
    printf("offset sau khi can chinh %d \n", pa_offset);
    printf("kich thuoc cua file in %ld\n", sb.st_size); 

    if (offset >= sb.st_size) {
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 4) {
        length = 100000;
        //atoi(argv[1]);
        if (offset + length > sb.st_size)
            length = sb.st_size - offset;
            printf("do dai length %d \n",length);
                /* Can't display bytes past end of file *
    } else {    /* No length arg ==> display to end of file */
        length = sb.st_size - offset;
    }
    //length = sb.st_size - offset;
    map = mmap(NULL, length + offset - pa_offset, PROT_READ,
                MAP_PRIVATE, fd, pa_offset);
    if (map == MAP_FAILED)
        handle_error("mmap");
    printf("-------------\n");
    s = write(STDOUT_FILENO, map + offset - pa_offset, length);
    if (s != length) {
        if (s == -1)
            handle_error("write");
        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);
    }
    printf("\n -------------\n");
    printf("Da in ra thanh cong \n");
    munmap(map, length + offset - pa_offset);
    close(fd);
    exit(EXIT_SUCCESS);
}



// int main(int argc, char *argv[]) {

//     const char * filename="/home/duy/MemoryMapedFiles/sometext.txt";// duong dan cua file
//     void *data = (void *)0x01000000; /* Address at which to map. */

//     printf("dia chi tren ram %d\n",data);

//     int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR); /* Open file. */
//     if (fd == -1) {
//         printf("khong mo dc file");
//         perror("open");
//         return 1;   
//     }

//     struct stat st;

//     if(fstat(fd,&st)==-1){ // dua vao bo dem
//       perror("stat");
//       return 1;
//     }
//     size_t file_size = st.st_size;

//     printf("file size is %ld : da dua vao bo dem \n",file_size);
    
    
//     void *map = mmap(data, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /* Map file. */
    
//     if (map == MAP_FAILED) {
//         perror("mmap");
//         printf("loi anh xa\n");
//         close(fd);
//         return 1;
//     }
    
//     // char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
//     // printf("Printting...\n");
  
//     // for(int i=0; i < sb.st_size; i++ ){
//     //   //file_in_memory[i]=toupper(file_in_memory[i]);
//     //   file_in_memory[i]=file_in_memory[i]+1;
//     //   printf("%c",file_in_memory[i]);
//     // }
  
//     // printf("\n");
  

//     ssize_t bytes = write(STDOUT_FILENO, map, file_size);
//     printf("\n");
//     printf("so bytes %zd duoc viet ra console \n",bytes);

//     if (munmap(map, file_size) == 0) { /* Unmap file (optional). */
//         perror("munmap");
//         close(fd);
//         return 1;
//     }

//     close(fd);
//     return 0;
// }
