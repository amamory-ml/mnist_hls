/*
 * LeNet_tb.cpp
 *
 *  Created on: 2020. 2. 27.
 *      Author: floyed
 */
#include "iostream"
#include <unistd.h>
#include "fstream"
#include "cstring"
#include "ap_fixed.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "parameters.h"
#include "MNIST_DATA.h"

#ifdef USE_FRED
#include "lenet_fred_top.hpp"
#else
#include "LeNet_AXIS.h"
#endif

//#include "opencv/cv.h"
using namespace std;
//using namespace cv;
//#define float ap_fixed<32,2>
/*
const string fname[] = {
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wconv1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bconv1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wconv3.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bconv3.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wconv5.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bconv5.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wpool1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bpool1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wpool2.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bpool2.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wfc1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bfc1.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/Wfc2.mdl",
		"/home/parallels/Documents/LeNet/Vivado_hls/filter/bfc2.mdl"
};

const int size[]={
		CONV_1_TYPE*CONV_1_SIZE,
		CONV_1_TYPE,
		CONV_2_TYPE*CONV_1_TYPE*CONV_2_SIZE/3,
		CONV_2_TYPE,
		CONV_3_TYPE*CONV_2_TYPE*CONV_3_SIZE/50,
		CONV_3_TYPE,
		POOL_1_TYPE*4,
		POOL_1_TYPE,
		POOL_2_TYPE*4,
		POOL_2_TYPE,
		FILTER_NN_1_SIZE/21,
		BIAS_NN_1_SIZE,
		FILTER_NN_2_SIZE,
		BIAS_NN_2_SIZE
};

void load_weight(string filename, ap_axis<HW_DATA_WIDTH,1,1,1> *src, ap_axis<HW_DATA_WIDTH,1,1,1>*dst,
		int length, int id){

	ifstream file(filename.c_str(), ios::in);
	if(file.is_open()){
		//printf("file opened\r\n");

		for(int i=0; i<length; i++){
			float tmp;
			file >> tmp;
			//float *ptr = &tmp;
			src[i].data = (ap_int<HW_DATA_WIDTH>)(tmp*DATA_CONVERT_MUL);
					//(ap_int<HW_DATA_WIDTH>)(((ap_fixed<HW_DATA_WIDTH,HW_DATA_WIDTH>)tmp)*DATA_CONVERT_MUL);
			src[i].keep = 1;
			src[i].strb = 1;
			src[i].user = 1;
			src[i].last = 0;
			src[i].id = 0;
			src[i].dest = 1;
			//if(i == length-1)
			//	src[i].last = 1;
		}
	}
	else{
		printf("Open file failed\r\n");
	}
	LeNet(src, dst, id);
}

void load_weight_spc(string filename, ap_axis<HW_DATA_WIDTH,1,1,1> *src, ap_axis<HW_DATA_WIDTH,1,1,1> *dst,
		int length, int times, int id_begin){
	float data[length*times];

	ifstream file(filename.c_str(), ios::in);
	if(file.is_open()){
		for(int i=0; i<length*times; i++){
			float tmp;
			file >> tmp;
			data[i] = tmp;
		}
	}
	else{
		printf("Open file failed\r\n");
	}

	for(int t=0; t<times; t++){
		for(int i=0; i<length; i++){
			src[i].data = (ap_int<HW_DATA_WIDTH>)(data[t*length+i]*DATA_CONVERT_MUL);
			src[i].keep = 1;
			src[i].strb = 1;
			src[i].user = 1;
			src[i].last = 0;
			src[i].id = 0;
			src[i].dest = 1;
			//if(i == length - 1)
			//	src[i].last = 1;
		}
		LeNet(src, dst, id_begin+t);
	}
}
*/

float MNIST_IMG[image_Move*MNIST_PAD_SIZE];
int MNIST_LABEL[image_Move];

int main(int argc, char* argv[]){
	printf("hello world\r\n");
	ap_axis<HW_DATA_WIDTH,1,1,1> src[BUFFER_SIZE], dst[CLASSES];
	
   char cwd[200];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }
   // the cwd is mnist_hls/Vivado_hls/LeNet_hls/origin/csim/build
	READ_MNIST_DATA("../../../../MNIST_DATA/t10k-images.idx3-ubyte",
			MNIST_IMG,-1.0f, 1.0f, image_Move);
	READ_MNIST_LABEL("../../../../MNIST_DATA/t10k-labels.idx1-ubyte",
			MNIST_LABEL,image_Move,false);

/*
	// debugging the input images
	for(int k=0; k<100; k++){
		for(int i=0; i<INPUT_WH; i++){
			for(int j=0; j<INPUT_WH; j++){
				cout << MNIST_IMG[k*INPUT_WH*INPUT_WH + i*INPUT_WH + j] << ',';
			}
			cout << endl;
		}
		cout << endl << endl << endl;
	}
*/

	int test_num = image_Move / image_Batch;
	int correct = 0;
	//for(int i=0; i<test_num; i++){
	for(int i=0; i<10; i++){
		
#ifdef USE_FRED
		data_t fred_data_in[image_Batch*INPUT_WH*INPUT_WH/sizeof(data_t)];
		data_t fred_data_out[CLASSES];
		int8_t *temp_data_in  = (int8_t *)fred_data_in;
		ap_int<HW_DATA_WIDTH> aux;
		args_t id_out;
		args_t args[ARGS_SIZE];
		args[0] = (args_t)0;
		args[1] = (args_t)0;

		printf("Using the FRED interface\n");
		int idx=0;
		for(int batch=0; batch<image_Batch; batch++){
			for(int j=0; j<INPUT_WH; j++){
				for(int k=0; k<INPUT_WH; k++){
					aux = (ap_int<HW_DATA_WIDTH>)(MNIST_IMG[(i+batch)*MNIST_PAD_SIZE + j*INPUT_WH + k]*DATA_CONVERT_MUL);
					temp_data_in[batch*MNIST_PAD_SIZE + j*INPUT_WH + k] = aux.to_char();
	//				printf("%04d: %04d: %04d:  %d - %f - %f - d %s - b %s - h %s - fixed %s\n", idx, batch*MNIST_PAD_SIZE + j*INPUT_WH + k, (i+batch)*MNIST_PAD_SIZE + j*INPUT_WH + k,
	//						temp_data_in[batch*MNIST_PAD_SIZE + j*INPUT_WH + k],
	//						MNIST_IMG[(i+batch)*MNIST_PAD_SIZE + j*INPUT_WH + k],
	//						aux_float*DATA_CONVERT_MUL,
	//						aux.to_string(10).c_str(), aux.to_string(2).c_str(), aux.to_string(16).c_str(),
	//						aux_fixed.to_string(10).c_str());
	//				idx++;
				}
			}
		}

		// printf("FRED INPUT: \n");
		// for(int batch=0; batch<image_Batch; batch++){
		// 	printf("batch: %d\n", batch);
		// 	for(int j=0; j<INPUT_WH; j++){
		// 		for(int k=0; k<INPUT_WH; k++){
		// 			printf("%02X, ",  temp_data_in[batch*MNIST_PAD_SIZE + j*INPUT_WH + k]);
		// 		}
		// 		printf("\n");
		// 	}
		// }
		// printf("\n\n\n");

		lenet_fred_top(&id_out, args, fred_data_in, fred_data_out);

		for(int j=0; j<CLASSES; j++){
			dst[j].data = (int8_t)(fred_data_out[j] & 0xFF);
		}

#else
		printf("Using the original AXI Streaming interface\n");
		for(int batch=0; batch<image_Batch*INPUT_WH*INPUT_WH; batch++){
			src[batch].data = (ap_int<HW_DATA_WIDTH>)(MNIST_IMG[i*MNIST_PAD_SIZE + batch]*DATA_CONVERT_MUL);
			src[i].keep = 1;
			src[i].strb = 1;
			src[i].user = 1;
			src[i].last = 0;
			src[i].id = 0;
			src[i].dest = 1;
		}

		LeNet_AXIS(src, dst, 0);
#endif
		float max_num = -10000;
		int max_id = 0;
		char tmp;
		float result[MNIST_LABEL_SIZE];
		for(int batch=0; batch<image_Batch; batch++){
			max_id = 0;
			max_num = -10000;
			printf("Output for iteration %d, batch %d:\n", i, batch);
			for(int index=0; index<MNIST_LABEL_SIZE; index++){
				tmp = dst[batch*MNIST_LABEL_SIZE + index].data;
				result[index] = (float)tmp/DATA_CONVERT_MUL;
				if(result[index] > max_num){
					max_num = result[index];
					max_id = index;
				}
				printf("%0.3f ", result[index]);
			}
			cout << endl;
			// for(int index=0; index<MNIST_LABEL_SIZE; index++){
			// 	tmp = dst[batch*MNIST_LABEL_SIZE + index].data;
			// 	printf("%d: %x ", batch*MNIST_LABEL_SIZE + index, tmp & 0xFF);
			// }
			// cout << endl;
			if(MNIST_LABEL[i*image_Batch+batch] == max_id)
				correct ++;
			cout << "Expected idx: " << i*image_Batch+batch << endl;
			cout << "Expected: " << std::dec << MNIST_LABEL[i*image_Batch+batch] << endl;
			cout << "Obtained: " << std::dec << max_id << ", (" << max_num << ")"<< endl;
			cout << "Rate: " << (float)correct/(i*image_Batch+batch+1) << endl << endl;
		}
		cout << endl;
	}
}
