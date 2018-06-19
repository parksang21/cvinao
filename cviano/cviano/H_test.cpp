#include "custom.h"
#include "Key.h"

using namespace cv;
using namespace std;

void removeHand(Mat bkgImg, Mat binaryImg, Mat& grayImage, Mat hand) {
	for (int i = 0; i < binaryImg.rows; i++) {
		for (int j = 0; j < binaryImg.cols; j++) {
			if (*binaryImg.ptr<uchar>(i, j) == 255 && *hand.ptr<uchar>(i, j) != 255) *grayImage.ptr<uchar>(i, j) = 255;
		}
	}
}

void sihyun(std::vector<kb::Key> keys,	cv::VideoCapture vc, std::vector<std::pair<int, int>>& preNote)
{	

	Mat bkgImg, ycrvb1, ycrvb2, hand, grayImg1, grayImg2, diffImg, frame1, frame2;

	vc >> bkgImg; //get first frame
	cvtColor(bkgImg, grayImg2, CV_BGR2GRAY);
	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);

	Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
	int nThreshold = 50;
	int NoFrame = 0;

	while (1) {
		if (NoFrame % 2 == 0) { 
		vc >> frame1; 
		if (!frame1.data) break;
		cvtColor(frame1, grayImg1, CV_BGR2GRAY);
		//cvtColor(frame1, ycrvb1, CV_BGR2YCrCb);
		//inRange(ycrvb1, Scalar(0, 138, 79), Scalar(255, 162, 121), ycrvb1);
		//imshow("1", grayImg1);
		//removeHand(bkgImg, diffImg, backBoard, ycrvb1);
		//absdiff(grayImg1, bkgImg, diffImg);
		}
		else { 
		vc >> frame2; 	
		if (!frame2.data) break;
		cvtColor(frame2, grayImg2, CV_BGR2GRAY);
		//cvtColor(frame2, ycrvb2, CV_BGR2YCrCb);
		//inRange(ycrvb2, Scalar(0, 138, 79), Scalar(255, 162, 121), ycrvb2);
		//imshow("2", grayImg2);

		//removeHand(bkgImg, diffImg, backBoard, ycrvb2);
		//absdiff(grayImg2, bkgImg, diffImg);
		}

		Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
		absdiff(grayImg1, grayImg2, diffImg);
		threshold(diffImg, diffImg, nThreshold, 255, CV_THRESH_BINARY);
		resize(diffImg, diffImg, diffImg.size() / 2);
		imshow("이전 프레임과 비교", diffImg);
		//cvtColor(backBoard, backBoard, CV_GRAY2BGR);
	
		for (int i = 0; i < keys.size(); i++) { //건반의 타건 여부를 체크하는 함수를 건반 인스턴스의 갯수만큼 호출한다.
			if (keys[i].detectPress(backBoard))preNote.push_back(make_pair(keys[i].getNote(), NoFrame));
		}
		
		/*

		resize(backBoard, backBoard, Size(1080, 720));
		imshow("remove", backBoard);

		for (int i = 0; i < ycrvb.rows; i++) {
			for (int j = 0; j < ycrvb.cols; j++) {
				if (*ycrvb.ptr<uchar>(i, j) == 255) *backBoard.ptr<uchar>(i, j) = *frame.ptr<uchar>(i, j);
			}
		}
		*/
		//imshow("aa", backBoard);

		/*
		resize(backBoard, backBoard, Size(1080, 720));
		imshow("hand", backBoard);
		*/

		char chKey = cvWaitKey(10);
		if (chKey == 27) {
			break;
		}
		//cout << NoFrame << endl;
		NoFrame++;
	}
	for (int i = 0; i < preNote.size(); i++) {
		cout << "["<<preNote[i].first <<", "<<preNote[i].second<<"]"<< endl;
	}
	waitKey();
}