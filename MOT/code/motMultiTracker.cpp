#pragma once
#include"motutil.h"
#include<vector>
#include<opencv2/core/cuda.hpp>

extern std::vector<std::string> trackerTypes;

int funcMultiTrack()
{
	// video path
	std::string videopath = "rtsp://admin:zhhh6052@10.0.105.101:554/cam/realmonitor?channel=1&subtype=0";
	// 
	bool showCrosshair = true;
	bool fromCenter = false;
	// 初始化多目标跟踪框
	std::vector<cv::Rect> bboxes;
	

	// 创建capture object ,读取视频
	cv::VideoCapture cap(videopath);
	cv::Mat frame;
	cv::cuda::GpuMat gpuframe;
	// 无法读取那么退出
	if (!cap.isOpened())
	{
		std::cout << "Error opening video file" << videopath << std::endl;
		return -1;
	}
	bool bReadRet = cap.read(frame);
	
	if (!bReadRet)
	{
		std::cout << " 读取视频错误 " << std::endl;
		return -1;
	}

	std::cout << "\n==========================================================\n";
	std::cout << "OpenCV says press c to cancel objects selection process" << std::endl;
	std::cout << "It doesn't work. Press Escape to exit selection process" << std::endl;
	std::cout << "\n==========================================================\n";
	
	cv::selectROIs("MultiTracker", frame, bboxes, showCrosshair, fromCenter);
	if (bboxes.size() == 0)
	{
		return 0;
	}
	// gpuframe.upload(frame);

	std::vector<cv::Scalar> colors;
	getRandomColors(colors, bboxes.size());
	// 初始化跟踪器
	std::string trackerType = "KCF";
	// 创建多目标跟踪器
	cv::Ptr<cv::MultiTracker> multiTracker = cv::MultiTracker::create();
	// 初始化多目标跟踪器
	for (int i = 0; i < bboxes.size();i++)
		multiTracker->add(createTrackerByName(trackerType), frame, cv::Rect2d(bboxes[i]));
	while (cap.isOpened())
	{
		if (!cap.read(frame))
		{
			std::cout << "视频读取结束" << std::endl;
			return 0;
		}
		if (frame.empty())
			break;
		// 新的一帧更新目标跟踪框
		multiTracker->update(frame);
		// 绘制跟踪目标检测框
		for (unsigned i = 0;i< multiTracker->getObjects().size(); i++)
		{
			cv::rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);
		}
		// show frame
		imshow("MultiTracker", frame);
		
		// 按键退出
		if (cv::waitKey(1) == 27)
			break;
	}


}
