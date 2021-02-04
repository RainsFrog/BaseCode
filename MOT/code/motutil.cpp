#pragma once

#include"motutil.h"
extern std::vector<std::string> trackerTypes;
cv::Ptr<cv::Tracker> createTrackerByName(std::string trackerType)
{
	cv::Ptr<cv::Tracker> tracker;
	if (trackerType == trackerTypes[0])
	{
		tracker = cv::TrackerBoosting::create();
	}
	else if (trackerType == trackerTypes[1])
		tracker = cv::TrackerMIL::create();
	else if (trackerType == trackerTypes[2])
		tracker = cv::TrackerKCF::create();
	else if (trackerType == trackerTypes[3])
		tracker = cv::TrackerTLD::create();
	else if (trackerType == trackerTypes[4])
		tracker = cv::TrackerMedianFlow::create();
	else if (trackerType == trackerTypes[5])
		tracker = cv::TrackerGOTURN::create();
	else if (trackerType == trackerTypes[6])
		tracker = cv::TrackerMOSSE::create();
	else if (trackerType == trackerTypes[7])
		tracker = cv::TrackerCSRT::create();
	/*else if (trackerType == trackerTypes[8])
	tracker = TrackerMIL::create();*/
	else {
		std::cout << "Incorrect tracker name" << std::endl;
		std::cout << "Available trackers are: " << std::endl;
		for (std::vector<std::string>::iterator it = trackerTypes.begin(); it != trackerTypes.end(); ++it)
			std::cout << " " << *it << std::endl;
	}
	return tracker;
}

void getRandomColors(std::vector<cv::Scalar>&colors, int numColors)
{
	cv::RNG rng(0);
	for (int i = 0;i < numColors;i++)
		colors.push_back(cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
}