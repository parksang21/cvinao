// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main() {
	// Ŀ�� ������ �׻� ���� �ּ�ó�� ����!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;

	cv::VideoCapture vc("../../sample_image/test3.mov");

	vc >> frame;

	//sihyun();

	//ȹ���� ��ü���󿡼� ��κ� �������� �ǹ��� ��ġ�� �簢��
	detectKeyboard(frame, keyborad, keyboard_rect);


	std::cout << "������" << frame.cols << "   " << frame.rows<<std::endl;
	//�ʿ�ġ�� ���������� ����ǳ�, �簢������ �ǹݿ� ���Ͽ� �߶Ծ�������� �ȹٷ� �����ִ°�
	
	//distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	//cv::imshow("frame", frame);
	//cv::waitKey();
	
	
	//sihyun(keys, vc);
	return 0;
}