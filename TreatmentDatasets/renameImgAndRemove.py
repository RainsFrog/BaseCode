"""
该函数代码将标注完成的图片数据进行整理，删除未标注的图片。在标注时，有部分图片质量不符合预期，没有
进行标注，将该部分图片数据进行删除处理，保留合格的图片。

:raises Exception: [description]
"""

import os
import time
# 获取annotion文件夹下的所有文件名，
def removeUnsuiltImg(anopath, imgpath):
    """
    docstring
    """
    
    # anopath = os.getcwd() + "\\newboat"+"\\anotations\\" 
    filenameList = os.listdir(anopath)
    filenameFront = [file.split(".")[0] for file in  filenameList] # annotions 前缀

    # imgpath = os.getcwd() + "\\newboat"+"\\img\\"
    # print(imgpath)
    imgnameList = os.listdir(imgpath)
    filenameImgFront = [file.split(".")[0] for file in imgnameList]
    # print(filenameImgFront)
    for fileFront in filenameImgFront:
        if fileFront not in filenameFront:
            filepath = imgpath + fileFront
            if os.path.exists(filepath + ".jpg"):
                os.remove(imgpath + fileFront+".jpg")
                print(imgpath + fileFront+".jpg" + " removed")
            if os.path.exists(filepath + ".jpeg"):
                os.remove(imgpath + ".jpeg")
                print(imgpath + fileFront+".jpeg" + " removed")
            else:
                print(filepath + "文件不存在！")
                raise Exception ("文件不存在")
    print("Finish check and remove!")

def renameTheFile(imgpath, anotationpath):
    """
    对于标注完成后的图片进行重新命名处理，
    新文件名长度12位的数字组成，前8位设置为当前日期，后四位设置为编号
    :param imgpath: 图片存放路径
    :type imgpath: string
    :param anotationpath: 标注文件存放路径
    :type anotationpath: string
    """
    # 获取当前日期并设置成连续的8位数字
    timeNow = time.localtime() # time.struct_time(tm_year=2021, tm_mon=1, tm_mday=4, tm_hour=9, tm_min=3, tm_sec=44, tm_wday=0, tm_yday=4, tm_isdst=0)
    # timestring = time.strftime("%Y%m%d",timeNow) #'20210104'
    timestring = "20210105"
    filenameList = os.listdir(anotationpath) # ["xxx.xml", "xxx.xml", .....]

    # 获取名称前缀，并修改当前以及图片文件夹下的文件名称
    for i, xmlfilename in enumerate(filenameList):
        jpgorjpeg = 0
        filenameFront = xmlfilename.split(".")[0]
        if os.path.exists(anotationpath + xmlfilename):
            if os.path.exists(imgpath + filenameFront + ".jpg"):
                os.rename(anotationpath + xmlfilename, anotationpath + timestring + "{:0>4d}.xml".format(i))
                os.rename(imgpath + filenameFront + ".jpg", imgpath + timestring + "{:0>4d}.jpg".format(i))
                                
            if os.path.exists(imgpath + filenameFront + ".jpeg"):
                os.rename(anotationpath + xmlfilename, anotationpath + timestring + "{:0>4d}.xml".format(i))
                os.rename(imgpath + filenameFront + ".jpeg", imgpath + timestring + "{:0>4d}.jpeg".format(i))
    
    print("Finish check and rename!")



if __name__ == "__main__":
    # anopath = os.getcwd() + "\\newboat"+"\\anotations\\"
    anopath = os.getcwd() + "\\VOCdevkit\\VOC2007\\Annotations\\"
    print(anopath)
    imgpath = os.getcwd() + "\\VOCdevkit\\VOC2007\\JPEGImages\\"
    removeUnsuiltImg(anopath, imgpath)
    renameTheFile(imgpath, anopath)
    removeUnsuiltImg(anopath,imgpath)
