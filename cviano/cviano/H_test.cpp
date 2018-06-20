#include "custom.h"
#include "Key.h"

using namespace cv;
using namespace std;

void removeHand(Mat binaryImg, Mat& grayImage, Mat hand) {
	for (int i = 0; i < binaryImg.rows; i++) {
		for (int j = 0; j < binaryImg.cols; j++) {
			if (*binaryImg.ptr<uchar>(i, j) == 255 && *hand.ptr<uchar>(i, j) != 255) *grayImage.ptr<uchar>(i, j) = 255;
		}
	}
}

void sihyun(std::vector<kb::Key> keys,	cv::VideoCapture vc, std::vector<std::pair<int, int>>& preNote)
{	

	Mat bkgImg, ycrvb1, hand, grayImg1, diffImg, frame1;

	vc >> bkgImg; //get first frame

	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);

	int nThreshold = 50;
	int NoFrame = 0;

	while (1) {
	
		vc >> frame1;
		if (!frame1.data) break;
		cvtColor(frame1, grayImg1, CV_BGR2GRAY);
		cvtColor(frame1, ycrvb1, CV_BGR2YCrCb);
		inRange(ycrvb1, Scalar(0, 138, 79), Scalar(255, 162, 121), ycrvb1);
		dilate(ycrvb1, ycrvb1, getStructuringElement(MORPH_ELLIPSE, Size(5,5)), Point(-1, -1), 20);
		absdiff(grayImg1, bkgImg, diffImg);
		threshold(diffImg, diffImg, nThreshold, 255, CV_THRESH_BINARY);
		Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
		removeHand(diffImg, backBoard, ycrvb1);
		//cv::imshow("back", backBoard);

	//	absdiff(grayImg1, grayImg2, diffImg);
	
		//resize(diffImg, diffImg, diffImg.size() / 2);
		
		//cvtColor(backBoard, backBoard, CV_GRAY2BGR);


		/*
		std::vector<std::vector<cv::Point>> adjust_conts;
		for (int i = 0; i < keys.size(); i++)
		{
			adjust_conts.push_back(keys[i].getContour());
		}
	
		cv::drawContours(diffImg, adjust_conts, -1, cv::Scalar(255), 3);
		imshow("vide", diffImg);
		*/
		/*
		for (int i = 0; i < keys.size(); i++)
		{
			cv::namedWindow("roi" + std::to_string(i));
			cv::moveWindow("roi" + std::to_string(i), i * 100, 30);

		}

		*/
		//건반의 타건 여부를 체크하는 함수를 건반 인스턴스의 갯수만큼 호출한다.
		for (int i = 0; i < keys.size(); i++) 
		{
			if (keys[i].detectPress(backBoard))
			{
				preNote.push_back(make_pair(keys[i].getNote(), NoFrame));
				// cv::rectangle(backBoard, keys[i].getRect(), cv::Scalar(255, 255, 255), 3);
			}
			// imshow("roi" + std::to_string(i), keys[i].getRoi());
		}
		
		/*
		for (int i = 0; i < keys.size(); i++)
		{
			keys[i].setRoi(backBoard);
			//keys[i].detectPress(backBoard);
			unsigned int RR = keys[i].getRoi().rows;
			unsigned int CC = keys[i].getRoi().cols;
			for (unsigned int i = 0; i < RR; i++) {
				for (unsigned int j = 0; j < CC; j++) {
					if (*(keys[i].getRoi()).ptr<uchar>(i, j) == 255) {
						preNote.push_back(make_pair(keys[i].getNote(), NoFrame));
					}
				}
				imshow("roi" + std::to_string(i), keys[i].getRoi());
			}
		}
		*/
		//imshow("video", diffImg);
		
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
		for (int i = 0; i < preNote.size(); i++) {
		cout << "["<<preNote[i].first <<", "<<preNote[i].second<<"]"<< endl;
		}
		*/

		char chKey = cvWaitKey(1);
		if (chKey == 27) {
			break;
		}
		if(NoFrame%100==0)	cout << NoFrame << endl;
		NoFrame++;
	}
}