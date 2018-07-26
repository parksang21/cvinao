// this is main() function of the whole project

#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>

#include "Key.h"

int main() {
	// Ŀ�� ������ �׻� ���� �ּ�ó�� ����!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;
	std::vector <std::pair<int,int>> preNote;
	std::vector <std::pair<int, float>> output_note;

	
	std::string test1 = "../../sample_image/test4.mov";
	std::string test2 = "../../sample_image/test5.mov";
	// user �Լ��� ��ο� ������ ���� �̸��� �־��ּ���
	std::string user = "../../sample_image/test2.mov";
	cv::VideoCapture vc(test2);

	vc >> frame;

	//ȹ���� ��ü���󿡼� ��κ� �������� �ǹ��� ��ġ�� �簢��
	detectKeyboard(frame, keyborad, keyboard_rect);

	//�ʿ�ġ�� ���������� ����ǳ�, �簢������ �ǹݿ� ���Ͽ� �߶Ծ�������� �ȹٷ� �����ִ°�
	//distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	
	//�������ļ��� sens ���� �ϵ��ڵ� �غ���
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