# 检查文件名称和标签名称是否一一对应，即对比文件夹中的文件数量以及位置顺序是否相同
import os
#import system

_filepath_ = os.path.split(os.path.realpath(__file__))[0]
pic_lists = os.listdir(_filepath_ + "./JPEGImages/")
label_lists = os.listdir(_filepath_ + "./Annotations//")
length = len(pic_lists)
index = 1
count = 0
while index < length:
    if pic_lists[index].strip().split(".")[0] != label_lists[index].strip().split(".")[0]:
        print("第%s个文件名不对应"%index)
        print("")
        count += 1
    index += 1
if count == 0:
    print("所有文件正常匹配，没有发生错乱！！")

os.system("pause")