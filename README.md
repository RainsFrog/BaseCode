# BaseCode
库中包含了平时使用的比较多的基础代码，包括python语言和C/C++

目录以及代码说明：

## TreatmentDatasets: 

KeyWords：数据集处理，VOC， COCO， Yolo， 图像增强

数据处理代码，基本上是python实现

 * convert_xml_to_yolo_object.py

   将xml（VOC）格式的图片标注文件转换成YOLO格式的txt标注文件，

* genfiles.py: 

  VOC格式数据集生成训练数据的代码。原数据集格式：

  ​	|->VOCdevkit

  ​	|-> |-> VOC2007

  ​	|-> |-> |-> Annotations

  ​	                 |->JPEGImages

  ​	运行后生成labels文件夹以及Imagesets文件夹
  ​	├─VOC2007
  ​	│  ├─anchors
  ​	│  ├─Annotations
  ​	│  ├─ImageSets
  ​	│  │  └─Main
  ​	│  ├─JPEGImages
  ​	│  ├─labels
  ​	└─VOC2012
  ​    	└─labels

  ​	在VOCdevkit同级目录或者在其根目录下，会生成train.txt以及test.txt两个文件，训练时训练集和测试集通过这两个文件进行指定。

	*

* imgEhance.py

  图像增强代码，通过使用imgaug库实现图像增强。对于原数据可以覆盖也可以复制。生成新的VOC格式的数据集。

* renameImgAndRemove.py

  整理数据集，统一编号，文件名为12位的数字，前8位设置位数据处理日期，后四位为当前日期下的顺序编号。对于质量不好的数据没有标注而且没有生成xml文件，对于这部分图片进行了删除处理

* testTheConvertResult.py

  处理结果测试代码，对于数据较少的数据集，可以使用标注工具（本人使用的是labelImg）进行查看，对于处理数据比较多的，该代码可以随机查看指定数量的图片以及标注结果，界面显示。

* voc2coco.py

  现有xml格式的VOC数据集转换成COCO数据集格式，会生成json格式文件

* 标签和图片对应关系比较.py

  查看标签和图片的对应关系，是否一一对应，生成两个文件夹的文件列表，顺序比较

* 04ImagePretreatment.cpp

  这是一个自己实现的opencv-C++中的目前了解到的图像处理算法，包括特征查找、亮度变化、模糊等方法

---

# MOT

此文件夹中放置的MOT（多目标跟踪）中的一些基础代码。

* MultiThreadTracker.cpp

  多线程实现KCF多目标跟踪。初始帧绘制跟踪目标。后续可添加新的跟踪目标。

  TODO:

  - [ ] 增加目标重合判断，新选取的目标与原目标重合度是否过大。