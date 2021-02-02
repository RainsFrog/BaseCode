'''
作用：将文件夹下的所有图片筛选出来，并复制到指定文件夹。
'''

import os
import shutil

beforedir = os.getcwd()
int count = 0 

def dir(way):
    picture = ["jpg", "jpeg"]
    for root, dirs, files in os.walk(way):
        for dirname in dirs:
            dir(dirname)
        for Filename in files:
            path = os.path.join(root, Filename)
            size = os.path.getsize(path)
            if((Filename[-3:] in picture and Filename[0] != "$")):
                shutil.copy(path, "F:/Datasets/newboat/img/{}".format(Filename))

dir(beforedir)

        