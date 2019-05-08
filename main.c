#include "testsdram.h"
#include "parser.h"
#include <unistd.h>
#include "hwlib.h"
#include <stdlib.h>
#include <stdio.h>
//#include "globals.h"
#include <string.h>
#include "transfer_data.h"

#define RT_DATA_CHUNK_SIZE 23*20
int main() {
    printf("FPGA sdram test\n");
   // printf("check if recompiled\n");
    open_physical_memory_device();
    printf("Physical memory device opened\n");
    mmap_peripherals();
    printf("mapped the peripherals\n");
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t* uocram = get_uocram_base();
    uint32_t* wocram = get_wocram_base();
    uint32_t* xocram = get_xocram_base();
    uint32_t* av_slave = get_fpga_accelerator_base();

   // int32_t* words = calloc(NBWORDS, sizeof(int32_t));
    //parse_weights("FINAL_signed_6b.txt", &words);
   // ocram_init(uocram, wocram, xocram);
    //rearrange_conv2d_param(words, words+1);
    //load_param(av_slave, uocram, wocram, (uint32_t*) words);

  // concat_words(&words, words);
    //for (i = 0; i < NBWORDS; i++)
	//{
		//printf("words: 0x%x\n", words[i]);
	//}

   // uint32_t ocram[20];
    //for (i= 0; i <20; i++)
    //{
    //	ocram[i]=0;
    //}

   // free(words);
   // int32_t* xdata = calloc(RT_DATA_CHUNK_SIZE, sizeof(int32_t));
   // parse_rtdata("RT_datastream.txt", &xdata, 0);
    //set xocram B port mode to HPS (0)
    //write_accelerator(0, 0);
   // xocram_fill_RT(xocram, xdata);
   // free(xdata);
   // read_xocram(xocram);
    //read_uocram(uocram);
  //  write_accelerator(0, 3); // xocram B port in FPGA mode + trigger accelerator
    //usleep(ALT_MICROSECS_IN_A_SEC);
  //  read_accelerator(1);
  //  write_accelerator(0, 0); //switch back to HPS mode
  //  read_xocram(xocram);
    //xocram_read_Conv2D(xocram, 20*44);
    //printf("writing to accelerator\n");4820802816
    //while(1)
    //{
		//read_accelerator(address+4);
		//usleep(ALT_MICROSECS_IN_A_SEC*1);
		//printf("writing to accelerator\n");
		//read_accelerator(address+1);
		//usleep(ALT_MICROSECS_IN_A_SEC*1);
		//printf("writing to accelerator\n");
		//write_accelerator(address);
		//usleep(ALT_MICROSECS_IN_A_SEC*1);
		//printf("writing to accelerator\n");
		//read_accelerator(address+4);
		//usleep(ALT_MICROSECS_IN_A_SEC*1);
		//printf("writing to accelerator\n");
		//read_accelerator(address+1);
		//printf("observe signal tap\n");
		//usleep(ALT_MICROSECS_IN_A_SEC * 1);
    //}


    munmap_peripherals();
    close_physical_memory_device();
    return 0;
}
