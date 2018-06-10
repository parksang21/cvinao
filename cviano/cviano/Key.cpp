#include "Key.h"

using namespace std;

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

bool kb::Key::detectPress() {

	return false;
}

cv::Rect kb::Key::getRect() {
	return rect;
}

cv::Mat kb::Key::getMat() {
	return roi;
}

void kb::Key::setNote(int note) {
	scale = note;
}

int kb::Key::getNote() {
	return scale;
}


// ==================================================================================
//			클래스 외 function
// ==================================================================================

void kb::mapKeys(cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys) 
{
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin(); 
		iter < contours.end(); 
		iter++) 
	{
		cv::Rect r = boundingRect(*iter);
		kb::Key key(image, r, *iter, WHITE_KEY);
		keys.push_back(key);
	}

	int image_size = image.size().area();
	int criteria = image_size / keys.size();
	// 작은 것들 삭제하기
	for (vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
		cv::Rect r = iter->getRect();
		if (r.area() < criteria ||
			r.area() > image_size / 2)
		{
			keys.erase(iter);
			iter--;
			continue;
		}
	}

	// key 값 정제하기

	std::sort(keys.begin(), keys.end(), kb::compareKeys);
	
	if (keys.size() > 22)
	{
		int sum = 0;

		for (vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++)
		{
			sum += iter->getRect().width + iter->getRect().height;
		}

		int average = sum / keys.size();

		for (vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++)
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
	int key_num = keys.size();
	int start = 0;

	if (key_num == 22)
		start = F2;

	for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
		iter->setNote(start);
		start++;

		if (start % 10 > 7) start += 93;
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
