// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main() {

	cv::Mat keyborad;
	std::vector<kb::Key> keys;
	cv::Rect rect;
	cv::Mat frame;
	cv::VideoCapture vc("../../sample_image/test3.mov");
	if (!vc.isOpened()) cout << "비디오 파일이 없습니다." << endl;
	vc >> frame;


	heesoo(frame, keyborad, rect);
	setWhiteKeyVector(frame, keyborad, keys, rect);

	sihyun(keys,vc);

	//cv::imshow("frame", frame);
	//cv::waitKey();

	return 0;
	

}