#pragma once
#include"motutil.h"
void inittracker_thread(cv::Tracker** tracker, cv::Mat frame, cv::Rect bbox);
void funMytracker(std::vector<cv::Ptr<cv::Tracker>>* tracker, cv::Mat* frame, std::vector<cv::Rect2d>* bbox, std::vector<bool>* ok_, int i);
int motThread();