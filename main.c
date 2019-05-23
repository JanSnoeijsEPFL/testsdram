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

    uint32_t n = 0;
    uint32_t* uocram = get_uocram_base();
    uint32_t* wocram = get_wocram_base();
    uint32_t* xocram = get_xocram_base();
    uint32_t* av_slave = get_fpga_accelerator_base();
	write_accelerator(0, 0);

    int32_t* words = malloc(NBWORDS * sizeof(int32_t));
   // int32_t xdata[RT_DATA_CHUNK_SIZE];

    //printf("%p\n",&words);
    //if (words == NULL)
    //	return EXIT_FAILURE;
    parse_weights("FINAL_signed_6b.txt", words);
    ocram_init(uocram, wocram, xocram);
    rearrange_conv2d_param(words, words+1);
    load_param(av_slave, uocram, wocram, words);
    printf("no problem before freeing memory\n");
    free(words);
    printf("no problem while freeing memory\n");
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

    int32_t DEBUG_data_words[540];
	int32_t DEBUG_data_maxp[1078];
	int32_t DEBUG_data_gru[500];
    uint8_t timesteps = 0;
    uint8_t hps_DEBUG_read = 0;
    uint8_t hps_write_new_batch = 0;
	FILE* res_file;
	char filename[20];
	char prt_step;
	char seq_nb[3];
	printf("no problem before new memory allocation\n");
	int32_t* xdata = malloc(RTDATA_CHUNK_SIZE * sizeof(int32_t));
	printf("no problem while allocating new memory\n");

	//FIRST algorithm iteration

	parse_rtdata("RT_NormQuantdata_seq0", xdata, 0);
	printf("no problem parsing RT_datastream file memory\n");
	xocram_fill_RT(xocram, xdata);
	//free(xdata);

    for (timesteps=0; timesteps < 10; timesteps++){
    	prt_step = timesteps +'0';
    	printf("iteration number %d\n", timesteps);

		write_accelerator(0, 3); // xocram B port in FPGA mode + trigger accelerator
		write_accelerator(0, 2); //  deassert trigger
		//xdata = calloc(RT_DATA_CHUNK_SIZE, sizeof(int32_t));
		//if (xdata == NULL)
		//	return EXIT_FAILURE;
		//

		//while(hps_write_new_batch == 0){
		//	hps_write_new_batch = 1 & read_accelerator(1);
		//}

		//free(xdata);
		while(hps_DEBUG_read == 0){
			hps_DEBUG_read =  read_accelerator(1) >> 1;
		}
		hps_DEBUG_read = 0;
		write_accelerator(0, 0); //switch back to HPS mode
	  //  DEBUG_data_words = calloc(26*20, sizeof(int32_t));
	   // if (DEBUG_data_words == NULL)
	   // 	return EXIT_FAILURE;
		read_xocram(1, xocram, DEBUG_data_words);
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
		snprintf(filename, sizeof(filename), "res_acc/MAXP_t%c.txt", prt_step);
		res_file = fopen(filename, "w");
		printf("%s\n", filename);
		if (!res_file)
			printf("file never opened\n");
		else{
			printf("opened resfile\n");
			for (n=0; n<1078; n++){
				fprintf(res_file, "%f\n",((float)*(DEBUG_data_maxp+n))/16);
			}
		}
		fclose(res_file);
		snprintf(filename,sizeof(filename), "res_acc/GRU_t%c.txt", prt_step);
		res_file = fopen(filename, "w");
		if (!res_file)
				printf("file never opened\n");
			else{
				printf("opened resfile\n");
				for (n=0; n<400; n++){
					fprintf(res_file, "%f\n",((float)*(DEBUG_data_gru+n))/16);
			}
		}
		fclose(res_file);
		snprintf(filename,sizeof(filename), "res_acc/SR_t%c.txt", prt_step);
		res_file = fopen(filename, "w");
		if (!res_file)
				printf("file never opened\n");
			else{
				printf("opened resfile\n");
				for (n=400; n<500; n++){
					fprintf(res_file, "%f\n",((float)*(DEBUG_data_gru+n))/16);
			}
		}
		fclose(res_file);
		//snprintf(filename,sizeof(filename), "RT_NormQuantdata_seq%c.txt", seq_nb);
		parse_rtdata("RT_NormQuantdata_seq0", xdata, timesteps+1);
		if (timesteps != 9)
			xocram_fill_RT(xocram, xdata);
		else{
			write_accelerator(0,6);
			uint32_t y0 = read_accelerator(6);
			write_accelerator(1,6);
			uint32_t y1 = read_accelerator(6);
			write_accelerator(2,6);
			uint32_t y2 = read_accelerator(6);
			res_file = fopen("res_acc/Y.txt", "w");
			if (!res_file)
				printf("file never opened\n");
			else{
				printf("opened resfile\n");
				fprintf(res_file, "%f\n",((float)(y0))/2048);
				fprintf(res_file, "%f\n",((float)(y1))/2048);
				fprintf(res_file, "%f\n",((float)(y2))/2048);

			}

		}
    }
    free(xdata);
	//free(xdata);
	//free(DEBUG_data_gru);
	//free(DEBUG_data_maxp);
	//free(DEBUG_data_words);
    munmap_peripherals();
    close_physical_memory_device();
    return 0;
}
