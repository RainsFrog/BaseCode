/*
������������ͼ��Ԥ����Ĺ��������������������µ�ͼ����д������ͶԱȣ����������
*/
#include<opencv2\opencv.hpp>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string>
using namespace cv;

// ���ɺͻ���ֱ��ͼ����
class Histogram1D
{
private:
	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];
public:
	Histogram1D()
	{
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 256.0;
		ranges[0] = hranges;
		channels[0] = 0;
	}
	cv::Mat getHistogram(const cv::Mat& image)
	{
		cv::Mat hist;
		cv::calcHist(&image,
			1,//��Ϊһ��ͼ���ֱ��ͼ
			channels,//ʹ�õ�ͨ��
			cv::Mat(),//��ʹ������
			hist,//��Ϊ�����ֱ��ͼ
			1,//��ʱһά��ֱ��ͼ
			histSize,//��������
			ranges//����ֵ�ķ�Χ
		);
		return hist;
	}
	cv::Mat getHistogramImage(const cv::Mat& image, int zoom = 1)
	{
		cv::Mat hist = getHistogram(image);
		return getImageofHistogram(hist, zoom);

	}
	static cv::Mat getImageofHistogram(const cv::Mat &hist, int zoom)
	{
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		int histSize = hist.rows;
		cv::Mat histImg(histSize*zoom, histSize*zoom, CV_8U, cv::Scalar(255));
		//������ߵ�Ϊ90%���ӵĸ���
		int hpt = static_cast<int>(0.9*histSize);
		for (int h = 0; h < histSize; h++)
		{
			float binVal = hist.at<float>(h);
			if (binVal > 0)
			{
				int intensity = static_cast<int>(binVal*hpt / maxVal);
				cv::line(histImg, cv::Point(h*zoom, histSize*zoom),
					cv::Point(h*zoom, (histSize - intensity)*zoom),
					cv::Scalar(0), zoom);
			}
		}
		return histImg;
	}
};


cv::Mat ColorEqualist(cv::Mat image);
cv::Mat blurImg(cv::Mat image);
cv::Mat sharpenImg(cv::Mat image);
cv::Mat sobleImg(cv::Mat image);
void GammaCorrection(Mat& src, Mat& dst, float fGamma);

// �������
void raiseLight(cv::Mat &image1, float alpha, float beta) {

	cv::Mat gammaImg;
	cv::Mat image = image1;

	// imshow("���⴦����1", img_equalist);
	for (int i = 0;i<image1.rows;i++)
		for (int j = 0; j < image1.cols; j++) {
			for (int k = 0; k < 3; k++) {
				int tmp = (uchar)image1.at<Vec3b>(i, j)[k] * alpha + beta;
				if (tmp > 255)
					image1.at<Vec3b>(i, j)[k] = 2 * 255 - tmp;
				else
					image1.at<Vec3b>(i, j)[k] = tmp;
			}
		}
	
}

// ��ɫͼ���⴦��
cv::Mat ColorEqualist(cv::Mat image)
{
	// result
	cv::Mat result, result2show;
	// HLS
	cv::Mat hlsimg, image2show;
	cv::cvtColor(image, hlsimg, COLOR_BGR2HLS);
	// split image into channels
	std::vector<cv::Mat> channels;
	cv::split(hlsimg, channels);
	// equalize the light channel ; 
	cv::equalizeHist(channels[1], channels[1]);
	// merge the result channels
	cv::merge(channels, hlsimg);
	cv::cvtColor(hlsimg, result, COLOR_HLS2BGR);
	// show image
	cv::resize(image, image2show, cv::Size(), 0.4, 0.4);
	cv::resize(result, result2show, cv::Size(), 0.4, 0.4);
	cv::imshow("ԭͼ", image2show);
	cv::imshow("��ɫ����֮���ͼ��", result2show);
	// cv::hconcat(image2show, )
	return result;
}

// ��ֵ�˲�
cv::Mat blurImg(cv::Mat image)
{
	cv::Mat result;
	cv::medianBlur(image, result, 3);
	cv::resize(result, result, cv::Size(), 0.4, 0.4);
	cv::imshow("��ֵ�˲����չʾ", result);

	return result;
}

// clipHistPercent ��֦����ȥ�����صĶ��ٰٷֱȣ�
// histSize ������еĻҶ�ֵ�鵽���ķ�Χ
// lowhist ��С�ĻҶ�ֵ
void BrightnessAndContrastAuto(const cv::Mat &src, cv::Mat &dst, float clipHistPercent = 0, int histSize = 255, int lowhist = 0)
{

	CV_Assert(clipHistPercent >= 0);
	CV_Assert((src.type() == CV_8UC1) || (src.type() == CV_8UC3) || (src.type() == CV_8UC4));

	float alpha, beta;
	double minGray = 0, maxGray = 0;

	//to calculate grayscale histogram
	cv::Mat gray;
	if (src.type() == CV_8UC1) gray = src;
	else if (src.type() == CV_8UC3) cvtColor(src, gray, CV_BGR2GRAY);
	else if (src.type() == CV_8UC4) cvtColor(src, gray, CV_BGRA2GRAY);
	if (clipHistPercent == 0)
	{
		// keep full available range
		cv::minMaxLoc(gray, &minGray, &maxGray);
	}
	else
	{
		cv::Mat hist; //the grayscale histogram

		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true;
		bool accumulate = false;
		calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// calculate cumulative distribution from the histogram
		std::vector<float> accumulator(histSize);
		accumulator[0] = hist.at<float>(0);
		for (int i = 1; i < histSize; i++)
		{
			accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
		}

		// locate points that cuts at required value
		float max = accumulator.back();

		int clipHistPercent2;
		clipHistPercent2 = clipHistPercent * (max / 100.0); //make percent as absolute
		clipHistPercent2 /= 2.0; // left and right wings
								 // locate left cut
		minGray = 0;
		while (accumulator[minGray] < clipHistPercent2)
			minGray++;

		// locate right cut
		maxGray = histSize - 1;
		while (accumulator[maxGray] >= (max - clipHistPercent2))
			maxGray--;
	}

	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
	beta = -minGray * alpha + lowhist;             // beta shifts current range so that minGray will go to 0

												   // Apply brightness and contrast normalization
												   // convertTo operates with saurate_cast
	// src.convertTo(dst, -1, alpha, beta);
	// restore alpha channel from source
	convertScaleAbs(src, dst, alpha, beta);
	if (dst.type() == CV_8UC4)
	{
		int from_to[] = { 3, 3 };
		cv::mixChannels(&src, 4, &dst, 1, from_to, 1);
	}
}


 // ٤�����
void GammaCorrection(Mat& src, Mat& dst, float fGamma)
{
	CV_Assert(src.data);

	// accept only char type matrices
	CV_Assert(src.depth() != sizeof(uchar));

	// build look up table
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}

	dst = src.clone();
	const int channels = dst.channels();
	switch (channels)
	{
	case 1:
	{

		MatIterator_<uchar> it, end;
		for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
			//*it = pow((float)(((*it))/255.0), fGamma) * 255.0;
			*it = lut[(*it)];

		break;
	}
	case 3:
	{

		MatIterator_<Vec3b> it, end;
		for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
		{
			//(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;
			//(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;
			//(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;
			(*it)[0] = lut[((*it)[0])];
			(*it)[1] = lut[((*it)[1])];
			(*it)[2] = lut[((*it)[2])];
		}

		break;

	}
	}
}


// ����sobel���ӵ�ͼ���Ե���
cv::Mat sobleImg(cv::Mat image)
{
	if (image.empty())
	{
		std::cout << "ͼƬ��ȡ���ɹ�" << std::endl;
	}
	cv::imshow("ԭͼ", image);
	cv::Mat result, grayx, grayy, grayadd, gray;
	// ��ͼ��ת���ɻҶ�ͼ
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::Sobel(gray, grayx, CV_16S, 1, 0, 3, 1, 1, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grayx, grayx);
	cv::imshow("x�����ݶ�", grayx);

	cv::cvtColor(image, grayy, cv::COLOR_BGR2GRAY);
	cv::Sobel(gray, grayy, CV_16S, 0, 1, 3, 1, 1, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grayy, grayy);
	cv::imshow("y�����ݶ�", grayy);

	cv::addWeighted(grayx, 0.5, grayy, 0.5, 0.4, grayadd);
	cv::imshow("dx��dy�ں�", grayadd);
	return result;
	//return image;
}


// ʹ�þ����ͼ������񻯲���
cv::Mat sharpenImg(cv::Mat image)
{

	cv::Mat result;
	cv::Mat sharpen_op = (cv::Mat_<char>(3, 3) << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0);
	cv::filter2D(image, result, CV_32F, sharpen_op);
	convertScaleAbs(result, result);
	// cv::resize(result, result, cv::Size(), 0.4, 0.4);
	cv::imshow("��֮���ͼ��", result);
	return result;
}

// ȥ���߹⣬ δ���
int removeHighLight(cv::Mat image)
{
	// ת�ɻҶ�ͼ
	cv::Mat grayimg;
	cv::Mat grayHist;
	int histSize[1] = { 256 };
	int channels[1] = { 0 };
	float range[] = { 0, 256 };
	const float * histRanges[] = { range };
	cv::cvtColor(image, grayimg, COLOR_BGR2GRAY);
	// cv::calcHist()
	cv::calcHist(&grayimg, 1, channels, cv::Mat(), grayHist, 1, histSize, histRanges, true, false);
	cv::imshow("hist", grayHist);
	// cvWaitKey(0);

	// ����
	int hist_cols = 256;
	int hist_rows = 184;
	int bin_w = hist_rows / 256;
	Mat histImage(hist_rows, hist_cols, CV_8UC1, cv::Scalar(255, 255, 255));
	for (int i = 1; i < 256; i++)
	{
		line(histImage, cv::Point((i - 1)*bin_w, hist_cols - cvRound(grayHist.at<float>(i - 1))),
			cv::Point(i*bin_w, hist_cols - cvRound(grayHist.at<float>(i))), cv::Scalar(0, 0, 0), 2, LINE_AA);
	}
	cv::imshow("hist", histImage);
	return 1;
}

// ֱ��ͼͳ��
int countHist(cv::Mat image, char * name)
{
	Histogram1D ch;
	cv::Mat histo = ch.getHistogram(image);
	cv::Mat hist = ch.getHistogramImage(image);
	cv::imshow(name, hist);
	return 1;
}

// ����Ӧ��ͼ�����
int LimitEqualist(cv::Mat img)
{
	auto clahe_ = cv::createCLAHE(2.0);
	clahe_->apply(img, img);
	cv::equalizeHist(img, img);
	cv::imshow("����Ӧͼ�����", img);
	return 0;


}

int main(int argc, char** argv)
{

	
	cv::Mat img = cv::imread("E:\\Code\\�Զ�������\\�������\\opencvTest\\opencvTest\\���ݽ�ͼ\\backsingleship.jpg");
	// namedWindow("��Ե���");
	//cv::imshow("my window", img);
	//sobleImg(img);
	//removeHighLight(img);
	// raiseLight(1.5, 1.0);
	char name[] = "ԭͼhist";
	countHist(img, name);
	
	cv::Mat dst, reimg;
	cv::resize(img, reimg, cv::Size(), 0.4, 0.4);
	cv::imshow("ԭͼ", reimg);
	BrightnessAndContrastAuto(img, dst, 5, 200, 4);
	char name2[] = "����Ӧ���ȵ���img";
	countHist(dst, name2);
	cv::Mat dstblur = blurImg(dst);
	cv::resize(dst, dstblur, cv::Size(), 0.4, 0.4);
	cv::imshow("����Ӧ���ȵ���hist��", dstblur);
	//system("pause");

	raiseLight(img, 1.5, 1.0);
	cv::Mat resultImg = blurImg(img);
	cv::resize(img, img, cv::Size(), 0.4, 0.4);
	cv::imshow("�������˲�֮��img", img);
	countHist(img, "�������˲�֮��hist");
	cvWaitKey(0);
	return 0;
}




