#pragma once

#ifndef CL

#define CL

#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>
#include <cmath>

#define PrintIVector(c, vector) {cout << endl << c << endl; for(int i : vector) cout << i << endl;}

namespace cust
{
	template<typename T>
	T getVectorAverage(std::vector<T> input) {
		T sum = 0;
		for (T iter : input) sum += iter;
		return sum / (T)(input.size());
	}

	bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);
	double getAverageContourAreas(std::vector<std::vector<cv::Point>> conts, cv::Mat src);
}

void detectKeyboard(cv::Mat& sorce, cv::Mat& destnation, cv::Rect& rect);
void distortionRotation(cv::Mat& sorce, cv::Mat& dst, cv::Rect& rect);
#endif
