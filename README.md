# mnist_hls
 Lenet for MNIST handwritten digit recognition using Vivado hls tool.
* Accuracy : 97.5938% (8-bit Mode).
* Speed : 71.43 FPS (100MHz, Zedboard)

## requirement
* Vivado hls 2019.2
* petalinux 2019.2 (optional)
* Vitis 2019.2
* python 3+

## usage
* Download MNIST dataset from [MNIST](http://yann.lecun.com/exdb/mnist/) and cpoy the dataset to '~/Vivado_hls/MNIST_DATA/'

* build hls project
```
# When you are in the project root directory
cd Vivado_hls
vivado_hls -f script.tcl
```

* Generate 'tanh' table (default: ap_fixed<8,3>)
```
# When you are in the project root directory
cd Vivado_hls/tools
python ./tanh_table.py
```

* build vivado project
```
# When you are in the project root directory
cd Vivado
vivado -source source/tcl
```

* Generate output product && export hardware.

* Open Vitis && create platform from *.xsa generated by vivado.

* Create an application project based on the 'hello world' template.

* Use this file './Vitis/helloworld.c' replace the original file and add file './Vitis/mnist_data.h'.

* Run and see the results

## DART/FRED Compatibility

This HLS design has been adapted to work with DART and FRED tools designed by [Retis lab](retis.sssup.it/) in Scuola Sant'anna, Pisa.

In the 'Vivado_hls/script.tcl' file change `set use_fred 1` to use the design with FRED interface or set it to `set use_fred 0` to use the original interface based on AXI streaming.

Check the branch `debug` to see another version of the HLS source code with lots of additional code for debug. They were remove from the master branch because they let the code harder to read and understand.

## TODO
* Simplify lookup table of 'tanh'.
* Complete PS design based on embedded linux.
* Batch size != 1 is not working. See the parameter  `image_Batch` in the 'Vivado_hls/include/parameters.h'

## Authors

* Orignal design by FloyedShen;
* Adaptation to DART/FRED by Alexandre Amory,Scuola Sant'anna, Pisa.


