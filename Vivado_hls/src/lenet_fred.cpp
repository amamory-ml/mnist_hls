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

//#include <stdlib.h>
#include "lenet_fred.hpp"
#include "LeNet.h"

#include "parameters.h"

void myMemCpy(void *dest, const void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;

   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}

void lenet_fred(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];
	hw_fixed fred_out[image_Batch*OUTPUT_NN_2_SIZE];
	uint32_t index, batch, i, j, b;

	hw_fixed fred_in[image_Batch][INPUT_WH][INPUT_WH];
	data_t * fred_in_ptr = (data_t *)fred_in;
	uint64_t shift_reg;
	uint8_t aux_byte;
	hw_fixed aux_fixed;
	float aux_float;
	union {
		int8_t bytes[8];
		//hw_fixed fixed[8];
		data_t data;
	} data_in_vet[image_Batch*INPUT_WH*INPUT_WH/8];

	//myMemCpy((void*)fred_in_ptr,(void*)data_in,image_Batch*INPUT_WH*INPUT_WH);

	index=0;
	load_batch:
	for(batch=0; batch<image_Batch; batch++){
		load_row:
		for(i=0; i<INPUT_WH; i++){
			Load_col:
			for(j=0; j<INPUT_WH/sizeof(data_t); j++){
				shift_reg = data_in[index];
				//printf("data_in[%d] = %s\n", index, shift_reg.to_string(16).c_str());
				for(b = 0; b < sizeof(data_t); b++){
					#pragma HLS unroll
					aux_byte = shift_reg & 0xFF;
					aux_fixed = (hw_fixed)aux_byte;
					aux_float = aux_fixed.to_float();
					fred_in[batch][i][j * sizeof(data_t) +b ] = aux_fixed;
					printf("[%02d][%02d][%02d] = b %02X - fi %s - f %f\n", batch, i, j * sizeof(data_t) +b, aux_byte, aux_fixed.to_string(16).c_str(), aux_float);
					shift_reg >>= 8;
				}
				index++;
			}
		}
	}
	//printf ("index: %d\n", index);

	LeNet(fred_in,fred_out,0);

	// both array of data_t and the array hw_fixed have the same memory layout
	// So, this cast can eliminate the memcpy
	//LeNet((hw_fixed(*)[INPUT_WH][INPUT_WH]) data_in,fred_out,0);

	for (i = 0; i < image_Batch*OUTPUT_NN_2_SIZE; i++){
		// 8bit fixed-size output is converted to float and assigned to 64bit integer
		data_out[i] = (data_t)(fred_out[i].to_float()*DATA_CONVERT_MUL);
	}
	//memcpy(data_out,fred_out_ptr,image_Batch*OUTPUT_NN_2_SIZE);
}
