// this is main() function of the whole project

#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>

#include "Key.h"

int main() {
	// 커밋 전에는 항상 메인 주석처리 하자!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;
	std::vector <std::pair<int,int>> preNote;
	std::vector <std::pair<int, float>> output_note;

	
	std::string test1 = "../../sample_image/test4.mov";
	std::string test2 = "../../sample_image/test5.mov";
	// user 함수의 경로에 저장한 파일 이름을 넣어주세요
	std::string user = "../../sample_image/test2.mov";
	cv::VideoCapture vc(test2);

	vc >> frame;

	//획득한 전체영상에서 흰부분 기준으로 건반의 위치에 사각형
	detectKeyboard(frame, keyborad, keyboard_rect);

	//필요치는 않을것으로 예상되나, 사각형안의 건반에 대하여 삐뚤어졌을경우 똑바로 세워주는것
	//distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	
	//도레미파솔의 sens 값을 하드코딩 해보자
	preProcess(keys, vc,preNote);
	makeNote(preNote, output_note);
	makeMusicSheet(output_note);

	/*
	std::cout << "====================pre===========================" << std::endl;
	for (int i = 0; i < preNote.size(); i++) {
		std::cout << "[" << preNote[i].first << ", " << preNote[i].second << "]" << std::endl;
	}
	
	std::cout << "=====================out==========================" << std::endl;
	for (int i = 0; i < output_note.size(); i++) {
		std::cout << "[" << output_note[i].first << ", " << output_note[i].second << "]" << std::endl;
	}
	*/

	return 0;
}