// this is main() function of the whole project
#include "custom.h"
#include "Key.h"

#include <queue>

int sumPair(std::pair<int, int> pairs[])
{
	int sum = 0;
	for (int i = 0; i < 4; i++) sum += pairs[i].first;
	return sum;
}

void makeNote(std::vector<std::pair<int, int>>& preNote, std::vector<std::pair<int, float>>& output_note) {

	std::pair<int, int> start(0, 0), end(0, 0);
	for (std::vector<std::pair<int,int>>::iterator iter = preNote.begin(); iter < preNote.end(); iter++)
	{
		if (iter == preNote.begin())
		{
			start = *iter;
			std::cout << "start : " << start.first << "," << start.second << std::endl;
			continue;
		}

		if (iter->first != start.first)
		{
			end = *(iter-1);
			std::cout << "end : " << start.first << "," << start.second << std::endl;
			if (start == end)
			{
				iter = preNote.erase(--iter);
			}
			start = *iter;
		}
	}

	start = std::make_pair(0, 0);
	end = std::make_pair(0, 0);
	for (std::vector<std::pair<int, int>>::iterator iter = preNote.begin(); iter < preNote.end(); iter++)
	{
		if (iter == preNote.begin())
		{
			start = *iter;
			continue;
		}

		if (iter->first != (iter - 1)->first || iter == preNote.end() - 1)
		{
			end = *(iter - 1);
			output_note.push_back(std::make_pair(start.first, 0.5 * (end.second - start.second) / 5));
			start = *iter;
		}
		else if (iter->second - (iter-1)->second > 3)
		{
				end = *(iter - 1);
				output_note.push_back(std::make_pair(start.first, 0.5 * (end.second - start.second) / 5));
				start = *iter;
		}
	}
}

int main() {
	// 커밋 전에는 항상 메인 주석처리 하자!

	cv::Mat keyborad, frame;
	std::vector<kb::Key> keys;
	cv::Rect keyboard_rect;
	std::vector <std::pair<int,int>> preNote;
	std::vector <std::pair<int, float>> output_note;

	std::string test3 = "../../sample_image/test3.mov";
	std::string test4 = "../../sample_image/test4.mov";
	std::string test5 = "../../sample_image/test5.mov";
	std::string test6 = "../../sample_image/test6.mov";
	std::string test7 = "../../sample_image/test7.mov";
	cv::VideoCapture vc(test7);

	vc >> frame;

	//cv::resize(frame, frame, cv::Size(1920, 1080));

	//획득한 전체영상에서 흰부분 기준으로 건반의 위치에 사각형
	detectKeyboard(frame, keyborad, keyboard_rect);


	// to test keyboard detection
	
	imshow("SDAf", frame);
	imshow("vv", keyborad);
	cv::waitKey(0);
	


	//detectKeyboard2(frame);

	//필요치는 않을것으로 예상되나, 사각형안의 건반에 대하여 삐뚤어졌을경우 똑바로 세워주는것
	
	//distortionRotation(frame,keyborad,keyboard_rect);

	setWhiteKeyVector(frame, keyborad, keys, keyboard_rect);

	//cv::imshow("frame", frame);
	//cv::waitKey();
	
	//도레미파솔의 sens 값을 하드코딩 해보자
	sihyun(keys, vc,preNote);
	makeNote(preNote, output_note);
	std::cout << "====================pre===========================" << std::endl;
	for (int i = 0; i < preNote.size(); i++) {
		std::cout << "[" << preNote[i].first << ", " << preNote[i].second << "]" << std::endl;
	}
	
	std::cout << "=====================out==========================" << std::endl;
	for (int i = 0; i < output_note.size(); i++) {
		std::cout << "[" << output_note[i].first << ", " << output_note[i].second << "]" << std::endl;
	}
	
	return 0;
}