#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ext2_fs.h"

int fd;

int read_blocks(void* buf, int count, unsigned long blockNum) {
  int retVal = pread(fd, buf, count*1024, blockNum*1024);
  return retVal;
}

int main(int argc, char **argv)
{
  if(argc < 2) {
    fprintf(stderr, "file system image not specified");
    exit(1);
  }
  if(argc > 2) {
    fprintf(stderr, "too many arguments");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  if(fd == -1) {
    fprintf(stderr, "fail to open file system image");
    exit(1);
  }
  
  __u32 block_count;
  pread(fd, &block_count, 4, 1028);
  __u32 inode_count;
  pread(fd, &inode_count, 4, 1024);
  __u32 log_block_size;
  __u32 block_size = 1024;
  pread(fd, &log_block_size, 4, 1048);
  for(size_t i = 0; i < log_block_size; i++) {
    block_size = block_size << 1;
  }
  __u16 inode_size;
  pread(fd, &inode_size, 2, 1112);
  __u32 blocks_per_group;
  pread(fd, &blocks_per_group, 4, 1056);
  __u32 inodes_per_group;
  pread(fd, &inodes_per_group, 4, 1064);
  __u32 first_ino;
  pread(fd, &first_ino, 4, 1108);
  fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%hu,%u,%u,%u\n", block_count, inode_count, block_size, inode_size, blocks_per_group, inodes_per_group, first_ino);
  exit(0);
}
