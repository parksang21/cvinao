// this is main() function of the whole project
#include "custom.h"
#include "Key.h"

int main() {
	// Ŀ�� ������ �׻� ���� �ּ�ó�� ����!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;
	std::vector <std::pair<int,int>> preNote, output_note;
	cv::VideoCapture vc("../../sample_image/test3.mov");

	vc >> frame;

	cv::resize(frame, frame, cv::Size(1920, 1080));

	//sihyun();

	//ȹ���� ��ü���󿡼� ��κ� �������� �ǹ��� ��ġ�� �簢��
	//detectKeyboard(frame, keyborad, keyboard_rect);

	
	detectKeyboard2(frame);

	//�ʿ�ġ�� ���������� ����ǳ�, �簢������ �ǹݿ� ���Ͽ� �߶Ծ�������� �ȹٷ� �����ִ°�
	
	//distortionRotation(frame,keyborad,keyboard_rect);

	//setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	//cv::imshow("frame", frame);
	//cv::waitKey();
	
	
	sihyun(keys, vc,preNote);
	return 0;
}