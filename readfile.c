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
    do { perror(msg); exit(EXIT_FAILURE); } while (0) /*them kha nang xu ly loi hieu qua hon*/


int main(int argc, char *argv[])
{
   int          fd,i;
   void         *map;
   off_t        offset, pa_offset;
   size_t       length;
   ssize_t      s;
   struct stat  sb;

   char *fakeArgs[] = {"readfile", "sometextToRead.txt", "Demo1.txt", "Demo2.txt", NULL};
   
   // Gán giá trị mới cho argc và argv
   argc = sizeof(fakeArgs) / sizeof(fakeArgs[0])-1;
   argv = fakeArgs;
   // In ra tất cả các đối số dòng lệnh
   for (i = 0; i < argc; i++) {
       printf("argv[%d]: %s\n", i, argv[i]);
   }
      printf("%d \n", argc);
   if (argc < 3 || argc > 4) {
       printf("%s file offset \n", argv[0]);
       exit(EXIT_FAILURE);
   }
   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
       handle_error("open");
   if (fstat(fd, &sb) == -1)          
       handle_error("fstat");
   
   offset = 0x2900;// them dia chi bat dau de in ra file den cuoi cua file
   printf("page size %d \n",sysconf(_SC_PAGE_SIZE));// co dinh kich thuoc cua mot trang 4096 bytes ~ 4KB=4MB
   
   pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
       /*them kha nang can chinh offset trong trang truoc, sao cho offset
        la vi tri bat dau cua file phai la boi so cua kich thuoc trang, de trang dich chuyen
        den vi tri cuoi cung co the in ra bytes cua file*/
       
   printf("offset before aligned %d\n", offset);
   printf("offset after aligned %d \n", pa_offset);
   printf("file size of somtextToRead %ld\n", sb.st_size);    

   if (offset >= sb.st_size) { //them kha nang xac dinh offset co bi lon hon kich thuoc file khong
       fprintf(stderr, "offset is past end of file\n");
       exit(EXIT_FAILURE);
   }

   if (argc == 4) {
       length = 100000;
       //atoi(argv[1]);
       if (offset + length > sb.st_size)
           length = sb.st_size - offset;
           printf("length is %d \n",length);
               /* Can't display bytes past end of file *
   } else {    /* No length arg ==> display to end of file */
       length = sb.st_size - offset;
   }

   map = mmap(NULL, length + offset - pa_offset, PROT_READ,
               MAP_PRIVATE, fd, pa_offset);
   if (map == MAP_FAILED)
       handle_error("mmap");

   // Track mapping information
   printf("Track mapping information:\n");
   printf("Mapped address: %p\n", map);//la dia chi duoc anh xa tren bo nho RAM
   printf("File size: %ld bytes\n", sb.st_size);
   printf("-------------\n");

//    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   
//    printf("Printting...\n");
   
//    for(int i=0; i < sb.st_size; i++ ){
//      //file_in_memory[i]=toupper(file_in_memory[i]);
//      file_in_memory[i]=file_in_memory[i]+1;
//      printf("%c",file_in_memory[i]);
//    }

   s = write(STDOUT_FILENO, map + offset - pa_offset, length);

   if (s != length) {
       if (s == -1)
           handle_error("write");
       fprintf(stderr, "partial write");
       exit(EXIT_FAILURE);
   }

   printf("\n -------------\n");
   printf("printed success \n");
   munmap(map, length + offset - pa_offset);
   close(fd);
   exit(EXIT_SUCCESS);
} 



