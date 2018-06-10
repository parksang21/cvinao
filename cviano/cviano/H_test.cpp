#include "custom.h"

using namespace cv;
using namespace std;

Rect keyE(Point(1147, 395), Size(92, 624));
Rect keyD(Point(1055, 395), Size(92, 624));
Rect keyC(Point(963, 395), Size(92, 624));
Rect keyF(Point(1331, 395), Size(92, 624));

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
	thresh = keyE.size().width *keyE.size().height / 11;

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
	thresh = keyD.size().width *keyD.size().height / 6;

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


int hitnoteF(Mat video1) {
	int thresh = 0;
	int count = 0;

	Point ktl = keyF.tl();
	thresh = keyD.size().width *keyD.size().height / 11;

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
	thresh = keyC.size().width *keyC.size().height / 18;

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
		rectangle(backBoard, Point(1143, 335), Point(1210, 700), Scalar(0, 0, 255), 3);
//		rectangle(backBoard, Point(1055, 395), Point(1147, 1019), Scalar(255, 0, 0), 3);

		if (hitnoteD(backBoard) == 1) cout << "레" << endl;
		else if (hitnoteE(backBoard) == 1) cout << "미" << endl;
		else if (hitnoteF(backBoard) == 1) cout << "솔" << endl;
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