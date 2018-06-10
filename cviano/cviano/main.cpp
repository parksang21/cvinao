// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main() {


	cv::Mat image;
	std::vector<kb::Key> keys;
	cv::Rect rect;
	cv::Mat frame;
	cv::VideoCapture vc("../../sample_image/test3.mov");
	vc >> frame;

	//sihyun();

	heesoo(frame, image, rect);
	setWhiteKeyVector(frame, image, keys, rect);

	cv::imshow("frame", frame);
	cv::waitKey();

	return 0;
}