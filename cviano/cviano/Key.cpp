#include "Key.h"

#include "MidiFile.h"

kb::Key::Key() {};

kb::Key::Key(cv::Rect& r) {
	rect = r;
}

kb::Key::Key(cv::Mat& src, cv::Rect& r) {
	rect = r;
	oroi = src(r);
};

kb::Key::Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& c, int music){
	rect = r;
	contour = c;
	oroi = src(r);
	key_type = music;
}

kb::Key::Key(cv::Mat& image, std::vector<cv::Point>& contour) {

}

kb::Key::~Key() {}

bool kb::Key::detectPress(cv::Mat diffVideo) {
	//imshow("tesasdfasdf", mask);
	//imshow("asdfasdf", roi);
	//cv::waitKey(0);
	unsigned int RR= this->getRoi().rows;
	unsigned int CC = this->getRoi().cols;
	this->setRoi(diffVideo);
	//imshow("getRoi", this->getRoi());
	for (unsigned int i = 0; i < RR; i++) {
		for (unsigned int j = 0; j <CC; j++) {
			if (*roi.ptr<uchar>(i, j) ==255) {
				return true;
			}
		}
	}
	return false;
}


cv::Rect kb::Key::getRect() 
{
	return rect;
}

void kb::Key::setNote(int note) 
{
	this->note = note;
}

int kb::Key::getNote() 
{
	return note;
}

std::vector<cv::Point> kb::Key::getContour() 
{
	return contour;
}

void kb::Key::setORoi(cv::Mat source)
{
	oroi = source(rect);
}

void kb::Key::setORoi(cv::Mat source, cv::Rect rect_roi)
{
	oroi = source(rect_roi);
}

cv::Mat kb::Key::getORoi(){	return oroi;}

void kb::Key::setRoi(cv::Mat source){ roi = source(rect) & mask;}

cv::Mat kb::Key::getRoi() {	return roi;}

void kb::Key::setMask()
{
	cvtColor(oroi.clone(), mask, CV_BGR2GRAY);
	mask -= 255;
	std::vector<std::vector<cv::Point>> adjust_conts;
	std::vector<cv::Point> adjust_cont;
	for (int i = 0; i < contour.size(); i++)
	{
		adjust_cont.push_back(contour[i] - rect.tl());
	}
	adjust_conts.push_back(adjust_cont);
	cv::drawContours(mask, adjust_conts, -1, cv::Scalar(255), CV_FILLED);
	cv::drawContours(mask, adjust_conts, -1, cv::Scalar(0), 3);
	cv::Mat test_mask(mask.size(), CV_8UC1, cv::Scalar(0));
	
	cv::drawContours(test_mask, adjust_conts, -1, cv::Scalar(255), CV_FILLED);
	cv::drawContours(test_mask, adjust_conts, -1, cv::Scalar(0), 20);
	mask = mask - test_mask;

}

cv::Mat kb::Key::getMask(){	return mask; }

// ==================================================================================
//			클래스 외 function
// ==================================================================================

void kb::mapKeys(cv::Mat& source, cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys, cv::Rect srect)
{
	// to calculate the contour area's average
	int cont_sum = 0, avg_cont = 0;

	// to calculate the sum of contour area
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		cont_sum += cv::contourArea(*iter);
		if (cv::contourArea(*iter) >= (double)image.size().area() / 2)
		{
			contours.erase(iter);
			cont_sum = 0;
			iter = contours.begin();
		}
	}

	// calculate the average of contour area.
	avg_cont = cont_sum / contours.size();


	// delete the unwanted contours
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		if (cv::contourArea(*iter) < avg_cont)
		{
			contours.erase(iter);
			iter--;
		}
	}
	
	// to adjust to the origin image
	cv::Point origin = srect.tl();
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		cv::Rect r = boundingRect(*iter);
		r.height *= 0.95;
		r += origin;

		for (std::vector<cv::Point>::iterator point = iter->begin();
			point < iter->end(); point++) {
			*point += origin;
		}

		kb::Key key(source, r, *iter, WHITE_KEY);
		keys.push_back(key);
	}

	// key 값 소팅
	std::sort(keys.begin(), keys.end(), kb::compareKeys);

}


bool kb::compareKeys(kb::Key key1, kb::Key key2) 
{
	int i = key1.getRect().tl().x;
	int j = key2.getRect().tl().x;
	return (i < j);
}


void kb::setMusicalNote(std::vector<kb::Key>& keys) 
{
	int key_num = (int) keys.size();
	int start = 0;

	if (key_num == 22)
		start = (int) kb::NOTE::F2;

	for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
		if (start % 12 == 6 || start % 12 == 8 || start % 12 == 10 || start % 12 == 1 || start % 12 == 3)
			start++;

		iter->setNote(start);
		start++;
	}

}

void kb::drawKeys(cv::Mat& image, std::vector<kb::Key> keys)
{
	
	for (int i = 0; i < keys.size(); i++)
	{
		cv::rectangle(image, keys[i].getRect(), cv::Scalar(255, 0, 0), 2);
		cv::putText(image,
			cv::String(std::to_string(keys[i].getNote())),
			keys[i].getRect().tl() + cv::Point(0, 20),
			2,
			1.2,
			cv::Scalar(0, 0, 255));
	}
}

void setWhiteKeyVector(cv::Mat& source, cv::Mat& roi, std::vector<kb::Key>& keys, cv::Rect rect) 
{

	cv::Mat image;

	cv::cvtColor(roi, image, CV_BGR2GRAY);

	cv::Mat binary_adaptive, canny;
	adaptiveThreshold(image, binary_adaptive, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 21, 10);

	binary_adaptive = 255 - binary_adaptive;

	// display :: binary_adaptive image;
	// imshow("binary", binary_adaptive);

	GaussianBlur(binary_adaptive, binary_adaptive, cv::Size(), 2, 2);
	Canny(binary_adaptive, canny, 125, 350);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
	cv::Mat morph;
	morphologyEx(canny, morph, CV_MOP_CLOSE, kernel);

	// display :: morphed image
	// imshow("morph", morph);


	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::rectangle(morph, cv::Rect(cv::Point(0, 0), cv::Size(morph.size())), cv::Scalar(255), 5);
	cv::findContours(morph, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
;

	std::sort(contours.begin(), contours.end(), cust::compareContourAreas);

	kb::mapKeys(source, image, contours, keys, rect);

	// 흑건까지 찾은 뒤에 해야할 일.
	kb::setMusicalNote(keys);

	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].setORoi(source);
		// cv::imshow(std::to_string(i), keys[i].getORoi());
		keys[i].setMask();
		// cv::imshow("mask " + std::to_string(i), keys[i].getMask());
	}

	// to show cont
	cv::Mat cont(source);
	cv::drawContours(source, contours, -1, cv::Scalar(255, 255, 0), 3);
	kb::drawKeys(source, keys);
	imshow("cont", source);
	//cv::waitKey();
	
}
