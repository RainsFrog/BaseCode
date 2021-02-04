#pragma once
#include "motutil.h"
#include <thread>
#include<Windows.h>
#include<time.h>
#include"MouseEvent.h"

bool pushFlag = false;
std::vector<cv::Rect> global_bboxes;


typedef struct mousepara
{
	cv::Mat frame = cv::Mat(1080, 1920, CV_8UC3);
	std::vector<cv::Rect> bboxes;
	bool onFlag = false;
};

extern std::vector<std::string> trackerTypes;
// Ŀ����ٿ��ʼ�����򣬶������еĸ���bbox������������
void inittracker_thread(cv::Tracker** tracker, cv::Mat frame, cv::Rect bbox)
{
	(*tracker)->init(frame, bbox);
}
void subtrack(cv::Tracker *tracker, cv::Mat *frame, cv::Rect2d bbox) // 
{
	bool ok = tracker->update(*frame, bbox);
}
void funMytracker(std::vector<cv::Ptr<cv::Tracker>>* tracker, cv::Mat* frame, std::vector<cv::Rect2d>* bbox, std::vector<bool>* ok_, int i) 
{
	(*ok_)[i] = (*tracker)[i]->update(*frame, (*bbox)[i]);
}
void funMytracker_erase_(std::vector<cv::Ptr<cv::Tracker>>* tracker, std::vector<cv::Rect2d>* bbox, std::vector<bool>* ok_CSRT, std::vector<clock_t>* Start_time_record, std::vector<double>* fish_distance, std::vector<cv::Rect2d>* ptr_bboxCSRT, int i) {
	(*tracker).erase((*tracker).begin() + i);
	(*bbox).erase((*bbox).begin() + i);
	(*ok_CSRT).erase((*ok_CSRT).begin() + i);
	(*Start_time_record).erase((*Start_time_record).begin() + i);
	(*fish_distance).erase((*fish_distance).begin() + i);
	(*ptr_bboxCSRT).erase((*ptr_bboxCSRT).begin() + i);
}

void initTracker(std::vector<cv::Rect>& bboxes_, std::vector<cv::Rect2d>& bboxes,
	                cv::Mat& frame, std::vector<cv::Ptr<cv::Tracker>>& Mytracker, 
	                std::vector<bool>& OK_, bool OK_0)
{
	for (int i = 0; i < bboxes_.size();i++)
	{
		bboxes.push_back(cv::Rect2d(bboxes_[i].x, bboxes_[i].y, bboxes_[i].width, bboxes_[i].height));
		cv::Ptr<cv::Tracker> tracker_0 = cv::TrackerKCF::create();
		tracker_0->init(frame, bboxes[i]);
		Mytracker.push_back(tracker_0);
		OK_.push_back(OK_0);// �Ƿ����
	}
}


int motThread()
{
	// video path
	bool OK_0 = false;
	bool slectRoi = false;
	// std::string videopath = "3.avi";
	std::string videopath = "rtsp://admin:zhhh6052@10.0.105.103:554/cam/realmonitor?channel=1&subtype=0";
	bool showCrosshair = true;
	bool fromCenter = false;
	// ��ʼ����Ŀ����ٿ�
	std::vector<cv::Rect2d> bboxes;
	std::vector<cv::Rect> bboxes_;
	std::vector<cv::Rect> bboxes2;


	cv::VideoCapture cap(videopath);
	cv::Mat frame, frame_;
	// �޷���ȡ��ô�˳�
	if (!cap.isOpened())
	{
		std::cout << "Error opening video file" << videopath << std::endl;
		return -1;
	}
	bool bReadRet = cap.read(frame_);
	if (!bReadRet)
	{
		std::cout << " ��ȡ��Ƶ���� " << std::endl;
		return -1;
	}

	std::cout << "\n==========================================================\n";
	std::cout << "OpenCV says press c to cancel objects selection process" << std::endl;
	std::cout << "It doesn't work. Press Escape to exit selection process" << std::endl;
	std::cout << "\n==========================================================\n";
	cv::resize(frame_, frame, cv::Size(), 0.4, 0.4);
	cv::selectROIs("MultiTracker", frame, bboxes_, showCrosshair, fromCenter);

	if (bboxes_.size() == 0)
	{
		return 0;
	}
	int trackerCount = bboxes_.size(); // ��ʼ���ٿ����
	std::vector<cv::Scalar> colors;
	getRandomColors(colors, bboxes_.size());
	//******************************************
	std::vector<std::thread> TrackerThread_;
	std::vector<cv::Ptr<cv::Tracker>> Mytracker;
	std::vector<bool> OK_;
	//*******************************************
	// init the tracker ;
	std::string trackerType = "KCF";
	// ����Ŀ������

	// ��ʼ����Ŀ�������,�����Ŀ����������뵽vector�С�Init MOT trackers, push few MOT-tracker into Vector.
	//for (int i = 0; i < bboxes_.size();i++)
	//{
	//	bboxes.push_back(cv::Rect2d(bboxes_[i].x, bboxes_[i].y, bboxes_[i].width, bboxes_[i].height));
	//	cv::Ptr<cv::Tracker> tracker_0 = cv::TrackerKCF::create();
	//	tracker_0->init(frame, bboxes[i]);
	//	Mytracker.push_back(tracker_0);
	//	OK_.push_back(OK_0);// �Ƿ����
	//}
	initTracker(bboxes_, bboxes, frame, Mytracker, OK_, OK_0);
	while (cap.isOpened())
	{
		if (!cap.read(frame))
		{
			std::cout << "Video is ended!" << std::endl;
			return 0;
		}
		if (frame.empty())
			break;
		// The biger size frame needs more source of cpu, it will be faster if resize the frame into a smaller size.
		cv::resize(frame, frame, cv::Size(), 0.4, 0.4);
		clock_t startTime = clock();
		for (int i = 0; i < Mytracker.size(); i++)
		{
			TrackerThread_.push_back(std::thread(funMytracker, &Mytracker, &frame, &bboxes, &OK_, i));
			// TrackerThread_.push_back(std::thread(testThread, &Mytracker, &frame, &bboxes));
		}
		for (auto& ite : TrackerThread_)
		{
			ite.join();
		}
		TrackerThread_.clear();
		// show the tracker bbox.
		for (unsigned i = 0;i< Mytracker.size(); i++)
		{
			cv::rectangle(frame, bboxes[i], colors[i], 2, 1);
		}
		// �������¼�
		/*
		���� �˴�����ʹ��ָ�봫�Σ���������¼������ı�ֵ��δ�ɹ�����Ϊȫ�ֱ���ʵ�֡�
		���� TODO�� ���������Ƿ���ʵ�֡�
		*/
		// structMousePara_.onFlag = false;
		// std::vector<>
		// mousepara * structMousePara = new mousepara(1096*1080+ sizeof(cv::Rect)*20);
		mousepara structMousePara_;
		structMousePara_.frame = frame;
		mousepara *structMousePara = &structMousePara_;
		clock_t endTime = clock();
		double usingtime = double(endTime - startTime)/ CLOCKS_PER_SEC;
		char ctime[20];
		sprintf(ctime, "time:%0.4f, fps:%0.2f", usingtime, 1/usingtime);
		cv::putText(frame, ctime, cv::Point(frame.size().width - 600, 90), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 125, 0), 1);
		imshow("MultiTracker", frame);
		cv::setMouseCallback("MultiTracker", MouseEvent, structMousePara);
		
		// if (structMousePara->onFlag == true)
		Sleep(1);
		if (pushFlag == true)
		{
			for (auto bbox : global_bboxes)
			//for (auto bbox : structMousePara->bboxes)
			{
				bboxes_.push_back(bbox);
				cv::Ptr<cv::Tracker> tracker_add = cv::TrackerKCF::create(); // init the new track.
				tracker_add->init(frame, cv::Rect2d(bbox.x, bbox.y, bbox.width, bbox.height));
				Mytracker.push_back(tracker_add);
				OK_.push_back(OK_0);
			}
			//bboxes.swap(std::vector<cv::Rect2d>()); // 
			structMousePara->onFlag = false;
			pushFlag = false;
			global_bboxes.swap(std::vector<cv::Rect>());
		}
		//delete structMousePara;
		structMousePara = NULL;

		// �����˳�
		if (cv::waitKey(1) == 27)
			break;
	}


}



