#include "Key.h"

using namespace std;

kb::Key::Key() {};



kb::Key::Key(cv::Rect roi) {
	this->rect = roi;
}

kb::Key::Key(cv::Mat src, cv::Rect r) : cv::Mat(src, r) {
	this->rect = r;
};

kb::Key::Key(cv::Mat src, cv::Rect r, std::vector<cv::Point> contour) : cv::Mat(src, r) {
	this->rect = r;
	this->contour = contour;
}

kb::Key::Key(cv::Mat image, std::vector<cv::Point> contour) {

}

kb::Key::~Key() {}

bool kb::Key::detectPress() {

	return false;
}

cv::Rect kb::Key::getRect() {
	return this->rect;
}

void kb::mapKeys(cv::Mat image, std::vector<std::vector<cv::Point>> contours, std::vector<kb::Key> keys) {

	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin(); iter < contours.end(); iter++) {
		cv::Rect roi = boundingRect(*iter);
		kb::Key key(image, roi, *iter);
		keys.push_back(key);
	}
	

	// 작은 것들 삭제하기
	for (vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) {
		if (iter->getRect().area() < image.size().area() / keys.size()) {
			keys.erase(iter);
			iter--;
			continue;
		}

		cv::RNG rng(12345);
		cv::rectangle(image, iter->getRect(), cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), 2);
	}

	// key 값 정제하기


}

// ==================================================================================
//			클래스 외 function
// ==================================================================================
