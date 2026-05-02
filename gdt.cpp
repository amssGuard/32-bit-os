#include "gdt.h"

struct GDTPointer{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

GlobalDescriptorTable::GlobalDescriptorTable():nullSegmentSelector(0,0,0),
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024,0x9A),
dataSegmentSelector(0,64*1024*1024,0x92) {

    GDTPointer gdtp;
    gdtp.limit = sizeof(GlobalDescriptorTable) - 1;
    gdtp.base = (uint32_t) this;

    asm volatile("lgdt %0": :"m"(gdtp));
}

GlobalDescriptorTable::~GlobalDescriptorTable(){}

uint16_t GlobalDescriptorTable::DataSegmentSelector(){
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector(){
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type){
    uint8_t* target = (uint8_t*) this;

    if(limit<=65536){
        target[6] = 0x40; //64 KB
    }else{
        if((limit & 0xFFF) != 0xFFF) //100000000000000000000000000 &  111111111111
            limit = (limit >> 12) - 1; // 000000000000100000000000000 - 1 = 0011-1111-1111-1111
        else
            limit = limit >> 12;
        target[6] = 0xC0; //192 KB

    }
    target[0] = limit & 0xFF; // 011-1111-1111-1111 & 1111-1111 = 1111-1111
    target[1] = (limit >> 8) & 0xFF; // 011-1111-1111-1111 >> 8 = 0011-1111 & 1111-1111 = 0011-1111
    target[6] |= (limit >> 16) & 0xF; // 011-1111-1111-1111 >> 16 = 0000-0000-0000-0000 | 1100-0000 & 1111 = 1100-0000

    target[2] = base & 0xFF; // 0000-0000 & 1111-1111 = 0000-0000
    target[3] = (base >> 8) & 0xFF; // 0000-0000 & 1111-1111 = 0000-0000
    target[4] = (base >> 16) & 0xFF; //0000-0000 & 1111-1111 = 0000-0000
    target[7] = (base >> 24) & 0xFF;//0000-0000 & 1111-1111 = 0000-0000

    target[5] = type;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base(){
    uint8_t* target = (uint8_t*) this;
    uint32_t result = target[7];//0000-0000
    result = (result << 8) + target[4];//0000-0000
    result = (result << 8) + target[3];//0000-0000
    result = (result << 8) + target[2];//0000-0000
    return result;// 0000-0000-0000-0000-0000-0000-0000-0000
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit(){
    uint8_t* target = (uint8_t*) this;
    uint32_t result = target[6] & 0xF;// 1100-0000 & 0000-1111 = 0000-0000
    result = (result << 8) + target[1];//0000-0000 << 8 = 0000-0000-0000-0000 + 0011-1111 = 0000-0000-0011-1111
    result = (result << 8) + target[0];// 0000-0000-0011-1111 << 8 = 0011-1111-0000-0000 + 1111-1111 =  0011-1111-1111-1111

    if((target[6]&0xC0)==0xC0)//1100-0000 & 1100-0000
        result = (result<<12) | 0xFFF; //0011-1111-1111-1111-0000-0000-0000 | 1111-1111-1111
    return result;//11-1111-1111-1111-1111-1111-1111
}
