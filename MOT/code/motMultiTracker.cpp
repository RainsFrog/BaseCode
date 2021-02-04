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
	// ��ʼ����Ŀ����ٿ�
	std::vector<cv::Rect> bboxes;
	

	// ����capture object ,��ȡ��Ƶ
	cv::VideoCapture cap(videopath);
	cv::Mat frame;
	cv::cuda::GpuMat gpuframe;
	// �޷���ȡ��ô�˳�
	if (!cap.isOpened())
	{
		std::cout << "Error opening video file" << videopath << std::endl;
		return -1;
	}
	bool bReadRet = cap.read(frame);
	
	if (!bReadRet)
	{
		std::cout << " ��ȡ��Ƶ���� " << std::endl;
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
	// ��ʼ��������
	std::string trackerType = "KCF";
	// ������Ŀ�������
	cv::Ptr<cv::MultiTracker> multiTracker = cv::MultiTracker::create();
	// ��ʼ����Ŀ�������
	for (int i = 0; i < bboxes.size();i++)
		multiTracker->add(createTrackerByName(trackerType), frame, cv::Rect2d(bboxes[i]));
	while (cap.isOpened())
	{
		if (!cap.read(frame))
		{
			std::cout << "��Ƶ��ȡ����" << std::endl;
			return 0;
		}
		if (frame.empty())
			break;
		// �µ�һ֡����Ŀ����ٿ�
		multiTracker->update(frame);
		// ���Ƹ���Ŀ�����
		for (unsigned i = 0;i< multiTracker->getObjects().size(); i++)
		{
			cv::rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);
		}
		// show frame
		imshow("MultiTracker", frame);
		
		// �����˳�
		if (cv::waitKey(1) == 27)
			break;
	}


}
