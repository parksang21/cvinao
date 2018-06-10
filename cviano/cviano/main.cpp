// this is main() function of the whole project
#include "custom.h"
#include "Key.h"


int main() {


	cv::Mat image;
	std::vector<kb::Key> keys;
	//sihyun();

	heesoo(image);
	setWhiteKeyVector(image, keys);

	return 0;
}