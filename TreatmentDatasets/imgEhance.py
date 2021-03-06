#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# -----------------------------------------------------
# @Filename    :imgEhance.py
# @Notice    : 图片数据增强，使用imgaug数据增强库对当前图像进行数据增强。
#              imgaug库：https://github.com/aleju/imgaug
# @Time    :2021/01/25 13:59:22
# @Author    :sangwz
# @Version    :v1.0 能实现图像随机增强，想要添加其他图像增强方法，在图像增强的方法序列中添加imgaug
#                   中有的其他增强方法就行。
#              v1.1 添加其他格式的图片匹配，增加getEnds()函数，用来获取结尾为.png或者.jpeg的图片
#                   若要增加其他格式，再该函数中添加自己所要添加的文件后缀就行。
# ----------------------------------------------------


import xml.etree.ElementTree as ET
import pickle
import os
from os import getcwd
import numpy as np
from PIL import Image
import shutil
import matplotlib.pyplot as plt
 
import imgaug as ia
from imgaug import augmenters as iaa
from tqdm import tqdm
import random
 
import platform
if platform.system()=="Windows":
    splitchar = "\\"
elif platform.system() == "Linux":
    splitchar = "/"
ia.seed(1)
 
classes = ["boat", "pier"]

classes_count = ["Ferry", "Vessel/ship", "Speed boat", "Boat", "Kayak", "Sail boat"]


def getEnds(filenameFront):
    if os.path.exists(filenameFront + '.jpg'):
                strends = ".jpg"
    elif os.path.exists(filenameFront + '.jpeg'):
                strends = ".jpeg"
    elif os.path.exists(filenameFront + '.png'):
                strends = ".png"
    else:
        strends = "None"
    return strends


def mk_difficult_zero_change_class(root, image_id, saveroot = None): # 修改本地文件
    """
    针对原xml标注文件中的difficult标签以及name进行修改，
    将difficult非1的设置为0，SMD采集的数据集中的difficult为unXXXXX，
    name改为当前使用的boat，将smd中的船只类别进行合并
    :param root: [要修改的文件路径]
    :type root: [string]
    :param image_id: 文件名，也就是文件后缀名前面的内容
    :type image_id: [type]
    :param saveroot: 要保存的路径, defaults to None， 不指定的话就是修改原文件
    :type saveroot: [type], optional
    """
    in_file = open(os.path.join(root, str(image_id) + '.xml'), encoding='UTF-8')  # 这里root分别由两个意思
    tree = ET.parse(in_file)
    xmlroot = tree.getroot()
    objects = xmlroot.findall('object')
    index = 0
    for object in objects:
        bndbox = object.find('bndbox')
        xmin = bndbox.find('xmin')
        xmin.text = str(int(float(xmin.text)))
        ymin = bndbox.find('ymin')
        ymin.text = str(int(float(ymin.text)))
        xmax = bndbox.find('xmax')
        xmax.text = str(int(float(xmax.text)))
        ymax = bndbox.find('ymax')
        ymax.text = str(int(float(ymax.text)))
        difficult = object.find("difficult")
        if not (difficult.text == "1"):  # 将没有设置difficult的标签设置为0，因为部分数据中的该项是英文字符串，处理时可能会产生错误
            difficult.text = "0"
        name = object.find("name")  # 项目需要，将boat的子类别全部归类为boat类别，根据自己使用情况是否使用
        if name.text in classes_count:
            name.text = "boat"
        index += 1

        # print("index=", index)
    if(saveroot):
        tree.write(os.path.join(root, str("%06d" % (int(image_id))) + '.xml'))
    else:
        tree.write(os.path.join(root, str(image_id) + '.xml'))
    in_file.close()


def read_xml_annotation(root, image_id):
    in_file = open(os.path.join(root, image_id), encoding='UTF-8')
    tree = ET.parse(in_file)
    root = tree.getroot()
    bndboxlist = []
 
    for object in root.findall('object'):  # 找到root节点下的所有country节点
        bndbox = object.find('bndbox')  # 子节点下节点rank的值
 
        xmin = int(float(bndbox.find('xmin').text)) # 在项目中使用过smd数据集，生成的xml文件中的bbox是float
        xmax = int(float(bndbox.find('xmax').text)) # 此处多做了一步数据转换，提高适用性。
        ymin = int(float(bndbox.find('ymin').text))
        ymax = int(float(bndbox.find('ymax').text))
        # print(xmin,ymin,xmax,ymax)
        bndboxlist.append([xmin, ymin, xmax, ymax])
        # print(bndboxlist)
 
    # ndbox = root.find('object').find('bndbox')
    return bndboxlist
 
 
# (506.0000, 330.0000, 528.0000, 348.0000) -> (520.4747, 381.5080, 540.5596, 398.6603)
def change_xml_annotation(root, image_id, new_target):
    new_xmin = new_target[0]
    new_ymin = new_target[1]
    new_xmax = new_target[2]
    new_ymax = new_target[3]
 
    in_file = open(os.path.join(root, str(image_id) + '.xml'), encoding='UTF-8' )  # 这里root分别由两个意思
    tree = ET.parse(in_file)
    xmlroot = tree.getroot()
    object = xmlroot.find('object')
    bndbox = object.find('bndbox')
    xmin = bndbox.find('xmin')
    xmin.text = str(new_xmin)
    ymin = bndbox.find('ymin')
    ymin.text = str(new_ymin)
    xmax = bndbox.find('xmax')
    xmax.text = str(new_xmax)
    ymax = bndbox.find('ymax')
    ymax.text = str(new_ymax)
    tree.write(os.path.join(root, str("%06d" % (str(id) + '.xml'))))
 
 
def change_xml_list_annotation(root, image_id, new_target, saveroot, id):
    in_file = open(os.path.join(root, str(image_id) + '.xml'), encoding='UTF-8' )  # 这里root分别由两个意思
    tree = ET.parse(in_file)
    elem = tree.find('filename')
    cxla_strends = getEnds(os.path.join(root, str(image_id)))
    elem.text = (str("%06d" % int(id)) + cxla_strends)
    xmlroot = tree.getroot()
    index = 0
 
    for object in xmlroot.findall('object'):  # 找到root节点下的所有country节点
        bndbox = object.find('bndbox')  # 子节点下节点rank的值
 
        # xmin = int(bndbox.find('xmin').text)
        # xmax = int(bndbox.find('xmax').text)
        # ymin = int(bndbox.find('ymin').text)
        # ymax = int(bndbox.find('ymax').text)
 
        new_xmin = new_target[index][0]
        new_ymin = new_target[index][1]
        new_xmax = new_target[index][2]
        new_ymax = new_target[index][3]
 
        xmin = bndbox.find('xmin')
        xmin.text = str(new_xmin)
        ymin = bndbox.find('ymin')
        ymin.text = str(new_ymin)
        xmax = bndbox.find('xmax')
        xmax.text = str(new_xmax)
        ymax = bndbox.find('ymax')
        ymax.text = str(new_ymax)
        index += 1
        # print("index=",index)
 
    tree.write(os.path.join(saveroot, str("%06d" % int(id)) + '.xml'))
 
 
def mkdir(path):
    # 去除首位空格
    path = path.strip()
    # 去除尾部 \ 符号 ， linux系统下做相应的修改
    path = path.rstrip(splitchar)
    # 判断路径是否存在
    # 存在     True
    # 不存在   False
    isExists = os.path.exists(path)
    # 判断结果
    if not isExists:
        # 如果不存在则创建目录
        # 创建目录操作函数
        os.makedirs(path)
        print(path + ' 创建成功')
        return True
    else:
        # 如果目录存在则不创建，并提示目录已存在
        print(path + ' 目录已存在')
        return False




 
if __name__ == "__main__":
    
    treatmentPercent = 0.8 # 图像增强比例，在0,1之间
    AUGLOOP = 2  # 每张影像增强的数量
    recorrect = True
    # IMG_DIR = "F:\Datasets\VOCdevkit2\Init\JPEGImages"  # 原始图片数据文件路径
    IMG_DIR = "./newboat/img"  # 原始图片数据文件路径
    # XML_DIR = "F:\Datasets\VOCdevkit2\Init\Annotations"  # 原始xml数据文件路径
    XML_DIR = "./newboat/anotations"  # 原始xml数据文件路径
    AUG_XML_DIR = "./newboat/imgEnhance"  # 存储增强后的XML文件夹路径
    AUG_IMG_DIR = "./newboat/anotationsEnhance"  # 存储增强后的影像文件夹路径

    # 修改矫正xml文件中的标签信息
    if recorrect:
        print("正在处理原始标签信息：\n")
        for root, subdirectors, imageId in os.walk(XML_DIR):
            for imgid in tqdm(imageId):
                mk_difficult_zero_change_class(root, imgid[:-4])
        print("原始标签信息处理完毕！\n")
  
    if(os.path.exists(AUG_XML_DIR)):
        try:
            shutil.rmtree(AUG_XML_DIR)
        except FileNotFoundError as e:
            a = 1
    mkdir(AUG_XML_DIR)
     
    flag = os.path.exists(AUG_IMG_DIR)
    if(os.path.exists(AUG_IMG_DIR)):
        try:
            shutil.rmtree(AUG_IMG_DIR)
            # os.rmdir(AUG_IMG_DIR)
        except FileNotFoundError as e:
            a = 1
    mkdir(AUG_IMG_DIR)
 
    boxes_img_aug_list = []
    new_bndbox = []
    new_bndbox_list = []
    # 可以实现的图像增强方法；
    sequentialList = [
        # iaa.Flipud(0.5),  # vertically flip 20% of all images
        iaa.Fliplr(0.5),  # 镜像
        iaa.Sharpen(alpha=(0, 1.0), lightness=(0.75, 1.5)),
        iaa.Crop(px=(0, 16)),
        # iaa.Add((-30, 30), per_channel=0.5),
        iaa.Multiply((0.45, 1.5)),  # change brightness, doesn't affect BBs
        # iaa.Affine(
        #     # translate_px={"x": 15, "y": 15},
        #     scale=(0.8, 0.95),
        #     rotate = (-10, 10)
        #     # rotate=(-30, 30)
        # ), # translate by 40/60px on x/y axis, and scale to 50-70%, affects BBs
        iaa.GaussianBlur(),
        iaa.AddElementwise((-20, 20)),
        iaa.Dropout(p = (0, 0.1))
    ]

    seqLength = len(sequentialList)
    ''' 
    # 影像增强, 该部分代码会对所有的图像执行相同的操作， 可根据imgaug介绍做相应的选择，相关使用库中有详细介绍。
    seq = iaa.Sequential([
        # iaa.Flipud(0.5),  # vertically flip 20% of all images
        iaa.Fliplr(0.5),  # 镜像
        iaa.Sharpen(alpha=(0, 1.0), lightness=(0.75, 1.5)),
        iaa.Crop(px=(0, 16)),
        # iaa.Add((-30, 30), per_channel=0.5),
        iaa.Multiply((0.45, 1.5)),  # change brightness, doesn't affect BBs
        iaa.Affine(
            # translate_px={"x": 15, "y": 15},
            scale=(0.8, 0.95),
            rotate = (-10, 10)
            # rotate=(-30, 30)
        ), # translate by 40/60px on x/y axis, and scale to 50-70%, affects BBs
        iaa.GaussianBlur(),
        iaa.AddElementwise((-20, 20)),
        iaa.Dropout(p = (0, 0.1))
    ])
    '''

    print("正在进行图像增强：")
    for root, sub_folders, filesall in os.walk(XML_DIR):
        for name in filesall:
            shutil.copy(os.path.join(XML_DIR, name), AUG_XML_DIR) # 复制原xml文件
            # filenameFront, filenameEnd = getEnds(os.path.join(XML_DIR, name))
            strends = getEnds(os.path.join(IMG_DIR , name[:-4]))
            shutil.copy(os.path.join(IMG_DIR, name[:-4] + strends), AUG_IMG_DIR) # 复制原img文件
        filesNum = len(filesall) # 所有文件的长度
        if(filesNum == 0):
            print("没有要增强的文件")
            break
        if not (treatmentPercent == 1): # 对于全部处理，直接计算
            countToTreat = int(filesNum * treatmentPercent) # 计算要处理的数量    
            # 随机生成上面数量的索引列表
            indexList = random.sample(range(1, filesNum), countToTreat)
            # 生成要处理的文件列表
            files = [filesall[i] for i in indexList]
        else:
            files = filesall
        for name in tqdm(files):
            try:    
                bndbox = read_xml_annotation(XML_DIR, name)
                # print(os.path.join(IMG_DIR, name[:-4] + '.jpg'))
    
                for epoch in range(AUGLOOP):
                    # 随机生成增强序列
                    num = random.randint(0, 3)
                    seqindexList = [random.randint(0, seqLength-1) for _ in range(num)]
                    seq = iaa.Sequential([sequentialList[i] for i in seqindexList])
                    seq_det = seq.to_deterministic()  # 保持坐标和图像同步改变，而不是随机
                    # 读取图片
                    strends_ = getEnds(os.path.join(IMG_DIR, name[:-4]))
                    img = Image.open(os.path.join(IMG_DIR, name[:-4] + strends_))
                    # sp = img.size
                    img = np.asarray(img)
                    # 添加针对空图片的处理，即图像中没有标注信息时，对len(bndbox)的循环失效，里面的图片复制没有执行，但是外面的图像增强执行
                    if(len(bndbox) == 0):
                        # 存储变化后的图片
                        image_aug = seq_det.augment_images([img])[0]
                        # path = os.path.join(AUG_IMG_DIR,str("%06d" % (len(files) + int(name[:-4]) + (epoch+1) * 10000)) + '.jpg')
                        path = os.path.join(AUG_IMG_DIR, str("%06d" % (int(name[:-4]) + (epoch + 1) * 10000)) + strends_)

                        #image_auged = bbs.draw_on_image(image_aug, thickness=0)  ####################################

                        Image.fromarray(image_aug).save(path)
                    else:
                        # bndbox 坐标增强
                        for i in range(len(bndbox)):
                            bbs = ia.BoundingBoxesOnImage([
                                ia.BoundingBox(x1=bndbox[i][0], y1=bndbox[i][1], x2=bndbox[i][2], y2=bndbox[i][3]),
                            ], shape=img.shape)

                            bbs_aug = seq_det.augment_bounding_boxes([bbs])[0]
                            boxes_img_aug_list.append(bbs_aug)

                            # new_bndbox_list:[[x1,y1,x2,y2],...[],[]]
                            n_x1 = int(max(1, min(img.shape[1], bbs_aug.bounding_boxes[0].x1)))
                            n_y1 = int(max(1, min(img.shape[0], bbs_aug.bounding_boxes[0].y1)))
                            n_x2 = int(max(1, min(img.shape[1], bbs_aug.bounding_boxes[0].x2)))
                            n_y2 = int(max(1, min(img.shape[0], bbs_aug.bounding_boxes[0].y2)))
                            if n_x1 == 1 and n_x1 == n_x2:
                                n_x2 += 1
                            if n_y1 == 1 and n_y2 == n_y1:
                                n_y2 += 1
                            if n_x1 >= n_x2 or n_y1 >= n_y2:
                                print("\n")
                                print('error', name, "，%d epoch %d"%(epoch, i), "标签出现错误, ", n_x1, ",", n_y1)
                            new_bndbox_list.append([n_x1, n_y1, n_x2, n_y2])

                            # 存储变化后的图片
                            image_aug = seq_det.augment_images([img])[0]
                            # path = os.path.join(AUG_IMG_DIR,str("%06d" % (len(files) + int(name[:-4]) + (epoch+1) * 10000)) + '.jpg')
                            path = os.path.join(AUG_IMG_DIR,str("%06d" % (int(name[:-4]) + (epoch+1) * 10000)) + strends_)

                            image_auged = bbs.draw_on_image(image_aug, thickness=0)####################################

                            Image.fromarray(image_auged).save(path)
    
                    # 存储变化后的XML
                    # change_xml_list_annotation(XML_DIR, name[:-4], new_bndbox_list, AUG_XML_DIR,len(files) + int(name[:-4]) + epoch * 10000)
                    change_xml_list_annotation(XML_DIR, name[:-4], new_bndbox_list, AUG_XML_DIR, int(name[:-4]) + (epoch+1) * 10000)
                    # print(str("%06d" % (len(files) + int(name[:-4]) + epoch * 10000)) + '.jpg')
                    # print(str("%06d" % (int(name[:-4]) + (epoch+1) * 10000)) + '.jpg')
                    new_bndbox_list = []
            except Exception as e: 
                # print("发生错误：", e)
                raise e
    print("处理完毕")

    # os.system("pause")