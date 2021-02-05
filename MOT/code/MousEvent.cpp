#include"MouseEvent.h"
#include"dataType.h"
#include<time.h>
#include<Windows.h>
using namespace cv;
using namespace std;

Mat srcImg, tmpImg, MidImg;
#define WINDOWNAME "MultiTracker"
//int mainME(void)
//{
//	srcImg = imread("2.jpg");
//	namedWindow("DealImg", WINDOW_NORMAL);
//	srcImg.copyTo(tmpImg);
//	imshow("DealImg", tmpImg);
//	setMouseCallback("DealImg", MouseEvent, (void*)(&tmpImg)); //调用回调函数 
//	waitKey(0);
//	destroyWindow("DealImg");
//}
// extern std::vector<cv::Rect> rectStore;

void MouseEvent_aa(int event, int x, int y, int flags, void *para)
{
	mousepara* p = (mousepara *)para;
	((mousepara *)para)->bboxes.push_back(cv::Rect(1,2,3,4));
	printf("frame.width=%d\n", ((mousepara *)para)->frame.cols);
	cv::Rect r = cv::Rect(1, 2, 3, 4);
	memcpy(para, &r, sizeof(cv::Rect));
}
void MouseEvent(int EVENT, int x, int y, int flags, void *ustc)
{
	//mousepara *p = static_cast<mousepara *>(ustc);
	mousepara& p = *(mousepara *)ustc;
	srcImg = p.frame;
	std::cout << "2-->  " << &p << std::endl;
	std::cout << "3-->  " << ustc << std::endl;
	static Point Pre_pt(-1, -1);
	static Point Cur_pt(-1, -1);
	char Txt_Point[50] = { 0 };

	if (ustc == NULL)
	{
		return;
	}

	if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //左键按下，读取初始坐标，并在图像上该点处划圆 
	{
		Pre_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(p.frame, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(p.frame, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //左键按下的点划圆 			
		// imshow("DealImg", srcImg);
		cv::imshow(WINDOWNAME, p.frame);
	}

	else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
	{
		if (MidImg.data == NULL)
		{
			srcImg.copyTo(tmpImg);                                            //将src复制到临时图像tmp上，用于显示实时坐标  
		}
		else
		{
			MidImg.copyTo(tmpImg);                                             //当已划定矩形的情况时，在图像上显示前面所画图像
		}
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(p.frame, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		// imshow("DealImg", srcImg);
		cv::imshow(WINDOWNAME, p.frame);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(p.frame, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(p.frame, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		//imshow("DealImg", srcImg);
		// rectStore.push_back(cv::Rect(Cur_pt, Pre_pt));
		cv::imshow(WINDOWNAME, p.frame);
	}
	// 左键按下移动停止时，如果bbox不为空，那么画框
	else if (EVENT == CV_EVENT_LBUTTONDOWN)
	{
		rectStore.push_back(cv::Rect(Cur_pt, Pre_pt));
		/*for (auto box : rectStore)
		{
			rectangle(p.frame, box, Scalar(0, 0, 255), 1, 8, 0);
		}*/
		rectangle(p.frame, rectStore.back(), Scalar(0, 0, 255), 1, 8, 0);
		cv::imshow(WINDOWNAME, p.frame);
		
	}
	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //左键松开，将在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		Cur_pt = Point2i(x, y);
		putText(p.frame, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		cv::Rect rectNow = cv::Rect(Pre_pt, Cur_pt);
		 p.bboxes.push_back(rectNow);

		pushFlag = true;
		p.onFlag = true;
		rectangle(p.frame, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		if (rectStore.size() > 0)
		{
			rectStore.swap(std::vector<cv::Rect>());
		}
		//imshow(WINDOWNAME, p.frame);
		cv::imshow(WINDOWNAME, p.frame);
		DWORD time1 = GetTickCount();
		std::cout << "内部画框的时间是：" << time1 << std::endl;
	}
}

void on_mouse(int EVENT, int x, int y, int flags, void* ustc)
{
	mousepara &para = *(mousepara*)ustc;
	srcImg = para.frame;
	static Point Pre_pt(-1, -1);
	static Point Cur_pt(-1, -1);
	char Txt_Point[50] = { 0 };

	if (ustc == NULL)
	{
		return;
	}

	if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //左键按下，读取初始坐标，并在图像上该点处划圆 
	{
		Pre_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(tmpImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(tmpImg, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //左键按下的点划圆 			
		imshow("DealImg", tmpImg);
	}

	else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
	{
		if (MidImg.data == NULL)
		{
			srcImg.copyTo(tmpImg);                                            //将src复制到临时图像tmp上，用于显示实时坐标  
		}
		else
		{
			MidImg.copyTo(tmpImg);                                             //当已划定矩形的情况时，在图像上显示前面所画图像
		}
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(tmpImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		imshow("DealImg", tmpImg);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(MidImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", MidImg);
	}

	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //左键松开，将在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		Cur_pt = Point(x, y);
		putText(MidImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		putText(MidImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", MidImg);
	}
}
