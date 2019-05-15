#include "testsdram.h"
#include "parser.h"
#include <unistd.h>
#include "hwlib.h"
#include <stdlib.h>
#include <stdio.h>
//#include "globals.h"
#include <string.h>
#include "transfer_data.h"

#define RT_DATA_CHUNK_SIZE 460 //23*20
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

   // int32_t words[NBWORDS];
    int32_t xdata[RT_DATA_CHUNK_SIZE];
    int32_t DEBUG_data_words[2600];
    int32_t DEBUG_data_maxp[1078];
    int32_t DEBUG_data_gru[400];
    //printf("%p\n",&words);
    //if (words == NULL)
    //	return EXIT_FAILURE;
   // parse_weights("FINAL_signed_6b.txt", words);
    ocram_init(uocram, wocram, xocram);
  //  rearrange_conv2d_param(words, words+1);
   // load_param(av_slave, uocram, wocram, (uint32_t*) words);
   // free(words);

    //int32_t* xdata = NULL;
   // int32_t* DEBUG_data_words = NULL;
   // int32_t* DEBUG_data_maxp = NULL;
    //int32_t* DEBUG_data_gru = NULL;

    //xdata = calloc(RT_DATA_CHUNK_SIZE, sizeof(int32_t));
    //if (xdata == NULL)
    //	return EXIT_FAILURE;

   // DEBUG_data_words = calloc(26*20, sizeof(int32_t));
   // if (DEBUG_data_words == NULL)
	//	return EXIT_FAILURE;

	// DEBUG_data_maxp = calloc(1078, sizeof(int32_t));
	//if (DEBUG_data_words == NULL)
	//	return EXIT_FAILURE;

	// DEBUG_data_gru = calloc(400, sizeof(int32_t));
	//if (DEBUG_data_gru == NULL)
	//	return EXIT_FAILURE;


    uint8_t timesteps = 0;
    uint8_t hps_DEBUG_read = 0;
    uint8_t hps_write_new_batch = 0;
	FILE* res_file;
	char filename[20];

	//FIRST algorithm iteration
	//parse_rtdata("RT_datastream.txt", xdata, 0);
	//xocram_fill_RT(xocram, xdata);
	//free(xdata);
/*
    for (timesteps=0; timesteps < 10; i++){
    	printf("iteration number %d\n", i);

		write_accelerator(0, 3); // xocram B port in FPGA mode + trigger accelerator
		write_accelerator(0, 2); //  deassert trigger
		//xdata = calloc(RT_DATA_CHUNK_SIZE, sizeof(int32_t));
		//if (xdata == NULL)
		//	return EXIT_FAILURE;
		parse_rtdata("RT_datastream.txt", xdata, i+1);

		while(hps_write_new_batch == 0){
			hps_write_new_batch = 1 & read_accelerator(1);
		}
		if (timesteps != 9)
		{

			write_accelerator(0, 0); //switch back to HPS mode
			xocram_fill_RT(xocram, xdata);
			write_accelerator(0, 2); // allow accelerator to write to XOCRAM B port
		}
		//free(xdata);
		while(hps_DEBUG_read == 0){
			hps_DEBUG_read = (2 & read_accelerator(1) >> 1);
		}

		write_accelerator(0, 0); //switch back to HPS mode
	  //  DEBUG_data_words = calloc(26*20, sizeof(int32_t));
	   // if (DEBUG_data_words == NULL)
	   // 	return EXIT_FAILURE;
		read_xocram(1, (int32_t*)xocram, DEBUG_data_words);
		 ///DEBUG_data_maxp = calloc(1078, sizeof(int32_t));
		//if (DEBUG_data_maxp == NULL)
		//	return EXIT_FAILURE;
		get_data_maxp(DEBUG_data_maxp, DEBUG_data_words);
		//free(DEBUG_data_maxp);
		//DEBUG_data_gru = calloc(400, sizeof(int32_t));
		//if (DEBUG_data_gru == NULL)
		//	return EXIT_FAILURE;
		get_data_gru(DEBUG_data_gru, DEBUG_data_words+20*22);
		//free(DEBUG_data_gru);
		//free(DEBUG_data_words);
		snprintf(filename, "res_acc/MAXP_t%c.txt", (char) timesteps);
		res_file = fopen(filename, "w");
		if (!res_file)
			printf("file never opened\n");
		else{
			printf("opened resfile\n");
			j=0;
			for (i=0; i<1078; i++){
				j ++;
				fprintf(res_file, "%f\n",((float)*(DEBUG_data_maxp+i))/16);
			}
		}
		fclose(res_file);
		snprintf(filename,"res_acc/MAXP_t%c.txt", (char) timesteps);
		res_file = fopen(filename, "w");
		if (!res_file)
				printf("file never opened\n");
			else{
				printf("opened resfile\n");
				j=0;
				for (i=0; i<400; i++){
					fprintf(res_file, "%f\n",((float)*(DEBUG_data_gru+i))/16);
			}
		}
		fclose(res_file);
    }*/
	//free(xdata);
	//free(DEBUG_data_gru);
	//free(DEBUG_data_maxp);
	//free(DEBUG_data_words);
    munmap_peripherals();
    close_physical_memory_device();
    return 0;
}
