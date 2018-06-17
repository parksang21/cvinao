#include "custom.h"
#include "Key.h"
using namespace cv;
using namespace std;

Rect keyE(Point(1145, 357), Size(69, 250));
Rect keyD(Point(1076, 357), Size(69, 250));
Rect keyC(Point(1007, 357), Size(69, 250));
Rect keyG(Point(1283, 357), Size(69, 250));

void removeHand(Mat bkgImg, Mat binaryImg, Mat& grayImage, Mat hand) {
	for (int i = 0; i < binaryImg.rows; i++) {
		for (int j = 0; j < binaryImg.cols; j++) {
			if (*binaryImg.ptr<uchar>(i, j) == 255 && *hand.ptr<uchar>(i, j) != 255) *grayImage.ptr<uchar>(i, j) = 255;
		}
	}
}

int hitnoteE(Mat video1) {
	int thresh = 0;
	int count = 0;

	Point ktl = keyE.tl();
	thresh = keyE.size().width *keyE.size().height / 45;

	for (int i = ktl.x; i < ktl.x + keyE.size().width; i++) {
		for (int j = ktl.y; j < ktl.y + keyE.size().height; j++) {
			if (*video1.ptr<uchar>(j, i) == 255) {
				count++;

			}
		}
	}
	if (count >= thresh) return 1;
	return 0;
}

int hitnoteD(Mat video1) {
	int thresh = 0;
	int count = 0;

	Point ktl = keyD.tl();
	thresh = keyD.size().width *keyD.size().height / 20;

	for (int i = ktl.x; i < ktl.x + keyD.size().width; i++) {
		for (int j = ktl.y; j < ktl.y + keyD.size().height; j++) {
			if (*video1.ptr<uchar>(j, i) == 255) {
				count++;

			}
		}
	}

	if (count >= thresh) return 1;
	return 0;
}


int hitnoteG(Mat video1) {
	int thresh = 0;
	int count = 0;

	Point ktl = keyG.tl();
	thresh = keyD.size().width *keyD.size().height / 17;

	for (int i = ktl.x; i < ktl.x + keyD.size().width; i++) {
		for (int j = ktl.y; j < ktl.y + keyD.size().height; j++) {
			if (*video1.ptr<uchar>(j, i) == 255) {
				count++;

			}
		}
	}

	if (count >= thresh) return 1;
	return 0;
}

int hitnoteC(Mat video1) {
	int thresh = 0;
	int count = 0;

	Point ktl = keyC.tl();
	thresh = keyC.size().width *keyC.size().height / 20;

	for (int i = ktl.x + 5; i < ktl.x + keyC.size().width - 5; i++) {
		for (int j = ktl.y + 5; j < ktl.y + keyC.size().height - 5; j++) {
			if (*video1.ptr<uchar>(j, i) == 255) {
				count++;

			}
		}
	}

	if (count >= thresh) return 1;
	return 0;
}


void sihyun(std::vector<kb::Key> keys,	cv::VideoCapture vc)
{	

	Mat bkgImg, frame, ycrvb, hand, grayImg, diffImg;;

	vc >> bkgImg; //get first frame

	cvtColor(bkgImg, grayImg, CV_BGR2GRAY);
	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);
	Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
	int nThreshold = 50;
	vector<string> note;
	char Cstack=0;
	char Dstack=0;
	char Estack=0;
	char Gstack=0;
	char Xstack = 0;
	while (1) {
		vc >> frame;
		if (frame.data == NULL) break;
		Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));

		cvtColor(frame, grayImg, CV_BGR2GRAY);
		cvtColor(frame, ycrvb, CV_BGR2YCrCb);

		inRange(ycrvb, Scalar(0, 139, 79), Scalar(255, 161, 121), ycrvb);

		absdiff(grayImg, bkgImg, diffImg); // 현 프레임의 grayscale 영상과 첫 프레임과의 비교  
		threshold(diffImg, diffImg, nThreshold, 255, CV_THRESH_BINARY);
		removeHand(bkgImg, diffImg, backBoard, ycrvb);
		cvtColor(backBoard, backBoard, CV_GRAY2BGR);
		rectangle(backBoard, keyC, Scalar(0, 0, 255), 3);
		rectangle(backBoard, keyD, Scalar(0, 0, 255), 3);
		rectangle(backBoard, keyE, Scalar(0, 0, 255), 3);
		rectangle(backBoard, keyG, Scalar(0, 0, 255), 3);


		for (int i = 0; i < keys.size() - 1; i++) {

		}
		if (hitnoteE(backBoard)) { cout << "미" << endl; Estack++; }
		else if (hitnoteD(backBoard)) { cout << "레" << endl; Dstack++; }
		else if (hitnoteC(backBoard)) { cout << "도" << endl; Cstack++; }
		else if (hitnoteG(backBoard)) { cout << "솔" << endl; Gstack++; }
		else { cout << "X" << endl; Xstack++; }
		if (Cstack == 3) {
			note.push_back("도"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		if (Dstack == 3) { note.push_back("레"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		if (Estack == 3) { note.push_back("미"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		if (Gstack == 3) { note.push_back("솔"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		//if (Xstack == 8) { note.push_back("X"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		//}

		resize(backBoard, backBoard, Size(1080, 720));
		imshow("remove", backBoard);
		
		/*
		for (int i = 0; i < ycrvb.rows; i++) {
			for (int j = 0; j < ycrvb.cols; j++) {
				if (*ycrvb.ptr<uchar>(i, j) == 255) *backBoard.ptr<uchar>(i, j) = *frame.ptr<uchar>(i, j);
			}
		}
		*/
		//resize(backBoard, backBoard, Size(1080, 720));
		//imshow("hand", backBoard);
		char chKey = cvWaitKey(1);
		if (chKey == 27) {
			break;
		}
	}
	for (String n : note) {
		cout << n << endl;
	}

	waitKey();
}