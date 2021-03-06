# Int8量化
## 0 Pre

### 0.1 关于C++
- CMakeLists.txt
- Makefile

### 0.2 关于NCNN
#### param文件
- 版本信息
- 层与数据交换结构数量
- 相关层的具体信息
    - input:
        - 层类型
        - 层名称
        - 输入数据结构数量(bottom blob)
        - 输出数据结构数量(top blob)
        - 网络输入层名(一个层可能有多个输入，则有多个网络输入层名)
        - 网络输出层名(一个层可能由多个输出，则有多个网络输出层名)
        - 特殊参数(可能没有)

    - conv(以第一个卷积层为例)
        - 层类型:Convolution
        - 层名称:conv1
        - 输入数据结构数量(bottom blob 1)
        - 输出数据结构数量(top blob 1)
        - 网络输入层名(data)
        - 网络输出层名(conv1)
        - 特殊参数1：0=20，num_output: 20
        - 特殊参数2：1=5，kernel_size: 5
        - 特殊参数3：2=1，stride: 1
        - 特殊参数4：3=1
        - 特殊参数5：4=0
        - 特殊参数6：5=1
        - 特殊参数7：6=500，该层的参数量，5*5*1*20=500

## A Int8量化－介绍
- (知乎原文链接)[https://zhuanlan.zhihu.com/p/58182172]
### 前言
本系列目的：
- **底层Int8汇编层**实现原理以及汇编性能优化手段
- **中间层移动框架**的配套代码实现(NCNN)
- **上层PC端**的量化方法总结及实现
- **模型端re-train**方法、策略及指标实现
### 1 背景
#### 1.1 量化背景
移动端加速方案
- GPU
- CPU
    - 算法层，如可以用wingrad从数学上减少乘法的数量
    - 框架实现层可以实现内存池、多线程等策略
    - 底层就是硬件相关细节：硬件架构,pipeline,cache,内存数据重排，NEON汇编优化

量化的三个问题
- 为什么量化有用
    - 因为CNN对噪声不敏感
- 为什么用量化
    - 模型太大，存储压力太大
    - 每个层weights范围基本确定，且波动不大，适合量化压缩
    - 既减少仿存又减少计算量
- 为什么不直接训练低精度模型
    - 训练需要反向传播和梯度下降，int8不适合
    - 生态即为浮点模型
#### 1.2 量化方法综述
- 参考github--MVision
### 2 INT8量化算法原理
#### 2.1 INT8量化原理
- 英伟达的tensorRT方案，直接量化，无需retrain
- Google的方案，需要retrain
    - retrain的要求：权值及激活值必须分布均匀，即方差不要太大；其次应控制每层的输出在一定的范围，防止输出的溢出
- 目的：将原来的float32bit卷积操作转换为int8的卷积操作

```
FP32 Tensor(T) = scale_factor(sf) * 8-bit Tensor(t) + FP32_bias(b)
```

通过实验，bias值去掉对精度影响不大

```
T = sf * t
```
#### 思考
- 为什么量化可以保证原信息
    - 类似与高清图和低分辨率的区别
- 为什么最大值映射会精度损失严重
    - 考虑正负分布不均匀的情况
- 为什么右边的饱和截取可行
    - 因为非饱和截取的问题是当数据分布极不均匀时，很多动态范围是浪费的。而饱和截取可以保证当数据分布不均匀时，可以在映射原始信息之前找到一个**阈值T**截断一部分然后构成分布良好的截断信息，再把这个信息映射到INT8范围
- 问题转化为：如何寻找最优阈值T使得精度损失最小
    - 建立一个模型来评估量化前后的精度损失，然后使得损失最小
#### NVIDIA的思路
- 选择**KL-divergence**，即相对熵，表述两个分布的差异程度
    - 量化前后两个分布的差异程度
    - 熵越大代表事件发生概率小，不可能性大
- 从编码角度理解相对熵
    - F32是原来的最优编码，int8是次优编码，用KL散度来描述两种编码的差异
        - 相对熵表示采用次优编码时会需要多少个bit来编码，即与最优编码之间的bit差
        - 交叉熵表示用次优编码确切需要多少个bit表示
        - 最优编码所需**bits=交叉熵-相对熵**
#### 2.2 INT8量化流程
首先，准备一个校准数据集，然后
- 收集激活值的直方图
- 基于不同阈值产生不同的量化分布
- 计算每个分布与原分布的相对熵，选择熵最少的一个，即和原分布最像的一个

最关键的即是**校准算法部分**
- calibration:基于实验的迭代搜索阈值
- 校准是其核心部分，在校准数据集上运行FP32推理，收集激活的直方图，并生成一组具有不同阈值的8位表示法，并选择具有最少KL散度表示

#### 2.3 INT8量化实现－校准算法
- 首先不断地截断参考样本P，长度从128开始到2048，为什么从128开始呢？因为截断的长度为128的话，那么我们直接一一对应就好了，完全不用衰减因子了
- 将截断区外的值全部求和；
- 截断区外的值加到截断样本P的最后一个值之上；（截断区之外的值为什么要加到截断区内最后一个值呢？我个人理解就是有两个原因，其一是求P的概率分布时，需要总的P总值，其二将截断区之外的加到截断P的最后，这样是尽可能地将截断后的信息给加进来。）
- 求得样本P的概率分布；
- 创建样本Q，其元素的值为截断样本P的int8量化值；
- 将Q样本长度拓展到 i ，使得和原样本P具有相同长度；
- 求得Q的概率分布；
- 然后就求P、Q的KL散度值就好啦

上面就是一个循环，不断地构造P和Q，并计算相对熵，然后找到最小（**截断长度为m**）的相对熵，此时表示Q能极好地拟合P分布了。

**而阀值就等于（m + 0.5）*一个bin的长度**
#### 2.4 INT8量化实现－校准实现(python)
### 3 INT8移动端实现
#### 3.1 NCNN INT8实现
##### 3.1.1 底层量化处理流程
在每层计算时是需要先将feature map量化到INT8，然后将weights量化到INT8，最后卷积计算得到INT32的输出值，输出值乘以scale（float）值反量化到float，然后加上浮点格式的bias作为下一层的输入
##### 3.1.2 量化前处理
NCNN的`quantize.cpp`有量化的实现
- 将位机校准程序得到的**scale参数**乘以浮点值，给压缩到int8的范围内来，然后直接float2int转换，最后做下边界处理，这样量化的前处理就处理好了
##### [TODO]3.1.3 量化计算
NCNN的`convolution_1x1_int8.h`中有其实现，分别支持了Armv7跟Armv8
##### 3.1.4 量化后处理
NCNN的`dequantize.cpp`函数里面有其去量化的实现，把卷积最后输出的int32乘以scale
#### [TODO]3.2 优化INT8实现逻辑

## B INT8量化-python实现及代码分析
- [知乎原文链接](https://zhuanlan.zhihu.com/p/58208691)
### 1 量化的本质
量化就是原信号的采样
### 2 校准算法流程回顾
- 校准数据集在F32模式下跑一遍，然后将每个layers的数据收集起来
- 对于模型的每一层
    - 得到对应layer激活值的直方图
        - 直方分布bins个数设置多少
        - 直方分布bins宽度设置多少
        - 激活值一般是值ReLU之后的值，但是这样的激活值一定好吗
    - 不同截断阈值下产生许多的量化分布
        - 截断之后选择多少个bins压入一个bin,边界bin如何处理
        - 尾数如何处理，这样处理的原因是什么
    - 选择KL距离最小的阈值
        - 为什么KL最小的阈值一定最好
        - KL距离最小代表什么物理意义
        - KL smooth处理的数学原因是什么
### 3 获取激活值的概率分布
### 4 最大值映射会有显著精度损失的原因
### 5 激活值的直方分布到底取正半区还是负半区
**需要将负半轴的数据考虑进去**
- 追根究底
    - 在VGG19模型下，激活值全是正的情况下，取正半轴做量化，对吗？
        - 结论：若为非溢出实现，没什么问题，但如果是溢出实现，则可能有精度大幅度损失
### [TODO]6 饱和截断后如何获取目标量化分布
### 7 smooth处理的原因

# Realization
## NCNN官方
### quantized int8 inference





    





