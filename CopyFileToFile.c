/* mcp.c

   Copies one file to another, using mmap. */
   
#include <string.h>
#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <locale.h>

int main (int argc, char *argv[]) 
{
  int in_fd, out_fd;

  const char * FileIn="/home/duy/MemoryMapedFiles/FileIn.txt";
  const char * FileOut="/home/duy/MemoryMapedFiles/FileOut.txt";

  void *in_data = (void *) 0x10000000;
  void *out_data = (void *) 0x20000000;

  /* Open input file. */
  in_fd = open(FileIn, O_RDWR, S_IRUSR | S_IWUSR); 
    if (in_fd == -1) {
        perror("open");
        return EXIT_FAILURE;   
    }

  struct stat st;

  if(fstat(in_fd,&st)==-1){ 
      perror("stat");
      return EXIT_FAILURE;
    }

  size_t file_size = st.st_size; /*lay kich thuoc cua file_in, han che la neu file_in co kich thuoc
  lon hon kich thuoc trang se loi*/
  
  printf("file size is %ld : in buffer \n",file_size);

  /* Create and open output file. */
    out_fd = open(FileOut, O_RDWR, S_IRUSR | S_IWUSR); 
    if (out_fd == -1) {
        perror("open");
        return EXIT_FAILURE;   
    }
  /* Map files. */
  void *in_map = mmap(in_data, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, 0);/*0 la vi tri bat dau de doc trong file ngoai,
  sau khi doc duoc den vi tri bang file_size->dich chuyen den vi tri tiep theo trong file de doc tiep*/
  if (in_map == MAP_FAILED) 
    {
      printf ("%s: mmap in_map failed\n", FileIn);
      return EXIT_FAILURE;
    }

  //write(1, in_map, file_size);
  printf("\n");
  
  void *out_map = mmap(out_data, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);
  if (out_map == MAP_FAILED)
    {
      printf ("%s: mmap out_map failed\n", FileOut);
      return EXIT_FAILURE;
    }
  write(1, out_map, file_size);
  printf("\n");

  /* Copy files. */
  memcpy (out_map, in_map, file_size);

  /* Unmap files (optional). */
  munmap (in_map, file_size);
  munmap (out_map, file_size);

  return EXIT_SUCCESS;
}




