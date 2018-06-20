#include "Key.h"

#include "MidiFile.h"

kb::Key::Key() {};

kb::Key::Key(cv::Rect& r) {
	rect = r;
}

kb::Key::Key(cv::Mat& src, cv::Rect& r) {
	rect = r;
	oroi = src(r);
};

kb::Key::Key(cv::Mat& src, cv::Rect& r, std::vector<cv::Point>& c, int music){
	rect = r;
	contour = c;
	oroi = src(r);
	key_type = music;
}

kb::Key::Key(cv::Mat& image, std::vector<cv::Point>& contour) {

}

kb::Key::~Key() {}

bool kb::Key::detectPress(cv::Mat diffVideo) {
	//imshow("tesasdfasdf", mask);
	//imshow("asdfasdf", roi);
	//cv::waitKey(0);
	unsigned int RR= this->getRoi().rows;
	unsigned int CC = this->getRoi().cols;
	this->setRoi(diffVideo);
	//imshow("getRoi", this->getRoi());
	for (unsigned int i = 0; i < RR; i++) {
		for (unsigned int j = 0; j <CC; j++) {
			if (*roi.ptr<uchar>(i, j) ==255) {
				return true;
			}
		}
	}
	return false;
}


cv::Rect kb::Key::getRect() 
{
	return rect;
}

void kb::Key::setNote(int note) 
{
	this->note = note;
}

int kb::Key::getNote() 
{
	return note;
}

std::vector<cv::Point> kb::Key::getContour() 
{
	return contour;
}

void kb::Key::setORoi(cv::Mat source)
{
	oroi = source(rect);
}

void kb::Key::setORoi(cv::Mat source, cv::Rect rect_roi)
{
	oroi = source(rect_roi);
}

cv::Mat kb::Key::getORoi(){	return oroi;}

void kb::Key::setRoi(cv::Mat source){ roi = source(rect) & mask;}

cv::Mat kb::Key::getRoi() {	return roi;}

void kb::Key::setMask()
{
	cvtColor(oroi.clone(), mask, CV_BGR2GRAY);
	mask -= 255;
	std::vector<std::vector<cv::Point>> adjust_conts;
	std::vector<cv::Point> adjust_cont;
	for (int i = 0; i < contour.size(); i++)
	{
		adjust_cont.push_back(contour[i] - rect.tl());
	}
	adjust_conts.push_back(adjust_cont);
	cv::drawContours(mask, adjust_conts, -1, cv::Scalar(255), CV_FILLED);
	cv::drawContours(mask, adjust_conts, -1, cv::Scalar(0), 3);
	cv::Mat test_mask(mask.size(), CV_8UC1, cv::Scalar(0));
	
	cv::drawContours(test_mask, adjust_conts, -1, cv::Scalar(255), CV_FILLED);
	cv::drawContours(test_mask, adjust_conts, -1, cv::Scalar(0), 20);
	mask = mask - test_mask;

}

cv::Mat kb::Key::getMask(){	return mask; }

// ==================================================================================
//			클래스 외 function
// ==================================================================================

void kb::mapKeys(cv::Mat& source, cv::Mat& image, std::vector<std::vector<cv::Point>>& contours, std::vector<kb::Key>& keys, cv::Rect srect)
{
	// to calculate the contour area's average
	int cont_sum = 0, avg_cont = 0;

	// to calculate the sum of contour area
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		cont_sum += cv::contourArea(*iter);
		if (cv::contourArea(*iter) >= (double)image.size().area() / 2)
		{
			contours.erase(iter);
			cont_sum = 0;
			iter = contours.begin();
		}
	}

	// calculate the average of contour area.
	avg_cont = cont_sum / contours.size();


	// delete the unwanted contours
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		if (cv::contourArea(*iter) < avg_cont)
		{
			contours.erase(iter);
			iter--;
		}
	}
	
	// to adjust to the origin image
	cv::Point origin = srect.tl();
	for (std::vector<std::vector<cv::Point>>::iterator iter = contours.begin();
		iter < contours.end();
		iter++)
	{
		cv::Rect r = boundingRect(*iter);
		r.height *= 0.95;
		r += origin;

		for (std::vector<cv::Point>::iterator point = iter->begin();
			point < iter->end(); point++) {
			*point += origin;
		}

		kb::Key key(source, r, *iter, WHITE_KEY);
		keys.push_back(key);
	}

	// key 값 소팅
	std::sort(keys.begin(), keys.end(), kb::compareKeys);

}

bool kb::compareKeys(kb::Key key1, kb::Key key2) 
{
	int i = key1.getRect().tl().x;
	int j = key2.getRect().tl().x;
	return (i < j);
}

void kb::setMusicalNote(std::vector<kb::Key>& keys) 
{
	int key_num = (int) keys.size();
	int start = 0;

	if (key_num == 22)
		start = (int) kb::NOTE::F2;

	for (std::vector<kb::Key>::iterator iter = keys.begin(); iter < keys.end(); iter++) 
	{
		if (start % 12 == 6 || start % 12 == 8 || start % 12 == 10 || start % 12 == 1 || start % 12 == 3)
			start++;

		iter->setNote(start);
		start++;
	}

}

void kb::drawKeys(cv::Mat& image, std::vector<kb::Key> keys)
{
	
	for (int i = 0; i < keys.size(); i++)
	{
		cv::rectangle(image, keys[i].getRect(), cv::Scalar(255, 0, 0), 2);
		cv::putText(image,
			cv::String(std::to_string(keys[i].getNote())),
			keys[i].getRect().tl() + cv::Point(0, 20),
			2,
			1.2,
			cv::Scalar(0, 0, 255));
	}
}

void setWhiteKeyVector(cv::Mat& source, cv::Mat& roi, std::vector<kb::Key>& keys, cv::Rect rect) 
{

	cv::Mat image;

	cv::cvtColor(roi, image, CV_BGR2GRAY);

	cv::Mat binary_adaptive, canny;
	adaptiveThreshold(image, binary_adaptive, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 21, 10);

	binary_adaptive = 255 - binary_adaptive;

	// display :: binary_adaptive image;
	// imshow("binary", binary_adaptive);

	GaussianBlur(binary_adaptive, binary_adaptive, cv::Size(), 2, 2);
	Canny(binary_adaptive, canny, 125, 350);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15, 15));
	cv::Mat morph;
	morphologyEx(canny, morph, CV_MOP_CLOSE, kernel);

	// display :: morphed image
	// imshow("morph", morph);


	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::rectangle(morph, cv::Rect(cv::Point(0, 0), cv::Size(morph.size())), cv::Scalar(255), 5);
	cv::findContours(morph, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
;

	std::sort(contours.begin(), contours.end(), cust::compareContourAreas);

	kb::mapKeys(source, image, contours, keys, rect);

	// 흑건까지 찾은 뒤에 해야할 일.
	kb::setMusicalNote(keys);

	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].setORoi(source);
		// cv::imshow(std::to_string(i), keys[i].getORoi());
		keys[i].setMask();
		// cv::imshow("mask " + std::to_string(i), keys[i].getMask());
	}

	// to show cont
	/*
	cv::Mat cont(source);
	cv::drawContours(source, contours, -1, cv::Scalar(255, 255, 0), 3);
	kb::drawKeys(source, keys);
	imshow("cont", source);
	cv::waitKey();
	*/
	
}

void detectKeyboard(cv::Mat& sorce, cv::Mat& destnation, cv::Rect& rect) {

	int LowH = 0;
	int HighH = 120;

	int LowS = 0;
	int HighS = 150;  //4,5,6에 대해서 150

	int LowV = 120;     // 4,5,6에 대해서 150  7에 대해서 80
	int HighV = 240;      //4,5,6에 대해서 255 
	cv::Mat img_input, img_hsv, img_binary;

	int LowR = 160;    //4,5,6에 대해서 RGB모두 150~255  7번에 대하여 160
	int HighR = 255;

	int LowG = 150;     // 7번에 대하여 150
	int HighG = 255;

	int LowB = 150;     // 7번에 대하여 150
	int HighB = 255;


	img_input = sorce;


	//HSV로 변환
	cv::cvtColor(img_input, img_hsv, cv::COLOR_BGR2HSV);

	//지정한 HSV 범위를 이용하여 영상을 이진화
	//inRange(img_hsv, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), img_binary);
	inRange(img_input, cv::Scalar(LowR, LowG, LowB), cv::Scalar(HighR, HighG, HighB), img_binary);

	//morphological opening 작은 점들을 제거 
	erode(img_binary, img_binary, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 1);
	dilate(img_binary, img_binary, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 1);

	//morphological closing 영역의 구멍 메우기 
	dilate(img_binary, img_binary, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 5);  // 7번에 대하여 반복 5
	erode(img_binary, img_binary, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)), cv::Point(-1, -1), 1); //7번에 대하여 size 7,7  반복 1


																									   //라벨링 
	cv::Mat img_labels, stats, centroids;
	int numOfLables = connectedComponentsWithStats(img_binary, img_labels, stats, centroids, 8, CV_32S);


	//영역박스 그리기
	int max = -1, idx = 0;
	for (int j = 1; j < numOfLables; j++) {
		int area = stats.at<int>(j, cv::CC_STAT_AREA);
		if (max < area)
		{
			max = area;
			idx = j;

		}
	}


	int left = stats.at<int>(idx, cv::CC_STAT_LEFT);
	int top = stats.at<int>(idx, cv::CC_STAT_TOP);
	int width = stats.at<int>(idx, cv::CC_STAT_WIDTH);
	int height = stats.at<int>(idx, cv::CC_STAT_HEIGHT) * 97 / 100;
	top = top + height * 3 / 100;


	cv::rectangle(img_input, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 0, 255), 3);
	//rectangle(img_binary, Point(left, top), Point(left + width, top + height), Scalar(255, 255, 255), 1);


	cv::Rect roiRect(cv::Point2i(left, top), cv::Point2i(left + width, top + height));
	cv::Mat realRoi = img_input(roiRect);
	rect = roiRect;
	destnation = realRoi.clone();

	/*
	imshow("Detected Keboard area", img_input);
	imshow("Binary image", img_binary);
	imshow("Detected Keyboard image", destnation);
	waitKey(0);
	*/

}

void draw_houghLines(cv::Mat image, cv::Mat& dst, std::vector<cv::Vec2f> lines, int nline, std::vector<cv::Point2d>& linePts)
{

	cvtColor(image, dst, CV_GRAY2BGR);
	for (size_t i = 0; i < std::min((int)lines.size(), nline); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		cv::Point2d pt(a*rho, b*rho);
		cv::Point2d delta(10000 * -b, 10000 * a);

		linePts.push_back(pt + delta);
		linePts.push_back(pt - delta);

		line(dst, pt + delta, pt - delta, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	}
}

void distortionRotation(cv::Mat& sorce, cv::Mat& destnation, cv::Rect& rect) {

	cv::Mat img_input;
	img_input = sorce;
	cv::Mat roi = img_input(rect);


	cv::Point2i roiLT(roi.cols / 2, roi.rows / 2), roiLB(roi.cols / 2, roi.rows / 2), roiRT(roi.cols / 2, roi.rows / 2), roiRB(roi.cols / 2, roi.rows / 2);

	cv::Point2i temp;
	float distance1 = INT_MAX, distance2 = INT_MAX, distance3 = INT_MAX, distance4 = INT_MAX;

	for (int i = 0; i < roi.rows; i++) {
		for (int j = 0; j < roi.cols; j++) {
			if (roi.at<uchar>(i, j) != 0) {
				temp = cv::Point2i(j, i);
				if (sqrt(pow(temp.x - 0, 2) + pow(temp.y - 0, 2)) <= distance1) {
					distance1 = sqrt(pow(temp.x - 0, 2) + pow(temp.y - 0, 2));
					roiLT = temp;
				}
				if (sqrt(pow(temp.x - roi.cols, 2) + pow(0 - temp.y, 2)) <= distance3) {
					distance3 = sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - 0, 2));
					roiRT = temp;
				}
				if (sqrt(pow(temp.x - 0, 2) + pow(temp.y - roi.rows, 2)) <= distance2) {
					distance2 = sqrt(pow(temp.x - 0, 2) + pow(temp.y - roi.rows, 2));
					roiLB = temp;
				}

				if (sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - roi.rows, 2)) <= distance4) {
					distance4 = sqrt(pow(temp.x - roi.cols, 2) + pow(temp.y - roi.rows, 2));
					roiRB = temp;
				}
			}
		}
	}


	//원본영상좌표 좌상,우상,좌하,우하
	cv::Point2f pts1[4] = {
		roiLT + cv::Point2i(rect.x,rect.y),roiRT + cv::Point2i(rect.x,rect.y),roiLB + cv::Point2i(rect.x,rect.y),roiRB + cv::Point2i(rect.x,rect.y)
	};

	/*
	for (int i = 0; i < 4; i++) {
		std::cout << pts1[i] << std::endl;
	}
	*/
	


	//목적영상좌표 좌상,우상,좌하,우하
	cv::Point2f pts2[4] = {
		cv::Point2i(0,0) + cv::Point2i(rect.x,rect.y),cv::Point2i(roi.cols,0) + cv::Point2i(rect.x,rect.y),cv::Point2i(0,roi.rows) + cv::Point2i(rect.x,rect.y),cv::Point2i(roi.cols,roi.rows) + cv::Point2i(rect.x,rect.y)
	};



	cv::Mat dst;


	cv::Mat perspect_map = getPerspectiveTransform(pts1, pts2);
	cv::warpPerspective(roi, dst, perspect_map, roi.size(), cv::INTER_CUBIC);

	destnation = dst.clone();
}

void makeMusicSheet(std::vector<std::pair<int, float>>& notePairVec) {


	smf::MidiFile midifile;
	int track = 0;
	int channel = 0;
	int instr = 0;
	midifile.addTimbre(track, 0, channel, 0);
	midifile.absoluteTicks();

	int tpq = midifile.getTPQ();


	float timeSec = 0;
	for (int i = 0; i < notePairVec.size(); i++) {
		midifile.addNoteOn(0, timeSec*tpq, 0, notePairVec[i].first, 100);
		timeSec += notePairVec[i].second;
		midifile.addNoteOff(0, timeSec*tpq, 0, notePairVec[i].first);
	}


	midifile.sortTracks();  // Need to sort tracks since added events are
							// appended to track in random tick order.

	std::string filename = "test.mid";

	midifile.write(filename);
}

void makeNote(std::vector<std::pair<int, int>>& preNote, std::vector<std::pair<int, float>>& output_note) {

	std::pair<int, int> start(0, 0), end(0, 0);
	for (std::vector<std::pair<int, int>>::iterator iter = preNote.begin(); iter < preNote.end(); iter++)
	{
		if (iter == preNote.begin())
		{
			start = *iter;
			continue;
		}

		if (iter->first != start.first)
		{
			end = *(iter - 1);
			if (start == end)
			{
				iter = preNote.erase(--iter);
			}
			start = *iter;
		}
	}

	start = std::make_pair(0, 0);
	end = std::make_pair(0, 0);
	for (std::vector<std::pair<int, int>>::iterator iter = preNote.begin(); iter < preNote.end(); iter++)
	{
		if (iter == preNote.begin())
		{
			start = *iter;
			continue;
		}

		if (iter->first != (iter - 1)->first || iter == preNote.end() - 1)
		{
			end = *(iter - 1);
			output_note.push_back(std::make_pair(start.first, 0.5 * (end.second - start.second) / 5));
			start = *iter;
		}
		else if (iter->second - (iter - 1)->second > 3)
		{
			end = *(iter - 1);
			output_note.push_back(std::make_pair(start.first, 0.5 * (end.second - start.second) / 5));
			start = *iter;
		}
	}
}



void removeHand(cv::Mat binaryImg, cv::Mat& grayImage, cv::Mat hand) {
	for (int i = 0; i < binaryImg.rows; i++) {
		for (int j = 0; j < binaryImg.cols; j++) {
			if (*binaryImg.ptr<uchar>(i, j) == 255 && *hand.ptr<uchar>(i, j) != 255) *grayImage.ptr<uchar>(i, j) = 255;
		}
	}
}

void preProcess(std::vector<kb::Key> keys, cv::VideoCapture vc, std::vector<std::pair<int, int>>& preNote)
{
	int totalF = vc.get(cv::CAP_PROP_FRAME_COUNT);

	cv::Mat bkgImg, ycrvb1, hand, grayImg1, diffImg, frame1;

	vc >> bkgImg; //get first frame

	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);

	int nThreshold = 50;
	int NoFrame = 0;

	while (1) {

		vc >> frame1;
		if (!frame1.data) break;
		cvtColor(frame1, grayImg1, CV_BGR2GRAY);
		cvtColor(frame1, ycrvb1, CV_BGR2YCrCb);
		inRange(ycrvb1, cv::Scalar(0, 138, 79), cv::Scalar(255, 162, 121), ycrvb1);
		dilate(ycrvb1, ycrvb1, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 20);
		absdiff(grayImg1, bkgImg, diffImg);
		threshold(diffImg, diffImg, nThreshold, 255, CV_THRESH_BINARY);
		cv::Mat backBoard(bkgImg.size(), bkgImg.depth(), cv::Scalar(0));
		removeHand(diffImg, backBoard, ycrvb1);

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
				preNote.push_back(std::make_pair(keys[i].getNote(), NoFrame));
				// cv::rectangle(backBoard, keys[i].getRect(), cv::Scalar(255, 255, 255), 3);
			}
			// imshow("roi" + std::to_string(i), keys[i].getRoi());
		}

		char chKey = cvWaitKey(1);
		if (chKey == 27) {
			break;
		}

		system("cls");
		std::cout << NoFrame * 100 / totalF << "%" << std::endl;

		NoFrame++;
	}
}