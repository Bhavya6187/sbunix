#ifndef _PAGE_TABLE_H
#define _PAGE_TABLE_H

uint64_t set_paging(void * km, void * pf, void * pb);
void page_mapping(uint64_t vadd);
int getPDPEindex(uint64_t vadd);
//void set_virtual_video_memory(void* km);

#endif
