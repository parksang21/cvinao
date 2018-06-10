#pragma once

#include "custom.h"

// for key type
#define WHITE_KEY 0
#define BLACK_KEY 1

#define A0 006
#define B0 007
#define C1 101
#define D1 102
#define E1 103
#define F1 104
#define G1 105
#define A1 106
#define B1 107
#define C2 201
#define D2 202
#define E2 203
#define F2 204
#define G2 205
#define A2 206
#define B2 207
#define C3 301
#define D3 302
#define E3 303
#define F3 304
#define G3 305
#define A3 306
#define B3 307
#define C4 401
#define D4 402
#define E4 403
#define F4 404
#define G4 405
#define A4 406
#define B4 407
#define C5 501
#define D5 502
#define E5 503
#define F5 504
#define G5 505
#define A5 506
#define B5 507
#define C6 601
#define D6 602
#define E6 603
#define F6 604
#define G6 605
#define A6 606
#define B6 607
#define C7 701
#define D7 702
#define E7 703
#define F7 704
#define G7 705
#define A7 706
#define B7 707
#define C8 801


namespace kb{

	class Key
	{
	private:
		int scale = -1;
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