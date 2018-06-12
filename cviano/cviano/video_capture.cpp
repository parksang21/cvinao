// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main3() {

	cv::VideoCapture stream1(3);

	// Ŀ�� ������ �׻� ���� �ּ�ó�� ����!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;

	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);
	stream1.read(frame);

	std::cout << frame.rows << std::endl;
	std::cout << frame.cols << std::endl;
	//sihyun();


	detectKeyboard(frame, keyborad, keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	cv::imshow("frame", frame);
	cv::waitKey();


	while (true) {
		cv::Mat cameraFrame;
		stream1.read(cameraFrame);
		imshow("cam", cameraFrame);
		if (cv::waitKey(30) >= 0)
			break;
	}

	return 0;
}