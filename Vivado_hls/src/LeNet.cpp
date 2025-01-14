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
#include <stdlib.h>
#include "cstring"
#include "ap_fixed.h"

//layers
#include "activation.h"
#include "convolution.h"
#include "fullyconnected.h"
#include "pooling.h"

using namespace std;

//conv layer weight & bias
const hw_fixed Wconv1[CONV_1_TYPE][CONV_1_WH][CONV_1_WH] = {
#include "Wconv1.h"
};
const hw_fixed Bconv1[CONV_1_TYPE] = {
#include "bconv1.h"
};
const hw_fixed Wconv2[CONV_2_TYPE][CONV_1_TYPE][CONV_2_WH][CONV_2_WH] = {
#include "Wconv3.h"
};
const hw_fixed Bconv2[CONV_2_TYPE] = {
#include "bconv3.h"
};
const hw_fixed Wconv3[CONV_3_TYPE][CONV_2_TYPE][CONV_3_WH][CONV_3_WH] = {
#include "Wconv5.h"
};
const hw_fixed Bconv3[CONV_3_TYPE] = {
#include "bconv5.h"
};

//pool layer weight & bias
const hw_fixed Wpool1[POOL_1_TYPE*4] = {
#include "Wpool1.h"
};
const hw_fixed Bpool1[POOL_1_TYPE] = {
#include "bpool1.h"
};
const hw_fixed Wpool2[POOL_2_TYPE*4] = {
#include "Wpool2.h"
};
const hw_fixed Bpool2[POOL_2_TYPE] = {
#include "bpool2.h"
};

//fc layer weight & bias
const hw_fixed Wfc1[FILTER_NN_1_SIZE] = {
#include "Wfc1.h"
};
const hw_fixed Bfc1[BIAS_NN_1_SIZE] = {
#include "bfc1.h"
};
const hw_fixed Wfc2[FILTER_NN_2_SIZE] = {
#include "Wfc2.h"
};
const hw_fixed Bfc2[BIAS_NN_2_SIZE] = {
#include "bfc2.h"
};

//create layer
//hw_fixed input[image_Batch][INPUT_WH][INPUT_WH];
hw_fixed conv1[image_Batch][CONV_1_TYPE][CONV_1_OUTPUT_WH][CONV_1_OUTPUT_WH];
hw_fixed pool1[image_Batch][CONV_1_TYPE][CONV_2_INPUT_WH][CONV_2_INPUT_WH];
hw_fixed conv2[image_Batch][CONV_2_TYPE][CONV_2_OUTPUT_WH][CONV_2_OUTPUT_WH];
hw_fixed pool2[image_Batch][CONV_2_TYPE][CONV_3_INPUT_WH][CONV_3_INPUT_WH];
hw_fixed conv3[image_Batch][CONV_3_TYPE];
hw_fixed fc1[image_Batch][OUTPUT_NN_1_SIZE];
//hw_fixed output[image_Batch*OUTPUT_NN_2_SIZE];

void LeNet(hw_fixed input1[image_Batch][INPUT_WH][INPUT_WH], hw_fixed output1[image_Batch*OUTPUT_NN_2_SIZE], int id){
	Convolution_Layer_1(input1, Wconv1, Bconv1, conv1);
	Pooling_Layer_1(conv1, Wpool1, Bpool1, pool1);
	Convolution_Layer_2(pool1, Wconv2, Bconv2, conv2);
	Pooling_Layer_2(conv2, Wpool2, Bpool2, pool2);
	Convolution_Layer_3(pool2, Wconv3, Bconv3, conv3);
	Fully_Connected_Layer_1(conv3, Wfc1, Bfc1, fc1);
	Fully_Connected_Layer_2(fc1, Wfc2, Bfc2, output1);
}
