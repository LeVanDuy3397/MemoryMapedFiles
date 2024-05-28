#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

// int main (int argc, char *argv[]) 
// {
//   void *data = (void *) 0x10000000;     /* Address at which to map. */

//   int fd = open (argv[1]);              /* Open file. */
//   mmap (fd, data);        /* Map file. */
//   write (1, data, filesize (fd));       /* Write file to console. */
//   munmap (mmap (fd, data));                         /* Unmap file (optional). */
//   return 0;
// }

int main(int argc, char**argv){
  int fd=open("/home/duy/sometext.txt", O_RDWR, S_IRUSR | S_IWUSR);
  struct  stat sb;
  if(fstat(fd,&sb)==-1){
    perror ("Error");
  }
  printf("file size %ld\n", sb.st_size);
  char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("Printting...\n");
  for(int i=0; i < sb.st_size; i++ ){
    file_in_memory[i]=toupper(file_in_memory[i]);
    printf("%c",file_in_memory[i]);
  }
  printf("\n");
  munmap(file_in_memory, sb.st_size);
  close(fd);
}
