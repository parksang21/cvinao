// this is main() function of the whole project
#include "custom.h"
#include "Key.h"

int main() {
	// Ŀ�� ������ �׻� ���� �ּ�ó�� ����!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;
	std::vector <std::pair<int,int>> preNote, output_note;

	std::string test4 = "../../sample_image/test4.mov";
	std::string test5 = "../../sample_image/test5.mov";
	std::string test6 = "../../sample_image/test6.mov";
	std::string test7 = "../../sample_image/test7.mov";
	cv::VideoCapture vc(test4);

	vc >> frame;

	cv::resize(frame, frame, cv::Size(1920, 1080));

	//sihyun();

	//ȹ���� ��ü���󿡼� ��κ� �������� �ǹ��� ��ġ�� �簢��
	detectKeyboard(frame, keyborad, keyboard_rect);

	imshow("SDAf", frame);
	imshow("vv", keyborad);
	cv::waitKey(0);
	
	//detectKeyboard2(frame);

	//�ʿ�ġ�� ���������� ����ǳ�, �簢������ �ǹݿ� ���Ͽ� �߶Ծ�������� �ȹٷ� �����ִ°�
	
	//distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	//cv::imshow("frame", frame);
	//cv::waitKey();
	
	//�������ļ��� sens ���� �ϵ��ڵ� �غ���
	//sihyun(keys, vc,preNote);
	return 0;
}