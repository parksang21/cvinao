#pragma once

#ifndef _CUSTOM_H_

#define _CUSTOM_H_


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

#endif
