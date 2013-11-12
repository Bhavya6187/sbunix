#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/irq.h>
#include <sys/phy_mem.h>
#include <sys/page_table.h>
#include <sys/tasking.h>
#include <sys/tarfs.h>

char zero_size[12] = "000000000000";
uint64_t size_to_int(char* size)
{
  uint64_t ret = 0;
  uint64_t base = 1;
  int i =11;
  while(size[i] <= '0' || size[i] > '9'  )
    i--;
  for(i=i;  i >= 0; i--)
  {
    ret += base*(size[i] - '0');
    base*=8;
  }
  return ret;
}
 
void read_tarfs(){
 struct posix_header_ustar *header;
 char* end_pos =(char*) (&_binary_tarfs_end);
 
 printf("\n size in tarfs= %ld  for %s\n ", size_to_int(header->size),header->size);

 uint64_t temp_size = size_to_int(header->size) ;
 char* new_pos = (char*)(&_binary_tarfs_start);
 new_pos+=512;
 new_pos+=512;
 header =  (struct posix_header_ustar*)new_pos;

// while(header < (struct posix_header_ustar*)(&_binary_tarfs_end))
 while(new_pos< end_pos)
 {
    printf("name = %s\n",header->name);
    printf("size = %s\n",header->size);
    /*printf("mode = %s\n",header->mode);
    printf("uid = %s\n",header->uid);
    printf("gid = %s\n",header->gid);
    printf("mtime = %s\n",header->mtime);
    printf("checksum = %s\n",header->checksum);
    printf("typeflag = %s\n",header->typeflag);
    printf("prefix = %s\n",header->prefix);
    printf("linkname = %s\n",header->linkname);
    printf("magic = %s\n",header->magic);
    printf("version = %s\n",header->version);
    printf("uname = %s\n",header->uname);
    printf("gname = %s\n",header->gname);
    printf("devmajor = %s\n",header->devmajor);
    printf("devminor = %s\n",header->devminor);
    printf("pad = %s\n",header->pad);*/
    temp_size = size_to_int(header->size) ;
    printf("size given by function = %ld\n",temp_size);
    new_pos = new_pos + temp_size + sizeof(struct posix_header_ustar);
    printf("header = %p, new_pos = %p, size of struct = %d\n",header, new_pos, sizeof(struct posix_header_ustar));
    header =  (struct posix_header_ustar*)new_pos;
 }
}

