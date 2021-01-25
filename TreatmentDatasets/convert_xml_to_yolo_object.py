# 将xml格式的数据转传成txt格式的文件，并将船只相关的类别合并，统一成boat；
# 对所有图片数据文件进行重命名，统一成coco12位的数据名；
import xml.etree.ElementTree as ET
import pickle
import os
from os import listdir, getcwd
from os.path import join

classes = ['boat', "pier"]

classes_count = ["Ferry", "Vessel/ship", "Speed boat", "Boat", "Kayak", "Sail boat"]
# classes = ['truck']


def convert(size, box):
    dw = 1. / (size[0])
    dh = 1. / (size[1])
    x = (box[0] + box[1]) / 2.0 - 1
    y = (box[2] + box[3]) / 2.0 - 1
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x * dw
    w = w * dw
    y = y * dh
    h = h * dh
    return (x, y, w, h)


def convert_annotation(image_id):
    in_file = open('./test_annotations/%s.xml' % (image_id), encoding="UTF-8")
    out_file = open('./test_labels/%s.txt' % (image_id), 'w', encoding="UTF-8")
    tree = ET.parse(in_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls = obj.find('name').text # 查找图片类别
        if(cls in classes_count):
            cls = "boat"
        print(cls)
        if (difficult == "Unspecified"):
            difficult = "0"
        if cls not in classes or int(difficult) == 1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text),
             float(xmlbox.find('ymax').text))
        bb = convert((w, h), b)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')


data_path = './test'
img_names = os.listdir(data_path)

list_file = open('./class_test.txt', 'w')
for img_name in img_names:
    if not os.path.exists('./test_labels'):
        os.makedirs('./test_labels')

    list_file.write('./test/%s\n' % img_name)
    # image_id = str(int(img_name[:-4])+5000)
    image_id = img_name[:-4]
    convert_annotation(image_id)

list_file.close()
