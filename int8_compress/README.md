# 基于NCNN---INT8量化
## 代码结构
```
|--int8_compress               
    |--3rdParty
        |--ncnn
            |--include
            |--lib
        |--ncnn
    |--src
        |--headpose.cpp
        |--CMakeLists.txt
    |--caffe-int8-convert-tools
        |--classification-dev
        |--detection-dev
        |--cali_table.sh
        |--caffe-int8-convert-tool-dev-weight.py
        |--pytorch-int8-convert-tool-dev-weight.py
    |--tools
        |--convert.sh 
        |--CMakeLists.txt 
               
```
## FP32-INT8整体流程
### 1 caffe-int8-convert-tools
- 运行`sh test.sh`
    - 得到`headpose.table`，作为`ncnn`中`caffe2ncnn`的量化校准系数输入
### 2 tools
- 运行`sh convert.sh`
    - 得到经过INT8量化的`heapose-int8.param`和`headpose-int8.bin`
### 3 src
- 整体编译后，运行`./headpose test.jpg`，运用ncnn进行前传，对比量化前后的结果

# INT8 Quantization
This is a small project which I completed during my internship in Gamma lab,PingAn technology.
The goal of this project is to compress the original model to a smaller size with certain loss of
precision.Because of this goal, I chose int8 quantization as my basic solution, the whole procedure
and code structure are explained as follows.

- [Github link](https://github.com/lanhongvp/PA_Gamma)

# Code Structure

```
|--int8_compress               
    |--3rdParty
        |--ncnn
            |--include
            |--lib
        |--ncnn
    |--src
        |--headpose.cpp
        |--CMakeLists.txt
    |--caffe-int8-convert-tools
        |--classification-dev
        |--detection-dev
        |--cali_table.sh
        |--caffe-int8-convert-tool-dev-weight.py
        |--pytorch-int8-convert-tool-dev-weight.py
    |--tools
        |--convert.sh 
        |--CMakeLists.txt 
               
```

# Basic steps

## 1 Preparation: Obtain the caffe model and proto file

First, we should get the original caffe model and the corresponding proto file

## 2 Begin to realize INT8 Quantization
### 2.1 Get the calibration table
Second, based on ncnn, which is a fantastic hpc framework, we found its wiki
has official tools to help us get a calibration table through the original caffe model.
The corrsponding part of my project is the `caffe-int8-convert-tools`.And I write a shell
script `cali_table.sh` to get the int8 calibraion table.

### 2.2 Get the final int8 quantization model and original ncnn model
Third, when we get the calibration table, we get convert the original caffe model to a int8
model.And the shell script is `tools/convert.sh`. This script use the tools `caffe2ncnn` 
in ncnn, which takes **caffe model,prototxt file,int8 calibraion table,quantitative indicators** as inputs, and the
outputs are **param,bin** files which are used as infer model in ncnn.
- we can choose the quantitative indicators to decide whether to use the quantization or not. *If the value is 0,it will not use quantization*

## 3 Test the precision
Last, compare the original ncnn model and the int8 model. Use some test images to test the two models and compare the 
results. Based on this project,the detail is in `src/headpose.cpp`, you can write a different one according to your own model.

# Some thoughts
I have learned a lot through this project. It helps me know how to compress a large model and make it work in real life
instead of tuning some parameters. And it also improve my C++ skills through this project.