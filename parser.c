#include "parser.h"
#include "testsdram.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "hwlib.h"
#include <unistd.h>
//#include "globals.h"

void parse_weights(char* file, int32_t** words){
	printf("Starting parser\n");
	int32_t * word = calloc(NBWORDS, sizeof(int32_t));
	int8_t param[NBPARAM_IN_WORD];
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
	printf("i am done with unlegitimate bugs now \n");
	do
	{
		//printf("why is this stuck\n");
		CH = fgetc(weights_file);
		if(feof(weights_file))
		{
			*(word+NBWORDS-1) = params2word(param);
			printf("after concat: 0x%x\n", *(word+NBWORDS-1));
			break;
		}
		if (CH != '0' && CH != '1' && CH != '2' && CH != '3' && CH != '4' \
				&& CH != '5' && CH != '6' && CH != '7' && CH != '8' && CH != '9' \
				&& CH != '-' && CH != '+' && CH != 'e' && CH != '\n' && CH != ',' && CH != '.')
		{
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
					*(word+word_cnt) = params2word(param);
					for (i = 0; i < NBPARAM_IN_WORD; i++)
						param[i]=0;

					printf("after concatenat: 0x%x\n", *(word+word_cnt));
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
	free(*words);
	*words = word;
}

void parse_rtdata(file,line){
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

void concat_words(int32_t** words, int32_t paddedw[NBWORDS]){
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
}
