#ifndef _PAGE_TABLE_H
#define _PAGE_TABLE_H

int getPTEindex(uint64_t vadd);
int getPDEindex(uint64_t vadd);
int getPDPEindex(uint64_t vadd);
int getPML4Eindex(uint64_t vadd);
uint64_t set_paging(void * km, void * pf, void * pb);
void page_mapping(uint64_t vadd);
//void set_virtual_video_memory(void* km);
#endif
