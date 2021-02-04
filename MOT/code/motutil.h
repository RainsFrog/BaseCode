
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <thread>

cv::Ptr<cv::Tracker> createTrackerByName(std::string trackerType);
void getRandomColors(std::vector<cv::Scalar> &colors, int numColors);

/*
debug lib
opencv_core343d.lib
opencv_tracking343d.lib
opencv_xfeatures2d343d.lib
opencv_videoio343d.lib
opencv_photo343d.lib
opencv_highgui343d.lib
opencv_features2d343d.lib
opencv_plot343d.lib
opencv_datasets343d.lib
opencv_text343d.lib
opencv_shape343d.lib
opencv_imgproc343d.lib

*/