#include "custom.h"

bool cust::compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
	double i = fabs(cv::contourArea(contour1));
	double j = fabs(cv::contourArea(contour2));
	return (i > j);
}

double cust::getAverageContourAreas(std::vector<std::vector<cv::Point>> conts, cv::Mat src) {
	double sum = 0.0;
	std::vector<std::vector<cv::Point>>::iterator iter = conts.begin();;
	while (iter != conts.end()) {
		sum += contourArea(*iter);
		iter++;
	}
	std::cout << sum;
	return sum / (double)conts.size();
}
