#include"types.h"

void printf(char* str){
    static uint16_t* videoMemory = (uint16_t*)0xb8000;
    for(int i=0;str[i]!='\0';i++){
        videoMemory[i]=( 0x0F << 8) | str[i];
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void* multiboot_structure,uint32_t magicnumber){
    printf("Hello World! --- https://wiki.osdev.org/");
    while(1);
}
