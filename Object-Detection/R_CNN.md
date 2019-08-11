# R-CNN

## 物体检测相关基础知识
不同于分类问题，物体检测可能会存在多个检测目标    
这不仅需要判别出各个物体的类别，而且还要准确定位出物体的位置    
### 常用概念
#### Bounding Box(bbox)
bbox是包含物体的最小矩形，该物体应在最小矩形内部，如上图红色框蓝色框和绿色框。     
物体检测中关于物体位置的信息输出是一组(x,y,w,h)数据，其中x,y代表着bbox的左上角(或者其他固定点，可自定义)，对应的w,h表示bbox的宽和高.一组(x,y,w,h)可以唯一的确定一个定位框。     
#### Intersection over Union(IoU)
对于两个区域R和R′,则两个区域的重叠程度overlap计算如下:    

	O(R,R')=|intersection(R,R')| / |union(R,R')|    

训练网络时常依据侯选区域和标定区域的IoU值来确定正负样本    
    
#### 非极大值抑制(Non-Maximum Suppression又称NMS)
可以看做是局部最大值的搜索问题，NMS是许多计算机视觉算法的部分。如何设计高效的NMS算法对许多应用是十分关键的，例如视频跟踪、数据挖掘、3D重建、物体识别以及纹理分析等    
这里我们针对非极大值抑制在物体检测上的应用，非极大值抑制就是把不是极大值的抑制掉，在物体检测上，就是对一个目标有多个标定框，使用极大值抑制算法滤掉多余的标定框    

## R-CNN
由于较之于Fast R-CNN，现多把R-CNN称为Slow R-CNN    
R-CNN实现的目标检测系统可大致分为四步：   
1. 获取输入图像
2. 提取约2000个候选区域
3. 将候选区域分别输入CNN网络（这里需要将候选图片进行缩放）
4. 将CNN的输出输入SVM中进行类别的判定
上述四步为大致的检测过程，实际训练过程较麻烦，详述如下   

### 一、候选区域的提取
这部分有很多传统的方法可以选择，本文为了和之前的物体检测算法进行对比，选择了selective search方法    
### 二、候选区域的缩放
因为CNN对输入图像的大小有限制，所以在将候选区域输入CNN网络之前，要将候选区域进行固定尺寸的缩放   
缩放分为两大类（该部分在原文附录A）：   
#### 1）各向同性缩放，长宽放缩相同的倍数
+ tightest square with context：
把region proposal的边界进行扩展延伸成正方形，灰色部分用原始图片中的相应像素填补     
+ tightest square without context：
把region proposal的边界进行扩展延伸成正方形，灰色部分不填补   
#### 2）各向异性缩放, 长宽放缩的倍数不同
不管图片是否扭曲，长宽缩放的比例可能不一样，直接将长宽缩放到227\*227   
    
    
在放缩之前，作者考虑，在region proposal周围补额外的原始图片像素（pad p）   
最后试验发现，采用各向异性缩放并且p=16的时候效果最好    

### 三、测试方法
测试时其实分两个结果   
#### 1.分类
在测试的时候，先对带检测图像提取出约2000个候选区域，将每个区域都进行缩放，然后将缩放后的图片输入CNN进行特征提取，对CNN输出的特征用SVM进行打分(每类都有一个SVM，21类就有21个SVM分类器)，对打好分的区域使用NMS即非极大抑制(每类都单独使用)    
到这里分类就完成了，但是得到的位置只是候选区在图像中的位置，而候选区的位置并不一定就是ground truth，即检测目标的真实位置    
#### 2.定位(回归)
将CNN对候选区域提取出的特征输入训练好的线形回归器中，得到更为精确的位置定位，具体情况在训练方法中有详细介绍。但要明确的是，这些回归器是按照类来训练的，即每类分类完后进行回归    

### 四、训练方法
#### 1.预训练
训练的时候，文章用了个trick，先用ILSVRC2012数据库训练Alexnet，训练时目标是图片分类，因为ILSVRC2012数据库没有分类的标定数据。这步称为预训练    
#### 2.fine-tuning
这种方法也是当数据量不够的时候，常用的一种训练方式，即先用别的数据库训练网络，然后再用自己的数据库微调训练(fine-tuning)。微调期间，定义与ground truth的IoU大于0.5的候选区域为正样本，其余的为负样本。    
这里训练时，网络输出要有所改变，因为分类问题，网络输出为N+1，其中N为正样本的类别数，1为背景。   
对于VOC，N=20，对于ILSVRC2013, N=200。    
#### 3.目标分类
最终目标分类是通过SVM进行分类的，而不是通过网络框架中的softmax分类的   
先阐明两个问题：   
#### 1）SVM正负样本的定义，为什么fine-tuning与SVM正负样本定义不一样？
在训练SVM时，正样本为groundtruth，负样本定义为与ground truth的IoU小于0.3的候选区域，介于0.3与0.7之间的样本忽略   
fine-tuning为防止过拟合，要扩大正样本的样本量，所以定义比较宽松，但是SVM是最终用于分类的分类器，而且SVM原理就是最小的距离最大化，越难分的数据越有利于SVM的训练，所以对样本的定义比较严格   
#### 2）为什么不直接用softmax的输出结果？
因为在训练softmax的时候数据本来就不是很准确，而SVM的训练使用的是hard negative，样本比较严格，所以SVM效果会更好    
#### 4.回归器训练
回归器是线性的，输入为Alexnet pool5的输出    
bbox回归认为候选区域和ground-truth之间是线性关系(因为在最后从SVM内确定出来的区域比较接近ground-truth,这里近似认为可以线性关系)    
训练回归器的输入为N对值，{(P^i, G^i)i=1,2,...,N}，分别为候选区域的框坐标和真实的框坐标，下面在不必要时省略i。这里选用的Proposal必须和Ground Truth的IoU＞0.6才算是正样本   
从候选框P到预测框\hat{G}的基本思路如下：  
因为我们在分类之后得到候选框P (P_{x}, P_{y}, P_{w}, P_{h})，其中P_{x}和P_{y}为候选框的中心点，P_{w}和P_{h}为候选框的宽高，下面介绍中所有框的定位都用这种定义，即x和y表示中心点坐标，w和h表示框的宽高。知道候选框的表示，那么只要估计出出候选框与真实框的平移量和尺度缩放比例，就可以得到我们的估计框了    