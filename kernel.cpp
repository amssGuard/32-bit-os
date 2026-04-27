void printf(char* str){
    unsigned short* videoMemory = (unsigned short*)0xb8000;
    for(int i=0;str[i]!='\0';i++){
        videoMemory[i]=( 0x0F << 8) | str[i];
    }
}

typedef void (*constructor)();
extern "C" constructor* start_ctors;
extern "C" constructor* end_ctors;

extern "C" void callConstructors(){
    for(auto ctor=start_ctors;ctor!=end_ctors;ctor++){
        (*ctor)();
    }
}

extern "C" void kernelMain(void* multiboot_structure,unsigned int magicnumber){
    printf("Hello World! --- https://wiki.osdev.org/");
    while(1);
}
