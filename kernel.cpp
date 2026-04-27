void printf(char* str){
    unsigned short* videoMemory = (unsigned short*)0xb8000;
    for(int i=0;str[i]!='\0';i++){
        videoMemory[i]=( 0x0F << 8) | str[i];
    }
}

extern "C" void kernelMain(void* multiboot_structure,unsigned int magicnumber){
    printf("Hello World! --- https://wiki.osdev.org/");
    while(1);
}
