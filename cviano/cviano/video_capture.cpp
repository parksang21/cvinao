// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main() {

	cv::VideoCapture stream1(1);

	// 커밋 전에는 항상 메인 주석처리 하자!

	cv::Mat keyboard, frame;
	std::vector<kb::Key> keys;
	
	cv::Rect  keyboard_rect;

	stream1.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	stream1.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	stream1.read(frame);

	//cv::Rect keyboard_rect(cv::Point(20, frame.rows/3), cv::Size(frame.cols/40*38, frame.rows / 20 * 7));


	while (true) {
		stream1.read(frame);
		//cv::rectangle(frame, keyboard_rect, cv::Scalar(0, 0, 255), 2);
		imshow("cam", frame);
		if (cv::waitKey(30) >= 0)
			break;
	}

	

	//keyboard = frame(keyboard_rect);

	detectKeyboard(frame, keyboard, keyboard_rect);

	setWhiteKeyVector(frame, keyboard, keys, keyboard_rect);





	return 0;
}