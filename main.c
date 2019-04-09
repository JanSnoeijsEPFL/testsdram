#include "testsdram.h"
#include "parser.h"
#include <unistd.h>
#include "hwlib.h"
#include <stdlib.h>
#include <stdio.h>
//#include "globals.h"
#include <string.h>

int main() {
    printf("FPGA sdram test\n");
   // printf("check if recompiled\n");
    open_physical_memory_device();
    printf("Physical memory device opened\n");
    mmap_peripherals();
    printf("mapped the peripherals\n");
    uint32_t i = 0;
    //int address = 0;
   // for (i=0; i<50; i++){
   // 	read_sram_fpga(address);
   // 	address++;
    //	usleep(ALT_MICROSECS_IN_A_SEC / 100);
    //}

    //address = 0;
    //for (i=0; i<50; i++){
    //	write_sram_fpga(address, i);
    //	address++;
    //	usleep(ALT_MICROSECS_IN_A_SEC / 100);
    //}

   // address = 0;
   // for (i=0; i<50; i++){
    //	read_sram_fpga(address);
   // 	address++;
   // 	usleep(ALT_MICROSECS_IN_A_SEC / 100);
   // }

    int32_t* words = calloc(NBWORDS, sizeof(int32_t));
    parse_weights("keras_param_3class_30e_5bits_onlysign_modifiedHardSigm.txt", &words);
    //concat_words(&words, words);
    //for (i = 0; i < NBWORDS; i++)
	//{
		//printf("words: 0x%x\n", words[i]);
	//}

    uint32_t ocram[20];
    for (i= 0; i <20; i++)
    {
    	ocram[i]=0;
    }
    uint32_t* uocram = get_uocram_base();
   // uint32_t* wocram = get_wocram_base();
  ///  uint32_t* xocram = get_xocram_base();
    //memcpy((void *)uocram, (void*)words , sizeof(uint32_t));
    for (i=0; i<307; i++){
    	//printf("when writing::: uocram+(i<<5): %x, words +i*20 : %x\n", *(uocram+(i<<5)), *(words + i*20));
    	memcpy((void*)(uocram+(i<<5)), (void*)(words+i*20), 20*sizeof(uint32_t));
    	printf("uocram+(i<<5)::  0x%x -- 0x%x\n",uocram+(i<<5),words+i*20);
    	printf("when writing::: uocram+(i<<5): %x, words +i*20 : %x\n", *(uocram+(i<<5)), *(words + i*20));
    }
    // cant use memcpy because physical addresses dont map exactly to virtual addresses.
  //  printf("written to SDRAM\n");
   //8 uint32_t address = 0;
    uint32_t data = 0;
    for (i = 0; i < 100; i++)
	{
    	data = *(uint32_t*)(words+i);
    	printf("data to be written to uocram 0x%x\n", data);
    	//check the first 10 words
    	data = *(uint32_t*)(uocram+i);
    	printf("WRONG data read back from uocram 0x%x\n", data);
    	data = *(uint32_t*)(uocram+ i % 20 + (uint32_t)((i/20)<<5));
    	printf("(uint32_t)((i/20)<<5):::  %d\n", (uint32_t)((i/20)<<5));
    	printf("CORRECT data read back from uocram 0x%x\n", data);
    	usleep(ALT_MICROSECS_IN_A_SEC / 100);
	}

	//}
    free(words);
   // printf("writing to accelerator\n");
    //read_accelerator(address+4);
   // read_accelerator(address+1);
	//write_accelerator(address);
	//read_accelerator(address+4);
	//read_accelerator(address+1);
	//printf("observe signal tap\n");
	//usleep(ALT_MICROSECS_IN_A_SEC / 10);


    munmap_peripherals();
    close_physical_memory_device();
    return 0;
}
