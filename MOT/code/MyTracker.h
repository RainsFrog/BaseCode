#pragma once
#include<iostream>
#include <string>
// #include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

class MyTracker
{
public:
	std::string ctrackerTypes[10] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" , "SORT", "DEEPSORT"}; // 目标跟踪的可选类型
	std::vector<cv::Rect> bboxes;

	MyTracker();
	~MyTracker();
};

