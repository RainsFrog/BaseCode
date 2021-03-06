#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# -----------------------------------------------------
# @Filename    :genfiles.py
# @Notice    : 将voc格式的数据集分割成训练集和测试集，为避免麻烦，最好将
              #要处理的数据集编程VOC格式即如下所示：
              # 
              #------------------------------------------------
              # genfiles.py
              # VOCkeit
              # |____VOC2007
              #      |____JPEGImages --> 存放数据图片
              #      |____Anotations --> 存放xml格式数据标签
              #      |____labels --> .txt格式的标签存储位置
              #
              # 最后会在VOCKeit同级目录下生成train.txt， test.txt两个数据索引。darknet训练时可以直接再data中指定这两个文件即可。
              # ------------------------------------------------
              # 如果要追加数据，可以将原VOCKeit文件夹改名，并将新的数据按照上述的文件树存储
              # 数据，然后执行该文件。将执行过后生成的图片、标签文件复制到总数据集中， 数据路劲该索引，即train.txt中的内容复制粘贴到
              # 原来的train.txt文件后就行。
# @Time    :2021/02/02 12:58:56
# @Author    :RainsFrog
# @Version    :v1.0
# ----------------------------------------------------


import xml.etree.ElementTree as ET
import pickle
import os
from os import listdir, getcwd
from os.path import join
import random

classes=["boat","pier"]


def clear_hidden_files(path):
    dir_list = os.listdir(path)
    for i in dir_list:
        abspath = os.path.join(os.path.abspath(path), i)
        if os.path.isfile(abspath):
            if i.startswith("._"):
                os.remove(abspath)
        else:
            clear_hidden_files(abspath)

def convert(size, box):
    dw = 1./size[0]
    dh = 1./size[1]
    x = (box[0] + box[1])/2.0
    y = (box[2] + box[3])/2.0
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x*dw
    w = w*dw
    y = y*dh
    h = h*dh
    return (x,y,w,h)

def convert_annotation(image_id):

    in_file = open('VOCdevkit\VOC2007\Annotations\%s.xml' %image_id, encoding="UTF-8")
    out_file = open('VOCdevkit\VOC2007\labels\%s.txt' %image_id, 'w', encoding="UTF-8")
    print('正在处理：VOCdevkit\VOC2007\Annotations\%s.xml' %image_id)
    tree=ET.parse(in_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls = obj.find('name').text
        if (difficult != "0" or difficult != "1"):
            difficult = "0"
        if cls not in classes or int(difficult) == 1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text), float(xmlbox.find('ymax').text))
        bb = convert((w,h), b)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')
    in_file.close()
    out_file.close()

wd = os.getcwd()
wd = os.getcwd()
work_sapce_dir = os.path.join(wd, "VOCdevkit\\")
if not os.path.isdir(work_sapce_dir):
    os.mkdir(work_sapce_dir)
work_sapce_dir = os.path.join(work_sapce_dir, "VOC2007\\")
if not os.path.isdir(work_sapce_dir):
    os.mkdir(work_sapce_dir)
annotation_dir = os.path.join(work_sapce_dir, "Annotations\\")
if not os.path.isdir(annotation_dir):
        os.mkdir(annotation_dir)
clear_hidden_files(annotation_dir)
image_dir = os.path.join(work_sapce_dir, "JPEGImages\\")
if not os.path.isdir(image_dir):
        os.mkdir(image_dir)
clear_hidden_files(image_dir)
VOC_file_dir = os.path.join(work_sapce_dir, "ImageSets\\")
if not os.path.isdir(VOC_file_dir):
        os.mkdir(VOC_file_dir)
VOC_file_dir = os.path.join(VOC_file_dir, "Main\\")
if not os.path.isdir(VOC_file_dir):
        os.mkdir(VOC_file_dir)

train_file = open(os.path.join(wd, "2007_train.txt"), 'w')
test_file = open(os.path.join(wd, "2007_test.txt"), 'w')
train_file.close()
test_file.close()
VOC_train_file = open(os.path.join(work_sapce_dir, "ImageSets\\Main\\train.txt"), 'w')
VOC_test_file = open(os.path.join(work_sapce_dir, "ImageSets\\Main\\test.txt"), 'w')
VOC_train_file.close()
VOC_test_file.close()
if not os.path.exists('VOCdevkit\\VOC2007\\labels'):
    os.makedirs('VOCdevkit\\VOC2007\\labels')
train_file = open(os.path.join(wd, "2007_train.txt"), 'a')
test_file = open(os.path.join(wd, "2007_test.txt"), 'a')
VOC_train_file = open(os.path.join(work_sapce_dir, "ImageSets\\Main\\train.txt"), 'a')
VOC_test_file = open(os.path.join(work_sapce_dir, "ImageSets\\Main\\test.txt"), 'a')
list = os.listdir(image_dir) # list image files
probo = random.randint(1, 100)
print("Probobility: %d" % probo)
for i in range(0,len(list)):
    path = os.path.join(image_dir,list[i])
    if os.path.isfile(path):
        image_path = image_dir + list[i]
        voc_path = list[i]
        (nameWithoutExtention, extention) = os.path.splitext(os.path.basename(image_path))
        (voc_nameWithoutExtention, voc_extention) = os.path.splitext(os.path.basename(voc_path))
        annotation_name = nameWithoutExtention + '.xml'
        annotation_path = os.path.join(annotation_dir, annotation_name)
    probo = random.randint(1, 100)
    print("Probobility: %d" % probo)
    if(probo < 75):
        if os.path.exists(annotation_path):
            train_file.write(image_path + '\n')
            VOC_train_file.write(voc_nameWithoutExtention + '\n')
            convert_annotation(nameWithoutExtention)
    else:
        if os.path.exists(annotation_path):
            test_file.write(image_path + '\n')
            VOC_test_file.write(voc_nameWithoutExtention + '\n')
            convert_annotation(nameWithoutExtention)
train_file.close()
test_file.close()
VOC_train_file.close()
VOC_test_file.close()
