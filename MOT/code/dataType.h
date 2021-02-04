#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

typedef struct mousepara
{
	cv::Mat frame = cv::Mat(1080, 1920, CV_8UC3);
	std::vector<cv::Rect> bboxes;
	bool onFlag = false;
};