//#pragma once
//#include <thread>
//#include<iostream>
//#include<string>
//#define THREADNUM 5
//
//// 要执行的线程函数
//int sayHello(int a)
//{
//	std::cout << "hello world--thread " << a << std::endl;
//	return 0;
//}
//int main0(int argc, char* argv)
//{
//	std::thread lThread[THREADNUM];
//	for (int i = 0; i < THREADNUM; ++i)
//	{
//		/* code */
//		std::thread hread(sayHello, i);
//		hread.join();
//		
//
//	}
//	return 0;
//}
//
//
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <math.h>
//#include <thread>
//#include <opencv2/tracking.hpp>
//#include <opencv2/core/ocl.hpp>
//using namespace cv;
//using namespace std;
//#define  COLS 960
//#define  ROWS 540
//#define  FN 10//时序中值的图像数目
//Mat matRotateClockWise180(Mat src)//顺时针180
//{
//	flip(src, src, 0);
//	flip(src, src, 1);
//	return src;
//}
//void BaseLine_Generator_1(Mat* BaseLine, vector<Mat>* vMat)
//{
//	for (int i = 0; i < ROWS; ++i)
//	{
//		for (int j = 0; j < COLS; ++j)
//		{
//			uchar Mid_value_[FN];
//			for (int k = 0; k < FN; k++) {
//				Mid_value_[k] = (*vMat)[k].at<Vec3b>(i, j)[0];
//			}
//			sort(Mid_value_, Mid_value_ + FN);
//			BaseLine->at<Vec3b>(i, j)[0] = Mid_value_[FN / 2];
//			for (int k = 0; k < FN; k++) {
//				Mid_value_[k] = (*vMat)[k].at<Vec3b>(i, j)[1];
//			}
//			sort(Mid_value_, Mid_value_ + FN);
//			BaseLine->at<Vec3b>(i, j)[1] = Mid_value_[FN / 2];
//			for (int k = 0; k < FN; k++) {
//				Mid_value_[k] = (*vMat)[k].at<Vec3b>(i, j)[2];
//			}
//			sort(Mid_value_, Mid_value_ + FN);
//			BaseLine->at<Vec3b>(i, j)[2] = Mid_value_[FN / 2];
//		}
//	}
//}
//float RectOverlap(cv::Rect r, cv::Rect rOther)
//{
//	int x0 = std::max(r.x, rOther.x);
//	int x1 = std::min(r.x + r.width, rOther.x + rOther.width);
//	int y0 = std::max(r.y, rOther.y);
//	int y1 = std::min(r.y + r.height, rOther.y + rOther.height);
//	if (x0 >= x1 || y0 >= y1) return 0.f;
//	float areaInt = (x1 - x0) * (y1 - y0);
//	float MINArea = MIN((float)r.width * r.height, (float)rOther.width * rOther.height);
//	return areaInt / MINArea;
//}
////删除跟踪器
//void CSRT_tracker(vector<Ptr<Tracker>>* tracker, Mat* frame, vector<Rect2d>* bbox, vector<bool>* ok_CSRT, int i) {
//	(*ok_CSRT)[i] = (*tracker)[i]->update(*frame, (*bbox)[i]);
//}
//void CSRT_tracker_erase_(vector<Ptr<Tracker>>* tracker, vector<Rect2d>* bbox, vector<bool>* ok_CSRT, vector<clock_t>* Start_time_record, vector<double>* fish_distance, vector<Rect2d>* ptr_bboxCSRT, int i) {
//	(*tracker).erase((*tracker).begin() + i);
//	(*bbox).erase((*bbox).begin() + i);
//	(*ok_CSRT).erase((*ok_CSRT).begin() + i);
//	(*Start_time_record).erase((*Start_time_record).begin() + i);
//	(*fish_distance).erase((*fish_distance).begin() + i);
//	(*ptr_bboxCSRT).erase((*ptr_bboxCSRT).begin() + i);
//}
//int main4(int argc, char** argv)
//{
//	Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
//	bgsubtractor->setVarThreshold(20);
//	VideoCapture video("C:/testv/5.mp4");
//	if (!video.isOpened())
//	{
//		cout << "Could not read video file" << endl;
//		return 1;
//	}
//	Mat frame;
//	Mat src;
//	vector<Mat> Filtering_Mat;
//	//*******************8
//	vector<thread> TrackerThread_;
//	vector<Ptr<Tracker>> trackerCSRT;
//	vector<Rect2d> bboxCSRT;
//	vector<double> fish_distance;
//	vector<Rect2d> ptr_bboxCSRT;
//	Rect2d bbox;
//	bool sign = false;
//	vector<bool> OK_;
//	vector<clock_t> Start_time_record;
//	bool OK_0 = false;
//	long int fishnum = 0;
//	///
//	for (int i = 0; i < FN; i++) {
//		Mat src1;
//		video.read(frame);
//		resize(frame, src1, Size(960, 540));
//		src1 = matRotateClockWise180(src1);
//		imshow("check", src1);
//		waitKey(100);
//		Filtering_Mat.push_back(src1);
//	}
//	while (video.read(frame)) {
//		resize(frame, src, Size(960, 540));
//		src = matRotateClockWise180(src);
//		imshow("src", src);
//		Mat filter_src1 = src.clone();
//		Filtering_Mat.erase(Filtering_Mat.begin());
//		Filtering_Mat.push_back(filter_src1);
//		Mat filter_src = src.clone();
//		BaseLine_Generator_1(&filter_src, &Filtering_Mat);
//		imshow("Final result", filter_src);
//		Mat mask;
//		Mat Gfilter_src;
//		cvtColor(filter_src, Gfilter_src, CV_RGB2GRAY);
//		bgsubtractor->apply(filter_src, mask, 0.01);
//		imshow("GMask", mask);
//		std::vector<std::vector<cv::Point> > cnts;
//		Mat kernel = getStructuringElement(MORPH_CROSS, Size(13, 13), Point(-1, -1));
//		cv::dilate(mask, mask, kernel);
//		Mat kernel1 = getStructuringElement(MORPH_CROSS, Size(12, 12), Point(-1, -1));
//		cv::erode(mask, mask, kernel1);
//		cv::threshold(mask, mask, 128, 255, THRESH_BINARY);
//		imshow("GMask1", mask);
//		findContours(mask, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//		float Area;
//		Rect rect;
//		vector<Point> m;
//		for (int i = cnts.size() - 1; i >= 0; i--)
//		{
//			vector<Point> c = cnts[i];
//			Area = contourArea(c);
//			if (Area < 1600)//目标的大小
//			{
//				continue;
//			}
//			else
//			{
//				m = c;
//			}
//			rect = boundingRect(m);
//			//高斯背景求的前景目标比例限制，可根据需要调整
//			if (double(MAX(rect.width, rect.height)) / double(MIN(rect.width, rect.height)) <1.5) continue;
//			if (double(MAX(rect.width, rect.height)) / double(MIN(rect.width, rect.height)) > 3) continue;
//			if ((rect.width + rect.height)>400)continue;
//			// Rect rect_track = rect;
//			double cx = double(rect.x + double(rect.width) / 2);
//			double cy = double(rect.y + double(rect.height) / 2);
//			bool Needcontinue = false;
//			for (int kk = 0;kk<bboxCSRT.size();kk++)
//			{
//				//与已有的目标中心点曼哈顿距离小于100
//				if ((abs((bboxCSRT[kk].x + bboxCSRT[kk].width / 2) - cx) + abs((bboxCSRT[kk].y + bboxCSRT[kk].height / 2) - cy)) < 100)
//				{
//					Needcontinue = true;
//					continue;
//				}
//				//于已有框体交叉面积大于20%
//				float verlap_ = RectOverlap(rect, bboxCSRT[kk]);
//				if (verlap_ >0.2)
//				{
//					Needcontinue = true;
//					continue;
//				}
//			}
//			if (Needcontinue) {
//				continue;
//			}
//			Ptr<Tracker > tracker_0;
//			tracker_0 = TrackerCSRT::create();
//			bbox = rect;
//			rectangle(filter_src, bbox, Scalar(255, 0, 0), 2, 1);
//			bboxCSRT.push_back(bbox);
//			tracker_0->init(src, bbox);
//			trackerCSRT.push_back(tracker_0);
//			OK_.push_back(OK_0);
//			clock_t start_time = clock();
//			double zero_ = 0;
//			fish_distance.push_back(zero_);
//			ptr_bboxCSRT.push_back(bbox);
//			Start_time_record.push_back(start_time);
//		}
//		imshow("CSRT Tracking src", filter_src);
//		int k1 = waitKey(1);
//		Mat tracksrc;
//		bilateralFilter(src, tracksrc, 5, 2, 2);
//		if (OK_.size() > 0) {
//			double timer = (double)getTickCount();
//			for (int i = 1; i < trackerCSRT.size(); i++) {
//				TrackerThread_.push_back(thread(CSRT_tracker, &trackerCSRT, &tracksrc, &bboxCSRT, &OK_, i));
//			}
//			OK_[0] = trackerCSRT[0]->update(tracksrc, bboxCSRT[0]);
//			for (auto& ite : TrackerThread_) {
//				ite.join();
//			}
//			TrackerThread_.clear();
//			float fps = getTickFrequency() / ((double)getTickCount() - timer);
//			Mat frameCSRT = tracksrc.clone();
//			for (int i = 0; i < trackerCSRT.size(); i++) {
//				if (OK_[i])
//				{
//					rectangle(frameCSRT, bboxCSRT[i], Scalar(0, 255, 255), 2, 1);
//				}
//				else {
//					putText(frameCSRT, "Tracking failure detected" + to_string(i), Point(100, 80 + i * 10), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//				}
//				double x_b;
//				double y_b;//框体的右下点坐标
//				x_b = bboxCSRT[i].x + bboxCSRT[i].width;
//				y_b = bboxCSRT[i].y + bboxCSRT[i].height;
//				string str;
//				clock_t time_out = clock() - Start_time_record[i];
//				double cx0 = double(bboxCSRT[i].x + double(bboxCSRT[i].width) / 2);
//				double cy0 = double(bboxCSRT[i].y + double(bboxCSRT[i].height) / 2);
//				double cx1 = double(ptr_bboxCSRT[i].x + double(ptr_bboxCSRT[i].width) / 2);
//				double cy1 = double(ptr_bboxCSRT[i].y + double(ptr_bboxCSRT[i].height) / 2);
//				fish_distance[i] = fish_distance[i] + abs(cx0 - cx1) + abs(cy0 - cy1);
//				ptr_bboxCSRT[i].x = bboxCSRT[i].x;
//				ptr_bboxCSRT[i].y = bboxCSRT[i].y;
//				ptr_bboxCSRT[i].width = bboxCSRT[i].width;
//				ptr_bboxCSRT[i].height = bboxCSRT[i].height;
//				bool dis_time = false;
//				dis_time = ((time_out / CLOCKS_PER_SEC) > 30) && (fish_distance[i] < 300);//出现在场景中的时间超过30秒且目标的距离小于300
//																						  //目标越界，且走过de的距离大于400，删除目标，计数+1
//				if ((bboxCSRT[i].x < 0 || bboxCSRT[i].y < 0 || x_b >960 || y_b>540) && (fish_distance[i] > 400)) {
//					CSRT_tracker_erase_(&trackerCSRT, &bboxCSRT, &OK_, &Start_time_record, &fish_distance, &ptr_bboxCSRT, i);
//					str = "Find Fish, delete Rect!";
//					fishnum++;
//				}
//				else if (!OK_[i] || dis_time || ((time_out / CLOCKS_PER_SEC) > 80)) {
//					CSRT_tracker_erase_(&trackerCSRT, &bboxCSRT, &OK_, &Start_time_record, &fish_distance, &ptr_bboxCSRT, i);
//					str = "Not Fish, delete Rect!";
//				}
//				//跟踪目标越界，在场景中走过的距离不超过300个像素 曼哈顿距离
//				else if ((bboxCSRT[i].x < 0 || bboxCSRT[i].y < 0 || x_b >960 || y_b>540) && (fish_distance[i] < 300)) {
//					CSRT_tracker_erase_(&trackerCSRT, &bboxCSRT, &OK_, &Start_time_record, &fish_distance, &ptr_bboxCSRT, i);
//					str = "Not Fish, delete Rect!";
//				}
//				putText(frameCSRT, "CSRT Tracker Number:" + to_string(OK_.size()), Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//				putText(frameCSRT, "FPS : " + to_string((int(fps))) + str, Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//				putText(frameCSRT, "Found fish: " + to_string(fishnum), Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//				imshow("CSRT Tracking", frameCSRT);
//				waitKey(1);
//			}
//		}
//	}
//	return 0;
//}
//
//
