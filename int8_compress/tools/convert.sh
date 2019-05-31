#!/bin/bash

cd '/home/lan/working/ncnn/build/tools/caffe'

./caffe2ncnn /home/lan/working/PA_Gamma/int8_compress/models/headpose.prototxt /home/lan/working/PA_Gamma/int8_compress/models/headpose.caffemodel /home/lan/working/PA_Gamma/int8_compress/models/headpose-int8.param /home/lan/working/PA_Gamma/int8_compress/models/headpose-int8.bin 256 /home/lan/working/PA_Gamma/int8_compress/caffe-int8-convert-tools/headpose.table 
