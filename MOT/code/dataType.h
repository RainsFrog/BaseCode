#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

typedef struct mousepara
{
	cv::Rect arrayBox[2];
	cv::Mat frame = cv::Mat(1080, 1920, CV_8UC3);
	std::vector<cv::Rect> bboxes;
	bool onFlag = false;
};

extern std::vector<cv::Rect> rectStore;
extern std::vector<cv::Rect> global_bboxes;

extern bool pushFlag;