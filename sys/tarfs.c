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

void read_tarfs(){
 struct posix_header_ustar *header = (struct posix_header_ustar*)(&_binary_tarfs_start+sizeof(struct posix_header_ustar));
 char zero_size[12] = "000000000000";
 while(header->size == zero_size)
 {
   printf("name = %s\n",header->name);
/* printf("name = %s\n",header->name);
 printf("size = %s\n",header->size);
 printf("mode = %s\n",header->mode);
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
 }

}

