#include "parser.h"
#include "testsdram.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "hwlib.h"
#include <unistd.h>
//#include "globals.h"

void parse_weights(char* file, int32_t* words){
	//free(words);
	//words = calloc(NBWORDS/1000, sizeof(int32_t));
	printf("Starting parser\n");
	//int32_t * word = calloc(NBWORDS, sizeof(int32_t));

	int8_t param[NBPARAM_IN_WORD];
	//int32_t nbw = 0;
	printf("file %s \n", file);
	weights_file = fopen(file, "r");
	if (!weights_file)
		printf("file never opened\n");
	else
		printf("opened weights file\n");
	char STR[NBCHAR];
	char CH;
	uint8_t k = 0,j,i;
	uint32_t word_cnt = 0;
	do
	{
		CH = fgetc(weights_file);
		if(feof(weights_file))
		{
			printf("already finished reading file?\n");
			*(words+NBWORDS-1) = params2word(param);
			printf("after concat: 0x%x\n", *(words+NBWORDS-1));
			break;
		}
		if (CH != '0' && CH != '1' && CH != '2' && CH != '3' && CH != '4' \
				&& CH != '5' && CH != '6' && CH != '7' && CH != '8' && CH != '9' \
				&& CH != '-' && CH != '+' && CH != 'e' && CH != '\n' && CH != ',' && CH != '.'){
			printf("invalid character\n");
			continue;
		}
		else
		{
			if (!(CH==',' || CH=='\n'))
			{
				STR[k]=CH;
				k++;
			}
			else if (k!=0)
			{
				//printf("OKOK\n");
				param[j]=process_string((char*)STR, (uint8_t)NBCHAR);
				printf("params before concat: %d \n", param[j]);
				//printf("k : %d, j : %d \n", k, j);
				if (j == 4)
				{
					j = 0;
					*(words+word_cnt) = params2word(param);
					for (i = 0; i < NBPARAM_IN_WORD; i++)
						param[i]=0;

					printf("after concatenate: 0x%x\n", *(words+word_cnt));
					printf("PARAMWORD NUMBER %d \n ", word_cnt);
					word_cnt ++;
				}
				else
					j++;
				k = 0;
			}

		}

		//usleep(ALT_MICROSECS_IN_A_SEC/100);
	}while(1);
	fclose(weights_file);

}

void parse_rtdata(char* file, int32_t* words, int32_t chunk_number){
	//free(words);
	//words = calloc(RTDATA_CHUNK_SIZE, sizeof(int32_t));
	printf("starting RT data parser\n");
	//int32_t * word = calloc(RTDATA_CHUNK_SIZE, sizeof(int32_t));
	if (words != NULL)
	{
		rtdata_file = fopen(file, "r");
		int8_t in_data[NBPARAM_IN_WORD];
		uint8_t i, j, k=0;
		uint32_t word_cnt = 0;
		char STR[NBDIGIT_RTDATA];
		char CH;
		if (!rtdata_file)
			printf("file never opened\n");
		else
			printf("opened xData file\n");
		do
		{
			CH = fgetc(rtdata_file);
			if(feof(rtdata_file))
			{
				printf("already finished reading file?\n");
				*(words+RTDATA_CHUNK_SIZE-1) = params2word(in_data);
				printf("after concat: 0x%x\n", *(words+RTDATA_CHUNK_SIZE-1));
				break;
			}
			if (CH != '0' && CH != '1' && CH != '2' && CH != '3' && CH != '4' \
					&& CH != '5' && CH != '6' && CH != '7' && CH != '8' && CH != '9' \
					&& CH != '-' && CH != '\n' && CH != ',' && CH != '.'){
				printf("invalid character\n");
				continue;
			}
			else
			{
				if (!(CH==',' || CH=='\n'))
				{
					STR[k]=CH;
					k++;
				}
				else if (k!=0){
					if (word_cnt >= RTDATA_CHUNK_SIZE*chunk_number)
					{
						in_data[j]=quantize_param((char*)STR, (uint8_t)NBDIGIT_RTDATA);
						printf("params before concat: %d data input number %d\n", in_data[j], word_cnt);
					}

					if (j == 4)
					{
						j = 0;
						if (word_cnt >= RTDATA_CHUNK_SIZE*chunk_number){
							*(words+word_cnt-RTDATA_CHUNK_SIZE*chunk_number) = params2word(in_data);
							for (i = 0; i < NBPARAM_IN_WORD; i++)
								in_data[i]=0;
							printf("after concatenate: 0x%x\n", *(words+word_cnt-RTDATA_CHUNK_SIZE*chunk_number));
							//usleep(ALT_MICROSECS_IN_A_SEC/10);
						}
						word_cnt ++;
					}

					else
						j++;
					k = 0;
				}

			}

		}while(word_cnt < RTDATA_CHUNK_SIZE*chunk_number+RTDATA_CHUNK_SIZE);
		fclose(rtdata_file);
	}
	//printf("check if program crashed \n");

}

int8_t process_string(char* STR, uint8_t size){
	//from format "-6.250000000000000000e-02,...." to "-00.0001" => "11.1111"
	char digit;
	char mantissa[4];
	int8_t number=0;
	uint8_t expt;
	uint16_t mant;
	uint8_t i = 0;
	// extract sign bit
	uint8_t sign = 0;
	if (STR[0] == '-')
	{
		sign =1;
		strcpy(&STR[0], &STR[1]);
	}
	strcpy(&STR[1], &STR[2]); //remove dot
	// extract mantissa
	while (i < 4)
	{
		digit = STR[i];
		mantissa[i] = digit;
		i++;
	}

	mant = (uint16_t) strtol(mantissa, (char **)NULL, 10);
	mant = mant * pow(2,4)/1000;
	// extract exponent
	expt = (uint8_t) strtol(&STR[size-2], (char **)NULL, 10);
	if (expt !=0)
		mant /= pow(10, expt);

	if (sign == 1)
		number = -1*mant;
	else
		number = mant;
	//printf("Number is %d/16\n", number);
	return number;
}

int8_t quantize_param(char* STR, uint8_t size){
	char digit;
	//char mantissa[4];
	float number=0;
	int8_t quantized_nb = 0;
	//uint8_t expt;
	//uint16_t mant;
	uint8_t i = 0;
	// extract sign bit
	uint8_t sign = 0;
	if (STR[0] == '-')
	{
		sign =1;
		strcpy(&STR[0], &STR[1]);
	}
	//strcpy(&STR[1], &STR[2]); //remove dot
	// extract mantissa
	//printf("%s\n ", STR);
	number = strtof(STR, (char**)NULL);
	//printf("%f\n", number);
	number /= MAX_XDATA;
	//printf("%f\n", number);
	if (sign == 1)
		number = -number;
	if (number >= 1.9375)
		number = 1.9375;
	else if (number <= -2)
		number = -2;
	else
		number = number * 16;
	//printf("multiplied by 16 : %f\n", number);
	quantized_nb = (int8_t)round(number);
	//printf("%d\n and rint() output: %f\n", quantized_nb, round(number));

	//usleep(ALT_MICROSECS_IN_A_SEC/10);

	return quantized_nb;
}
int32_t params2word(int8_t param[NBPARAM_IN_WORD]){
	int32_t word = 0;
	uint8_t i;
	uint8_t MASK = 0b00111111;
	for (i = 0; i <NBPARAM_IN_WORD; i++)
	{
		word = word | ((param[i]&MASK)<<(i*NBITS));
		//printf("0x%x\n", param[i]);
	}
	return word;
}

/*void concat_words(int32_t** words, int32_t paddedw[NBWORDS]){
	int32_t * word = calloc(NBWORDS, sizeof(int32_t));
	uint8_t i, j=0;
	uint32_t k = 0;
	uint32_t maskMSB0 = 0x00000003;
	uint32_t maskMSB = 0x00000003;
	for (k=0; k < 3541; k++){
		for (i=0; i<19; i++){
			//printf("i : %d\n", i);
			if(i <15){
				maskMSB = maskMSB | (maskMSB0 << 2*i);
				*(word+i+19*k) = ((uint32_t)paddedw[i+20*k]>>(2*i)) | (((uint32_t)paddedw[i+1+20*k] & maskMSB)<<(30-2*i));

			}
			if (i == 15)
				maskMSB = maskMSB0;
			if (i >= 15){
				maskMSB = maskMSB | (maskMSB0 << 2*j);
				if (i!=18)
					*(word+i+19*k) = ((uint32_t)paddedw[i+1+20*k]>>(2*j)) | (((uint32_t)paddedw[i+2+20*k] & maskMSB)<<(30-2*j));

				else
					*(word+i+19*k) = (uint32_t)paddedw[i+1+20*k]>>(2*j);
				j++;

			}
		//printf("maskMSB 0x%x\n", maskMSB);
		//printf("padded 32-bit word 0x%x\n", paddedw[i]);
		//printf("unpadded 32-bit word 0x%x\n", *(word+i));
		//usleep(ALT_MICROSECS_IN_A_SEC/100);
		}
		printf("full 600-bit line:\n");
		printf("0x");
		for( i = 18; i<255; i--){
			printf("%x ", *(word+i+19*k));
		}
		printf("\n");
		printf("full padded 640-bit line:\n");
		printf("0x");
		for (i = 19; i<255; i--){
			printf("%x ", paddedw[i+20*k]);
		}
		printf("\n");
	}
	free(*words);
	*words = word;
}*/
