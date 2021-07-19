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

#include "lenet_fred.hpp"
#include "LeNet.h"

#include "parameters.h"


void lenet_fred(args_t *id, args_t args[ARGS_SIZE], volatile data_t *mem_in, volatile data_t *mem_out)
{
	// the order of the args must be consistent w the order in the fred_bind function
	data_t *data_in = (data_t *)&mem_in[args[0] / sizeof (data_t)];
	data_t *data_out = (data_t *)&mem_out[args[1] / sizeof (data_t)];
	hw_fixed *fred_in= (hw_fixed *)data_in;
	hw_fixed *fred_out= (hw_fixed *)data_out;

	LeNet(fred_in,fred_out,0);

}
