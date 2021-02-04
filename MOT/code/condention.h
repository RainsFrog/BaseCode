#pragma once
#include <opencv2/opencv.hpp>

/****定义粒子结构体****/

typedef struct particle

{

	int orix, oriy;//原始粒子坐标

	int x, y;//当前粒子的坐标

	double scale;//当前粒子窗口的尺寸

	int prex, prey;//上一帧粒子的坐标

	double prescale;//上一帧粒子窗口的尺寸

	cv::Rect rect;//当前粒子矩形窗口

	cv::Mat hist;//当前粒子窗口直方图特征

	double weight;//当前粒子权值 

}PARTICLE;