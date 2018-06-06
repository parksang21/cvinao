// keyboard의 건반 값을 각각 ROI로 맵핑하는 코드
#include "custom.h"
#include "key.h"
#include "LaplacianZC.h"


using namespace std;
using namespace cv;


int main() {
	Mat image;
	String test1 = "../../sample_image/test1.jpg";
	String test2 = "../../sample_image/test2.jpg";
	String test3 = "../../sample_image/test3.jpg";
	String test6 = "../../sample_image/test6.jpg";
	String test7 = "../../sample_image/side.jpg";

	image = imread(test7, 0);
	CV_Assert(image.data);

	Mat binary_adaptive, canny;
	adaptiveThreshold(image, binary_adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 21, 10);

	binary_adaptive = 255 - binary_adaptive;

	GaussianBlur(binary_adaptive, binary_adaptive, Size(), 2, 2);
	Canny(binary_adaptive, canny, 125, 350);

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(17, 17));
	Mat morph;
	morphologyEx(canny, morph, CV_MOP_CLOSE, kernel);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	rectangle(morph, Rect(Point(0, 0), Size(morph.size())), Scalar(255), 5);
	findContours(morph, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours.begin(), contours.end(), cust::compareContourAreas);

	Mat only_poly(canny.size(), CV_8UC3);
	drawContours(only_poly, contours, -1, Scalar(0, 0, 255), 5);


	Mat rois(image.size(), CV_8UC3);
	cvtColor(image, rois, CV_GRAY2BGR);

	vector<kb::Key> keys;

	kb::mapKeys(rois, contours, keys);

	/*
	int a = 0;
	for (kb::Key k : keys) {
		cout << k.getRect().tl() << "  " << k.getRect().area() << endl;
		imshow(to_string(a), k.roi);
		a++;
	}*/


	
	namedWindow("roi", WINDOW_NORMAL);
	resizeWindow("roi", rois.size());
	imshow("roi", rois);

	namedWindow("morp", WINDOW_NORMAL);
	resizeWindow("morp", morph.size());
	imshow("morp", morph);
	
	waitKey();

	return 0;
}