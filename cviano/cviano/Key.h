#pragma once

#include "custom.h"

// for key type
#define WHITE_KEY 0
#define BLACK_KEY 1

namespace kb{

	class Key : public cv::Mat
	{
	private:
		int sclae = -1;
		int key_type;
		cv::Rect rect;		// ROI 영역을 만들기 위한 rect 객체
		std::vector<cv::Point> contour;

	public:
		bool detectPress();
		cv::Rect getRect();
		Key();
		Key(cv::Rect roi);
		Key(cv::Mat src, cv::Rect r);
		Key(cv::Mat src, cv::Rect r, std::vector<cv::Point> contour);
		Key(cv::Mat image, std::vector<cv::Point> contour);
		~Key();
	};

	void mapKeys(cv::Mat image, std::vector<std::vector<cv::Point>> contours, std::vector<kb::Key> keys);

}


