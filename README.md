# BaseCode
库中包含了平时使用的比较多的基础代码，包括python语言和C/C++

目录以及代码说明：

## TreatmentDatasets: 

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
	* 