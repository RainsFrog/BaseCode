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
//	setMouseCallback("DealImg", MouseEvent, (void*)(&tmpImg)); //���ûص����� 
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

	if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ 
	{
		Pre_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(p.frame, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(p.frame, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //������µĵ㻮Բ 			
		// imshow("DealImg", srcImg);
		cv::imshow(WINDOWNAME, p.frame);
	}

	else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����
	{
		if (MidImg.data == NULL)
		{
			srcImg.copyTo(tmpImg);                                            //��src���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
		}
		else
		{
			MidImg.copyTo(tmpImg);                                             //���ѻ������ε����ʱ����ͼ������ʾǰ������ͼ��
		}
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(p.frame, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		// imshow("DealImg", srcImg);
		cv::imshow(WINDOWNAME, p.frame);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ����� 
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
	// ��������ƶ�ֹͣʱ�����bbox��Ϊ�գ���ô����
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
	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //����ɿ�������ͼ���ϻ����� 
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
		std::cout << "�ڲ������ʱ���ǣ�" << time1 << std::endl;
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

	if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ 
	{
		Pre_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(tmpImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(tmpImg, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //������µĵ㻮Բ 			
		imshow("DealImg", tmpImg);
	}

	else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����
	{
		if (MidImg.data == NULL)
		{
			srcImg.copyTo(tmpImg);                                            //��src���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
		}
		else
		{
			MidImg.copyTo(tmpImg);                                             //���ѻ������ε����ʱ����ͼ������ʾǰ������ͼ��
		}
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(tmpImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		imshow("DealImg", tmpImg);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ����� 
	{
		srcImg.copyTo(MidImg);
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(MidImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", MidImg);
	}

	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //����ɿ�������ͼ���ϻ����� 
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
