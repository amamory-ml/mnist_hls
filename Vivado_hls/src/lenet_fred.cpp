/*
 * FRED interface for LeNet CNN.
 *
 * Copyright (C) 2021, Alexandre Amory, ReTiS Lab.
 * <alexandre.amory(at)santannapisa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
*/

#include <stdlib.h>
#include "lenet_fred.hpp"
#include "LeNet.h"

#include "parameters.h"

#if defined(FRED_REF_DATA)
void print_bin(char *str, data_t n, uint32_t nbits){
	int i=0;
	for (i = 0; i < nbits; i++)	{
		if (n & 1)
			str[i] = '1';
		else
			str[i] = '0';

		n >>= 1;
	}
	str[i] = '\0';
}
#endif

void lenet_fred(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];
	//hw_fixed *fred_in= (hw_fixed *)data_in;
	//hw_fixed *fred_out= (hw_fixed *)data_out;
	hw_fixed fred_out[image_Batch*OUTPUT_NN_2_SIZE];
	//ap_fixed<HW_DATA_WIDTH, HW_DATA_INTEGER, AP_RND_ZERO, AP_SAT> fred_out[image_Batch*OUTPUT_NN_2_SIZE];
	//data_t * fred_out_ptr = (data_t *)fred_out;

	//hw_fixed fred_in[image_Batch][INPUT_WH][INPUT_WH];
	//data_t * fred_in_ptr = (data_t *)fred_in;
	//memcpy(fred_in_ptr,data_in,image_Batch*INPUT_WH*INPUT_WH);
	//LeNet(fred_in,fred_out,0);

	// both array of data_t and the array hw_fixed have the same memory layout
	// So, this cast can eliminate the memcpy
	LeNet((hw_fixed(*)[INPUT_WH][INPUT_WH]) data_in,fred_out,0);

#if defined(FRED_REF_DATA)
	FILE *fp;
	fp = fopen("fred_output_ref.dat", "w");
	if (fp == NULL)	{
	    printf("Error opening file!\n");
		exit(1);
	}
	printf("Writing the reference OUTPUT data\n");
	for(int k=0; k<image_Batch*OUTPUT_NN_2_SIZE; k++){
		//fprintf(fp,"%s\n", fred_out[k].to_string(2).c_str()); 
		fprintf(fp,"%04d. f %f - d %s - b %s - h %s\n", k, fred_out[k].to_float(), fred_out[k].to_string(10).c_str(), fred_out[k].to_string(2).c_str(), fred_out[k].to_string(16).c_str());
		//fprintf(fp,"%2X\n", input[k].to_int());//
	}
	char buffer[50];
#endif

	float aux_float;
	for (size_t i = 0; i < image_Batch*OUTPUT_NN_2_SIZE; i++)
	{
		aux_float = fred_out[i].to_float(); // convert 8 bit into data_t, 64 bits
		//printf("FLOAT %f - %f\n",aux_float, aux_float*DATA_CONVERT_MUL);
		data_out[i] = (data_t)(aux_float*DATA_CONVERT_MUL);
		//data_out[i] = (data_t)(fred_out[i] && 0xFF); // convert 8 bit into data_t, 64 bits
		//data_out[i] = fred_out[i].to_int(); // convert 8 bit into data_t, 64 bits
#if defined(FRED_REF_DATA)
		//itoa (data_out[i],buffer,2);
		print_bin(buffer,data_out[i],8);
		//fprintf(fp,"%lld\n", data_out[i]);
		fprintf(fp,"%s\n", buffer);
		//fprintf(fp,"%04d. f %f - d %s - b %s - h %s\n", k, fred_out[k].to_float(), fred_out[k].to_string(10).c_str(), fred_out[k].to_string(2).c_str(), fred_out[k].to_string(16).c_str());
#endif
	}

#if defined(FRED_REF_DATA)
	fclose(fp);
#endif

	//memcpy(data_out,fred_out_ptr,image_Batch*OUTPUT_NN_2_SIZE);

}
