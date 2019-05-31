# 基于NCNN---INT8量化
## 代码结构
```
|--int8_compress               
    |--3rdParty
        |--ncnn
            |--include
            |--lib
    |--src
        |--headpose.cpp
        |--CMakeLists.txt
    |--caffe-int8-convert-tools
    |--tools
        |--convert.sh        
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
