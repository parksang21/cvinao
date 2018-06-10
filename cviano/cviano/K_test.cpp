#include "custom.h"

using namespace std;
using namespace cv;

void heesoo(Mat& destnation) {

	int LowH = 0;
	int HighH = 180;

	int LowS = 0;
	int HighS = 105;

	int LowV = 170;
	int HighV = 255;
	Mat img_input, img_hsv, img_binary;


	Mat frame;
	VideoCapture vc("../../sample_image/test3.mov");
	vc >> frame;

	img_input = imread("whatthe");
	img_input = frame;


	//HSV로 변환
	cvtColor(img_input, img_hsv, COLOR_BGR2HSV);

	//지정한 HSV 범위를 이용하여 영상을 이진화
	inRange(img_hsv, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), img_binary);

	//morphological opening 작은 점들을 제거 
	erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);
	dilate(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);

	//morphological closing 영역의 구멍 메우기 
	dilate(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);
	erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);


	//라벨링 
	Mat img_labels, stats, centroids;
	int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
		stats, centroids, 8, CV_32S);

	cout << "넘 오브 라벨스 : " << numOfLables << endl;
	//영역박스 그리기
	int max = -1, idx = 0;
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, CC_STAT_AREA);
		if (max < area)
		{
			max = area;
			idx = j;
			cout << "idx값 : " << idx << endl;
		}
	}


	int left = stats.at<int>(idx, CC_STAT_LEFT);
	int top = stats.at<int>(idx, CC_STAT_TOP);
	int width = stats.at<int>(idx, CC_STAT_WIDTH);
	int height = stats.at<int>(idx, CC_STAT_HEIGHT);


	rectangle(img_input, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 3);
	//rectangle(img_binary, Point(left, top), Point(left + width, top + height), Scalar(255, 255, 255), 1);

	Rect roiRect(Point2i(left, top), Point2i(left + width, top + height));

	cout << "rectRect : " << roiRect << endl;

	Mat roi = img_binary(roiRect);

	//morphological opening 작은 점들을 제거 
	erode(roi, roi, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 4);
	dilate(roi, roi, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 4);

	//morphological closing 영역의 구멍 메우기 
	dilate(roi, roi, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);
	erode(roi, roi, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), 2);
	//Mat roi = img_input(roiRect);
	Point2i roiLT(roi.cols / 2, roi.rows / 2), roiLB(roi.cols / 2, roi.rows / 2), roiRT(roi.cols / 2, roi.rows / 2), roiRB(roi.cols / 2, roi.rows / 2);

	Point2i temp;
	float distance1 = INT_MAX, distance2 = INT_MAX, distance3 = INT_MAX, distance4 = INT_MAX;

	for (int i = 0; i < roi.rows; i++) {
		for (int j = 0; j < roi.cols; j++) {
			if (roi.at<uchar>(i, j) == 255) {
				temp = Point2i(j, i);
				if (sqrt(pow(temp.x - 0, 2) + pow(temp.y - 0, 2)) < distance1) {
					distance1 = sqrt(pow(temp.x - 0, 2) + pow(temp.y - 0, 2));
					roiLT = temp;
				}
				if (sqrt(pow(temp.x - 0, 2) + pow(temp.y - roi.rows, 2)) < distance2) {
					distance2 = sqrt(pow(temp.x - 0, 2) + pow(temp.y - roi.rows, 2));
					roiLB = temp;
				}
				if (sqrt(pow(temp.x - roi.cols, 2) + pow(0 - temp.y, 2)) < distance3) {
					distance3 = sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - 0, 2));
					roiRT = temp;
					//cout << "변화중인 roiRT 값들 : " << roiRT << endl;
					//cout << "그 점에서의 distance : " << distance3 << endl;

				}
				if (sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - roi.rows, 2)) < distance4) {
					distance4 = sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - roi.rows, 2));
					roiRB = temp;
				}
			}
		}
	}
	cout << "roiLeftTop Point : " << roiLT << endl;
	cout << "roiLeftBot Point : " << roiLB << endl;
	cout << "roiRightTop Point : " << roiRT << endl;
	cout << "roiRightBot Point : " << roiRB << endl;

	cvtColor(roi, roi, CV_GRAY2BGR);
	circle(roi, roiLT, 2, Scalar(0, 0, 255), 2);
	circle(roi, roiLB, 2, Scalar(0, 0, 255), 2);
	circle(roi, roiRT, 2, Scalar(0, 0, 255), 2);
	circle(roi, roiRB, 2, Scalar(0, 0, 255), 2);

	cout << "그 점 값 : " << roi.at<int>(roiLT) << endl;

	Mat realRoi = img_input(roiRect);

	//원본영상좌표
	Point2f pts1[4] = {
		roiLT,roiRT,roiLB,roiRB
	};

	cout << "pts1 : " << pts1[0] << endl;
	//목적영상좌표 좌상,우상,좌하,우하
	Point2f pts2[4] = {
		Point2f(0,0),Point2f(realRoi.cols,0),Point2f(0,realRoi.rows),Point2f(realRoi.cols,realRoi.rows)
	};
	cout << "pts2 : " << pts2[0] << endl;

	Mat dst(realRoi.size(), CV_8UC1);
	Mat perspect_map = getPerspectiveTransform(pts1, pts2);
	warpPerspective(realRoi, dst, perspect_map, realRoi.size(), INTER_CUBIC);
	cout << "[perspect_mat[] = " << endl << perspect_map << endl << endl;

	vector<Point3f>pts3, pts4;
	for (int i = 0; i < 4; i++) {
		pts3.push_back(Point3f(pts1[i].x, pts1[i].y, 1));
	}
	transform(pts3, pts4, perspect_map);

	

	destnation = dst.clone();

}