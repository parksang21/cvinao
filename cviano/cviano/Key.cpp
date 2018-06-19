#include "Key.h"

kb::Key::Key() {};

kb::Key::Key(cv::Rect& r) {
	rect = r;
}

kb::Key::Key(cv::Mat& src, cv::Rect& r) {
	rect = r;
	roi = src(r);
};

kb::Key::Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& c, int music){
	rect = r;
	contour = c;
	roi = src(r);
	key_type = music;
}

kb::Key::Key(cv::Mat& image, std::vector<cv::Point>& contour) {

}

kb::Key::~Key() {}

bool kb::Key::detectPress(cv::Mat diffVideo) {
	unsigned int hitCount = 0; // how many pixels change
	unsigned int criticalPoint = 0; //if hitCount is bigger than this value, we assume that key is pressed

	unsigned int TopLeftX = this->getRect().x;
	unsigned int TopLeftY = this->getRect().y;

	unsigned int KeyWidth = this->getRect().size().width;
	unsigned int KeyHeight = this->getRect().size().height;

	criticalPoint = KeyWidth * (KeyHeight-60)/15;
	/*
	std::cout << "W: " << KeyWidth << std::endl;
	std::cout << "H: " << KeyHeight << std::endl;
	std::cout << "C: " << criticalPoint << std::endl;
	*/
	for (unsigned int i = 0; i < this->roi.rows; i++) {
		for (unsigned int j = 0; j <this->roi.cols; j++) {
			if (*diffVideo.ptr<uchar>(j, i) == 255) { hitCount++; //std::cout<<"[" << hitCount<<", "<<note << "]"<<std::endl; 
			}
			if (hitCount >= criticalPoint) { return true; }
		}
	}

	return false;
}

cv::Rect kb::Key::getRect() {
	return rect;
}

cv::Mat kb::Key::getMat() {
	return roi;
}

void kb::Key::setNote(int note) {
	this->note = note;
}

int kb::Key::getNote() {
	return note;
}


// ==================================================================================
//			클래스 외 function
// ==================================================================================

void kb::mapKeys(cv::Mat& source, cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys, cv::Rect srect)
{
	int cont_sum = 0, avg_cont = 0;

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

	avg_cont = cont_sum / contours.size();

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
	
	cv::Point origin = srect.tl();
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin(); 
		iter < contours.end(); 
		iter++) 
	{
		cv::Rect r = boundingRect(*iter);
		r += origin;

		for (std::vector<cv::Point>::iterator point = iter->begin();
			point < iter->end(); point++) {
			*point += origin;
		}

		kb::Key key(source, r, *iter, WHITE_KEY);
		keys.push_back(key);
	}

	int image_size = image.size().area();
	int criteria = image_size / (int)keys.size();

	cv::Mat test(image.size(), CV_8UC3);
	cv::drawContours(source, contours, -1, cv::Scalar(0, 0, 255));

	cv::imshow("test", source);
	cv::waitKey();

	// 작은 것들 삭제하기

	/*
	for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
		cv::Rect r = iter->getRect();
		if (r.area() < criteria ||
			r.area() > image_size / 2)
		{
			keys.erase(iter);
			iter--;
		}
	}
	*/


	// key 값 정제하기
	std::sort(keys.begin(), keys.end(), kb::compareKeys);
	
	if (keys.size() > 22)
	{
		int sum = 0;

		for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++)
		{
			sum += iter->getRect().width + iter->getRect().height;
		}

		int average = sum / (int) keys.size();

		for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++)
		{
			cv::Rect r = iter->getRect();
			if (r.width + r.height < average)
			{
				keys.erase(iter);
				iter--;
				continue;
			}
		}
	}
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
		start = (int) kb::NOTE::E4;

	for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
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

void setWhiteKeyVector(cv::Mat& source, cv::Mat& roi, std::vector<kb::Key>& keys, cv::Rect rect) {

	cv::Mat image;

	cv::cvtColor(roi, image, CV_BGR2GRAY);

	cv::Mat binary_adaptive, canny;
	adaptiveThreshold(image, binary_adaptive, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 21, 10);

	binary_adaptive = 255 - binary_adaptive;
	imshow("binary", binary_adaptive);
	GaussianBlur(binary_adaptive, binary_adaptive, cv::Size(), 2, 2);
	Canny(binary_adaptive, canny, 125, 350);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
	cv::Mat morph;
	morphologyEx(canny, morph, CV_MOP_CLOSE, kernel);

	imshow("morph", morph);


	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::rectangle(morph, cv::Rect(cv::Point(0, 0), cv::Size(morph.size())), cv::Scalar(255), 5);
	cv::findContours(morph, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
;

	std::sort(contours.begin(), contours.end(), cust::compareContourAreas);

	kb::mapKeys(source, image, contours, keys, rect);


	// 흑건까지 찾은 뒤에 해야할 일.
	kb::setMusicalNote(keys);
}