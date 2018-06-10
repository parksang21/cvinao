#pragma once

#include "custom.h"

// for key type
#define WHITE_KEY 0
#define BLACK_KEY 1

namespace kb{

	class Key
	{
	private:
		int sclae = -1;
		int key_type;
		cv::Rect rect;		// ROI 영역을 만들기 위한 rect 객체
		std::vector<cv::Point> contour;

	public:
		cv::Mat roi;
		bool detectPress(cv::Mat diffVideo);
		cv::Rect getRect();
		cv::Mat getMat();

		Key();
		Key(cv::Rect& roi);
		Key(cv::Mat& src, cv::Rect& r);
		Key(cv::Mat& image, std::vector<cv::Point>& contour);
		Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& contour);
		~Key();
	};

	void mapKeys(cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys);
	bool compareKeys(kb::Key key1, kb::Key key2);
}


