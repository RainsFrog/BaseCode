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
//	setMouseCallback("DealImg", MouseEvent, (void*)(&tmpImg)); //���ûص����� 
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

	if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ 
	{
		Pre_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(srcImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		circle(srcImg, Pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);    //������µĵ㻮Բ 			
		imshow("DealImg", srcImg);
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
		putText(srcImg, Txt_Point, Cur_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		imshow("DealImg", srcImg);
	}

	else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ����� 
	{
		srcImg.copyTo(MidImg);
		Cur_pt = Point(x, y);
		sprintf_s(Txt_Point, "(%d,%d)", x, y);
		putText(srcImg, Txt_Point, Pre_pt, CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
		rectangle(srcImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
		imshow("DealImg", srcImg);
	}
	else if (EVENT == CV_EVENT_LBUTTONUP)                                          //����ɿ�������ͼ���ϻ����� 
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
