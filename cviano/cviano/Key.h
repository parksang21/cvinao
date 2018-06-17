#pragma once

#include "custom.h"

// for key type
#define WHITE_KEY 0
#define BLACK_KEY 1

#define Cn1 0
#define CSn1 1
#define Dn1 2
#define DSn1 3
#define En1 4
#define Fn1 5
#define FSn1 6
#define Gn1 7
#define GSn1 8
#define An1 9
#define ASn1 10
#define Bn1 11


#define C0 12
#define CS0 13
#define D0 14
#define DS0 15
#define E0 16
#define F0 17
#define FS0 18
#define G0 19
#define GS0 20
#define A0 21
#define AS0 22
#define B0 23

#define C1 24
#define CS1 25
#define D1 26
#define DS1 27
#define E1 28
#define F1 29
#define FS1 30
#define G1 31
#define GS1 32
#define A1 33
#define AS1 34
#define B1 35

#define C2 36
#define CS2 37
#define D2 38
#define DS2 39
#define E2 40
#define F2 41
#define FS2 42
#define G2 43
#define GS2 44
#define A2 45
#define AS2 46
#define B2 47

#define C3 48
#define CS3 49
#define D3 50
#define DS3 51
#define E3 52
#define F3 53
#define FS3 54
#define G3 55
#define GS3 56
#define A3 57
#define AS3 58
#define B3 59

#define C4 60
#define CS4 61
#define D4 62
#define DS4 63
#define E4 64
#define F4 65
#define FS4 66
#define G4 67
#define GS4 68
#define A4 69
#define AS4 70
#define B4 71

#define C5 72
#define CS5 73
#define D5 74
#define DS5 75
#define E5 76
#define F5 77
#define FS5 78
#define G5 79
#define GS5 80
#define A5 81
#define AS5 82
#define B5 83

#define C6 84
#define CS6 85
#define D6 86
#define DS6 87
#define E6 88
#define F6 89
#define FS6 90
#define G6 91
#define GS6 92
#define A6 93
#define AS6 94
#define B6 95

#define C7 96
#define CS7 97
#define D7 98
#define DS7 99
#define E7 100
#define F7 101
#define FS7 102
#define G7 103
#define GS7 104
#define A7 105
#define AS7 106
#define B7 107

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