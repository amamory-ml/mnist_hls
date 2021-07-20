############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
# set this variable to 0 to use the original AXIS interface, 
# or other value to use the new FRED/DART compatible interface
set use_fred 1

# select different kinds of interfaces for lenet
if {$use_fred != 0} {
    open_project lenet_fred2
    set_top lenet_fred_top
    add_files src/lenet_fred.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
    add_files src/lenet_fred_top.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
    add_files src/LeNet.cpp -cflags "-Iinclude -Ifilter -DCOMP_DATA -Wno-unknown-pragmas"
} else {
    open_project lenet_axis
    set_top LeNet_AXIS
    add_files src/LeNet_AXIS.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
    add_files src/LeNet.cpp -cflags "-Iinclude -Ifilter -DREF_DATA -Wno-unknown-pragmas"
}
# this is the top of the logic
add_files src/activation.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
add_files src/convolution.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
add_files src/fullyconnected.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
add_files src/pooling.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
if {$use_fred != 0} {
    add_files -tb src/LeNet_tb.cpp -cflags "-Iinclude -Wno-unknown-pragmas -DUSE_FRED"
} else {
    add_files -tb src/LeNet_tb.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
}
add_files -tb src/MNIST_DATA.cpp -cflags "-Iinclude -Wno-unknown-pragmas"
open_solution "origin"
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
#csim_design
csim_design -clean
#csynth_design
#cosim_design
#export_design -format ip_catalog
