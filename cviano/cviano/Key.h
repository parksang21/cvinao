#pragma once

#include "custom.h"

// for key type
#define WHITE_KEY 0
#define BLACK_KEY 1

namespace kb{

	class Key
	{
	private:
		int note = -1;
		int key_type;
		cv::Rect rect;		// ROI 영역을 만들기 위한 rect 객체
		std::vector<cv::Point> contour;

	public:
		cv::Mat roi;
		bool detectPress(cv::Mat diffVideo);
		cv::Rect getRect();
		cv::Mat getMat();
		void setNote(int note);
		int getNote();

		Key();
		Key(cv::Rect& roi);
		Key(cv::Mat& src, cv::Rect& r);
		Key(cv::Mat& image, std::vector<cv::Point>& contour);
		Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& contour, int music);
		~Key();
	};

	void mapKeys(cv::Mat& src, cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys, cv::Rect source_rect);
	bool compareKeys(kb::Key key1, kb::Key key2);
	void setMusicalNote(std::vector<kb::Key>& keys);
	void drawKeys(cv::Mat& image, std::vector<kb::Key> keys);
}


void setWhiteKeyVector(cv::Mat& src, cv::Mat& dst, std::vector<kb::Key>& keys, cv::Rect rect);
void sihyun(std::vector<kb::Key>, cv::VideoCapture);