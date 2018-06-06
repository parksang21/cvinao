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

	image = imread(test3, 0);
	CV_Assert(image.data);

	Mat binary_adaptive, canny;
	adaptiveThreshold(image, binary_adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 21, 10);

	binary_adaptive = 255 - binary_adaptive;

	GaussianBlur(binary_adaptive, binary_adaptive, Size(), 2, 2);
	Canny(binary_adaptive, canny, 125, 350);

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
	Mat morph;
	morphologyEx(canny, morph, CV_MOP_CLOSE, kernel);

	size_t idx, i;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(morph, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours.begin(), contours.end(), cust::compareContourAreas);

	Mat only_poly(canny.size(), CV_8UC3);

	Mat rois(image.size(), CV_8UC3);
	cvtColor(image, rois, CV_GRAY2BGR);

	vector<kb::Key> keys;

	kb::mapKeys(rois, contours, keys);

	namedWindow("roi", WINDOW_NORMAL);

	resizeWindow("roi", rois.size());

	imshow("roi", rois);
	waitKey();

	return 0;
}