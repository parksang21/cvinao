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

kb::Key::Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& c){
	rect = r;
	contour = c;
	roi = src(r);
}

kb::Key::Key(cv::Mat& image, std::vector<cv::Point>& contour) {

}

kb::Key::~Key() {}

bool kb::Key::detectPress(cv::Mat diffVideo) {
	unsigned int hitCount = 0; // how many pixels change
	unsigned int criticalPoint = 0; //if hitCount is bigger than this value, we assume that key is pressed

	unsigned int TopLeftX = this->getRect().x;
	unsigned int TopLeftY = this->getRect().y;
	unsigned int KeyWidth = TopLeftX + this->getRect().size().width;
	unsigned int KeyHeight = TopLeftY + this->getRect().size().height;

	for (unsigned int i = TopLeftX; i < KeyWidth; i++) {
		for (unsigned int j = TopLeftY; j < KeyHeight; j++) {
			if (*diffVideo.ptr<uchar>(j, i) == 255) hitCount++;
			if (hitCount >= criticalPoint) return true;
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


// ==================================================================================
//			클래스 외 function
// ==================================================================================

void kb::mapKeys(cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys) {

	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin(); 
		iter < contours.end(); 
		iter++) {
		cv::Rect r = boundingRect(*iter);
		kb::Key key(image, r, *iter);
		keys.push_back(key);
	}


	// 작은 것들 삭제하기
	for (vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) {
		if (iter->getRect().area() < image.size().area() / keys.size() || 
			iter->getRect().area() > image.size().area() / 2) {
			keys.erase(iter);
			iter--;
			continue;
		}

		cv::RNG rng(12345);
		cv::rectangle(image, iter->getRect(), 
				cv::Scalar(rng.uniform(0, 256), 
				rng.uniform(0, 256), 
				rng.uniform(0, 256)), 
				2);
	}

	// key 값 정제하기

	std::sort(keys.begin(), keys.end(), kb::compareKeys);

	for (int i = 0; i < keys.size(); i++) {
		cv::putText(image, 
				cv::String(std::to_string(i)), 
				keys[i].getRect().tl()+cv::Point(0,20), 
				2, 
				1.2, 
				cv::Scalar(0, 0, 255));
	}
}


bool kb::compareKeys(kb::Key key1, kb::Key key2) {
	int i = key1.getRect().tl().x;
	int j = key2.getRect().tl().x;
	return (i < j);
}
