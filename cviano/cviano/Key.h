#pragma once

#ifndef _KEY_H_
#define _KEY_H_

#include "custom.h"


// for key_type
#define WHITE_KEY 0
#define BLACK_KEY 1


namespace kb{
		
	enum class NOTE
	{
		Cn1=0, CSn1, Dn1, DSn1, En1, Fn1, FSn1, Gn1, GSn1, An1, ASn1, Bn1, 
		C0, CS0, D0, DS0, E0, F0, FS0, G0, GS0, A0, AS0, B0, 
		C1, CS1, D1, DS1, E1, F1, FS1, G1, GS1, A1, AS1, B1,
		C2, CS2, D2, DS2, E2, F2, FS2, G2, GS2, A2, AS2, B2,
		C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3,
		C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4,
		C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5,
		C6, CS6, D6, DS6, E6, F6, FS6, G6, GS6, A6, AS6, B6,
		C7, CS7, D7, DS7, E7, F7, FS7, G7, GS7, A7, AS7, B7
	};

	/*
	std::string sNote[] = {
		"Cn1", "CSn1", "Dn1", "Dsn1", "En1", "Fn1", "FSn1", "Gn1", "An1", "ASn1", "Bn1",
		"C0", "CS0", "D0", "DS0", "E0", "F0", "FS0", "G0", "GS0", "A0", "AS0", "B0",
		"C1", "CS1", "D1", "DS1", "E1", "F1", "FS1", "G1", "GS1", "A1", "AS1", "B1",
		"C2", "CS2", "D2", "DS2", "E2", "F2", "FS2", "G2", "GS2", "A2", "AS2", "B2",
		"C3", "CS3", "D3", "DS3", "E3", "F3", "FS3", "G3", "GS3", "A3", "AS3", "B3",
		"C4", "CS4", "D4", "DS4", "E4", "F4", "FS4", "G4", "GS4", "A4", "AS4", "B4",
		"C5", "CS5", "D5", "DS5", "E5", "F5", "FS5", "G5", "GS5", "A5", "AS5", "B5",
		"C6", "CS6", "D6", "DS6", "E6", "F6", "FS6", "G6", "GS6", "A6", "AS6", "B6",
		"C7", "CS7", "D7", "DS7", "E7", "F7", "FS7", "G7", "GS7", "A7", "AS7", "B7"
	};
	*/

	class Key
	{
	private:
		int note = -1;
		int key_type;
		cv::Rect rect;	// ROI 영역을 만들기 위한 rect 객체
		cv::Mat oroi, roi, mask;
		std::vector<cv::Point> contour;
	public:
		bool detectPress(cv::Mat diffVideo);
		cv::Rect getRect();
		void setNote(int note);
		int getNote();
		std::vector<cv::Point> getContour();
		void setORoi(cv::Mat source);
		void setORoi(cv::Mat source, cv::Rect rect_roi);
		cv::Mat getORoi();
		void setMask();
		cv::Mat getMask();

		void setRoi(cv::Mat cource);
		cv::Mat getRoi();

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
void sihyun(std::vector<kb::Key>, cv::VideoCapture,std::vector<std::pair<int,int>>&);
void makeMusicSheet(std::vector<std::pair<int, float>>& notePairVec);
void detectKeyboard(cv::Mat& sorce, cv::Mat& destnation, cv::Rect& rect);
void distortionRotation(cv::Mat& sorce, cv::Mat& dst, cv::Rect& rect);
void draw_houghLines(cv::Mat image, cv::Mat& dst, std::vector<cv::Vec2f> lines, int nline, std::vector<cv::Point2d>& linePts);
void makeNote(std::vector<std::pair<int, int>>& preNote, std::vector<std::pair<int, float>>& output_note);

#endif // !KEYBOARD

