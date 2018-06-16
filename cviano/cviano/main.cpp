// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main2() {
	// 커밋 전에는 항상 메인 주석처리 하자!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;

	cv::VideoCapture vc("../../sample_image/test3.mov");

	vc >> frame;

	//sihyun();

	//획득한 전체영상에서 흰부분 기준으로 건반의 위치에 사각형
	detectKeyboard(frame, keyborad, keyboard_rect);

	//필요치는 않을것으로 예상되나, 사각형안의 건반에 대하여 삐뚤어졌을경우 똑바로 세워주는것
	
	distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	//cv::imshow("frame", frame);
	cv::waitKey();
	
	

	return 0;
}