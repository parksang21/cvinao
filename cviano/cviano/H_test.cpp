#include "custom.h"

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


void sihyun()
{	
	
	VideoCapture vc("../../sample_image/test3.MOV");
	if (!vc.isOpened()) cout << "비디오 파일이 없습니다." << endl;

	Mat bkgImg, frame, grayImg2, ycrvb, hand;
	vc >> bkgImg;

	Mat grayImg, diffImg;
	cvtColor(bkgImg, grayImg, CV_BGR2GRAY);
	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);

	int nThreshold = 50;
	int frameN = 0;
	while (1) {
		vc >> frame;
		if (frame.data == NULL) break;
		Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
		cvtColor(frame, grayImg, CV_BGR2GRAY);
		cvtColor(frame, grayImg2, CV_BGR2GRAY);
		cvtColor(frame, ycrvb, CV_BGR2YCrCb);
		inRange(ycrvb, Scalar(0, 133, 77), Scalar(255, 173, 127), ycrvb);

		absdiff(grayImg, bkgImg, diffImg);
		threshold(diffImg, diffImg, nThreshold, 255, CV_THRESH_BINARY);
		removeHand(bkgImg, diffImg, backBoard, ycrvb);
		cvtColor(backBoard, backBoard, CV_GRAY2BGR);
		rectangle(backBoard,keyC, Scalar(0, 0, 255), 3); 
		rectangle(backBoard, keyD, Scalar(0, 0, 255), 3);
		rectangle(backBoard, keyE, Scalar(0, 0, 255), 3);
		rectangle(backBoard, keyG, Scalar(0, 0, 255), 3);


		if (hitnoteD(backBoard) == 1) cout << "레" << endl;
		else if (hitnoteE(backBoard) == 1) cout << "미" << endl;
		else if (hitnoteG(backBoard) == 1) cout << "솔" << endl;
		else if (hitnoteC(backBoard) == 1) cout << "도" << endl;
		else cout << "X" << endl;

		resize(backBoard, backBoard, Size(1080, 720));
		imshow("remove", backBoard);

		char chKey = cvWaitKey(1);
		if (chKey == 27) {
			break;
		}
	}
	waitKey();
}