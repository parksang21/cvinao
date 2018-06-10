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
	setWhiteKeyVector(image, keys,rect);

	cv::rectangle(frame, keys[0].getRect(), cv::Scalar(0, 255, 0), 3);

	cv::imshow("frame", frame);
	cv::imshow("image", image);
	cv::waitKey();

	return 0;
}