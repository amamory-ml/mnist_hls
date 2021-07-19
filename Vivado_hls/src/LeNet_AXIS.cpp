/*
 * LeNet.cpp
 *
 *  Created on: 2020. 2. 27.
 *      Author: floyed
 */

/********************************************************************/
/*					correct rate: 0.975938                          */
/********************************************************************/
#include "LeNet.h"
#include "iostream"
#include "cstring"
#include "ap_fixed.h"
using namespace std;

//create layer

void LeNet_AXIS(ap_axis<HW_DATA_WIDTH,1,1,1>src[BUFFER_SIZE], ap_axis<HW_DATA_WIDTH,1,1,1>dst[CLASSES], int id){

	ap_axis<HW_DATA_WIDTH, 1, 1,1> data[BUFFER_SIZE];
	hw_fixed input[image_Batch][INPUT_WH][INPUT_WH];
	hw_fixed output[image_Batch*OUTPUT_NN_2_SIZE];

	Copy:
	for(int i=0; i<BUFFER_SIZE; i++){
		data[i].data = src[i].data;
		data[i].dest = src[i].dest;
		data[i].id = src[i].id;
		data[i].keep = src[i].keep;
		data[i].last = src[i].last;
		data[i].strb = src[i].strb;
		data[i].user = src[i].user;
	}

	load_batch:
	for(int batch=0; batch<image_Batch; batch++){
		load_row:
		for(int i=0; i<INPUT_WH; i++){
			Load_col:
			for(int j=0; j<INPUT_WH; j++){
				int index = i*INPUT_WH+j;
				input[batch][i][j] = ((float)data[index].data)/DATA_CONVERT_MUL;
			}
		}
	}

	LeNet(input,output,0);

	Output:
	for(int i=0; i<CLASSES-1; i++){
//#pragma HLS pipeline
		dst[i].data = ((float)output[i])*DATA_CONVERT_MUL;//((int*)output)[i];
		//cout<<output[i]<<' '<<((float)output[i])<<' '<<((float)output[i])*DATA_CONVERT_MUL<<endl;
		dst[i].keep = data[i].keep;
		dst[i].strb = data[i].strb;
		dst[i].user = data[i].user;
		dst[i].last = data[i].last;
		dst[i].id = data[i].id;
		dst[i].dest = data[i].dest;
	}//cout << endl;

	dst[CLASSES-1].data = ((float)output[CLASSES-1])*DATA_CONVERT_MUL;//((int*)output)[i];
	//cout<<output[i]<<' '<<((float)output[i])<<' '<<((float)output[i])*DATA_CONVERT_MUL<<endl;
	dst[CLASSES-1].keep = data[CLASSES-1].keep;
	dst[CLASSES-1].strb = data[CLASSES-1].strb;
	dst[CLASSES-1].user = data[CLASSES-1].user;
	dst[CLASSES-1].last = 1;
	dst[CLASSES-1].id = data[CLASSES-1].id;
	dst[CLASSES-1].dest = data[CLASSES-1].dest;

	/*
	Output_fill:
	for(int i=CLASSES; i<BUFFER_SIZE; i++){
		dst[i].data = data[i].data;
		dst[i].keep = data[i].keep;
		dst[i].strb = data[i].strb;
		dst[i].user = data[i].user;
		dst[i].last = data[i].last;
		dst[i].id = data[i].id;
		dst[i].dest = data[i].dest;
	}*/

}
