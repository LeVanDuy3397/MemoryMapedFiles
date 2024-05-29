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


int main (int argc, char *argv[]) 
{
  int in_fd, out_fd;
  //mapid_t in_map, out_map;
  const char * FileIn="/home/duy/MemoryMapedFiles/FileIn.txt";
  const char * FileOut="/home/duy/MemoryMapedFiles/FileOut.txt";
  void *in_data = (void *) 0x10000000;
  void *out_data = (void *) 0x20000000;
  int size;

  /* Open input file. */
  in_fd = open(FileIn, O_RDWR, S_IRUSR | S_IWUSR); 
    if (in_fd == -1) {
        printf("open file in failed \n");
        perror("open");
        return EXIT_FAILURE;   
    }

  struct stat st;

  if(fstat(in_fd,&st)==-1){ 
      perror("stat");
      return EXIT_FAILURE;
    }
  size_t file_size = st.st_size;
  
  printf("file size is %ld : da dua vao bo dem \n",file_size);

  /* Create and open output file. */
    out_fd = open(FileOut, O_RDWR, S_IRUSR | S_IWUSR); 
    if (out_fd == -1) {
        printf("open file out failed \n");
        perror("open");
        return EXIT_FAILURE;   
    }
  /* Map files. */
  void *in_map = mmap(in_data, size, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, 0);
  if (in_map == MAP_FAILED) 
    {
      printf ("%s: mmap in_map failed\n", FileIn);
      return EXIT_FAILURE;
    }
  void *out_map = mmap(out_data, size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);
  if (out_map == MAP_FAILED)
    {
      printf ("%s: mmap out_map failed\n", FileOut);
      return EXIT_FAILURE;
    }

  /* Copy files. */
  memcpy (out_data, in_data, size);

  /* Unmap files (optional). */
  munmap (in_map, size);
  munmap (out_map, size);
  
  return EXIT_SUCCESS;
}

