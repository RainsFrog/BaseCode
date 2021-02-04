#include"MouseEvent.h"
#include"dataType.h"
using namespace cv;
using namespace std;

Mat srcImg, tmpImg, MidImg;

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

void MouseEvent(int EVENT, int x, int y, int flags, void *ustc)
{
	mousepara *p = static_cast<mousepara *>(ustc);
	srcImg = p->frame;
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
		putText(srcImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(srcImg, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //左键按下的点划圆 			
		imshow("DealImg", srcImg);
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
		putText(srcImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		imshow("DealImg", srcImg);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(srcImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(srcImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", srcImg);
	}
	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //左键松开，将在图像上划矩形 
	{
		srcImg.copyTo(MidImg);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		Cur_pt = Point(x, y);
		putText(srcImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		putText(srcImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(srcImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", srcImg);
	}
}
