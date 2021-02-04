#pragma
#include <iostream>
#include <string>

#include"motutil.h"
#include"motMultiTracker.h"
#include"MultiThreadTracker.h"

std::vector<std::string> trackerTypes = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
std::string modelChose[5] = { "MultiTracker", "MultiThreadTracker", "condention", "SORT", "DEEPSORT" };

int main(int argc, char** argv)
{
	cv::Ptr<cv::Tracker> myTracker;
	std::string typeT = trackerTypes[2];
	//myTracker = createTrackerByName(typeT);
	motThread();
	//funcMultiTrack();
	system("pause");
	return 0;
}



