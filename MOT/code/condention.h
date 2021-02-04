#pragma once
#include <opencv2/opencv.hpp>

/****�������ӽṹ��****/

typedef struct particle

{

	int orix, oriy;//ԭʼ��������

	int x, y;//��ǰ���ӵ�����

	double scale;//��ǰ���Ӵ��ڵĳߴ�

	int prex, prey;//��һ֡���ӵ�����

	double prescale;//��һ֡���Ӵ��ڵĳߴ�

	cv::Rect rect;//��ǰ���Ӿ��δ���

	cv::Mat hist;//��ǰ���Ӵ���ֱ��ͼ����

	double weight;//��ǰ����Ȩֵ 

}PARTICLE;