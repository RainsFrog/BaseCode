#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# -----------------------------------------------------
# @Filename    :testTheConvertResult.py
# @Notice    :代码用来测试生成的检测结果是否正确，使用txt文件
              #进行验证.当然也可以使用labelImg标注工具进行查看。
# @Time    :2021/01/21 14:59:43
# @Author    :sangwz
# @Version    :v1.0
# ----------------------------------------------------

from os import X_OK
import matplotlib.patches as patches
import cv2
import matplotlib as plt
import os
import random

def validBoundbox(imagepath, bboxpath):
    """
    docstring
    """
    img = cv2.imread(imagepath)
    print(img.shape)
    height = img.shape[0]
    width = img.shape[1]
    objectFind = []
    with open(bboxpath) as bboxreader:
        lines = bboxreader.readlines()
    for line in lines:
        line_ = line.strip().split(" ")
        objectFind.append(line_)
        print(line_)
        xcenter, ycenter, bwidth, bheight = float(line_[1]), float(line_[2]), float(line_[3]), float(line_[4])
        print(xcenter, ycenter, bwidth, bheight)
        xleft = xcenter - bwidth/2
        xleft = 0 if xleft < 0 else xleft
        yleft = ycenter - bheight/2
        yleft = 0 if yleft < 0 else yleft
        xright = xcenter + bwidth/2
        yright = ycenter + bheight/2

        xleft *= width
        xright *= width
        yleft *= height
        yright *= height

        if xleft <= 0:
            xleft = 1
        if yleft <= 0:
            yleft = 1
        #print("========= 框 ============")
        #print(xleft, xright, yleft, yright)
        # print(pt1, pt2)
        img = cv2.rectangle(img, (int(xleft), int(yleft)), (int(xright), int(yright)), (255, 0, 0), 1)

    cv2.imshow(imgpath, img)
    cv2.waitKey(0)
    cv2.destroyWindow(imagepath)


# imagePath = "F:\\Datasets\\Singapore_Maritime_Dataset\\train\\MVI_1613_VIS_frame0.jpg"
# imagePath = "VOC2007\\JPEGImages\\202101216870.jpg"
# bboxPath = "F:\\Datasets\\Singapore_Maritime_Dataset\\train_labels\\MVI_1613_VIS_frame0.txt"
# bboxPath = "VOC2007\\labels\\202101216870.txt"
# validBoundbox(imagePath, bboxPath)
def random_int_list(start, stop, length):
    start, stop = (int(start), int(stop)) if start <= stop else (int(stop), int(start))
    length = int(abs(length)) if length else 0
    random_list = []
    for i in range(length):
        random_list.append(random.randint(start, stop))
    return random_list

if __name__ == "__main__":
    # 获取所有图片文件
    imgFiles = "VOC2007\\JPEGImages\\"
    labelFiles = "VOC2007\\labels\\"
    imgLists = os.listdir(imgFiles) 
    numofimg = len(imgLists)
    randomImgNum = random_int_list(0, numofimg, 20)
    # 此处图片和label应该是一一对应的，可以直接列出两个文件夹的所有文件然后按照上面生成的随机数
    # 索引提取两个对应的文件显示即可。但是为了避免其他因素干扰，以提取出来的图片的id来查找对应的
    # label的id

    # 此处随机生成20张图片进行查看，可以根据自己需要进行调整。减少或增加或全部查看标注文件。
    for i in randomImgNum:
        imgId = imgLists[i].split(".")[0] # 图片id；
        imgpath = imgFiles + imgLists[i] # 图片路径
        labelpath = labelFiles + imgId + ".txt" # label路径
        validBoundbox(imgpath, labelpath)
        

