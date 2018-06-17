#include "custom.h"
#include "Key.h"

#include "MidiFile.h"
#include "Options.h"
#include <random>


#define Cn1 0
#define CSn1 1
#define Dn1 2
#define DSn1 3
#define En1 4
#define Fn1 5
#define FSn1 6
#define Gn1 7
#define GSn1 8
#define An1 9
#define ASn1 10
#define Bn1 11


#define C0 12
#define CS0 13
#define D0 14
#define DS0 15
#define E0 16
#define F0 17
#define FS0 18
#define G0 19
#define GS0 20
#define A0 21
#define AS0 22
#define B0 23

#define C1 24
#define CS1 25
#define D1 26
#define DS1 27
#define E1 28
#define F1 29
#define FS1 30
#define G1 31
#define GS1 32
#define A1 33
#define AS1 34
#define B1 35

#define C2 36
#define CS2 37
#define D2 38
#define DS2 39
#define E2 40
#define F2 41
#define FS2 42
#define G2 43
#define GS2 44
#define A2 45
#define AS2 46
#define B2 47

#define C3 48
#define CS3 49
#define D3 50
#define DS3 51
#define E3 52
#define F3 53
#define FS3 54
#define G3 55
#define GS3 56
#define A3 57
#define AS3 58
#define B3 59

#define C4 60
#define CS4 61
#define D4 62
#define DS4 63
#define E4 64
#define F4 65
#define FS4 66
#define G4 67
#define GS4 68
#define A4 69
#define AS4 70
#define B4 71

#define C5 72
#define CS5 73
#define D5 74
#define DS5 75
#define E5 76
#define F5 77
#define FS5 78
#define G5 79
#define GS5 80
#define A5 81
#define AS5 82
#define B5 83

#define C6 84
#define CS6 85
#define D6 86
#define DS6 87
#define E6 88
#define F6 89
#define FS6 90
#define G6 91
#define GS6 92
#define A6 93
#define AS6 94
#define B6 95

#define C7 96
#define CS7 97
#define D7 98
#define DS7 99
#define E7 100
#define F7 101
#define FS7 102
#define G7 103
#define GS7 104
#define A7 105
#define AS7 106
#define B7 107




using namespace std;
using namespace smf;
using namespace cv;


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


void sihyun(std::vector<kb::Key> keys, cv::VideoCapture vc)
{

	Mat bkgImg, frame, ycrvb, hand, grayImg, diffImg;;

	vc >> bkgImg; //get first frame

	cvtColor(bkgImg, grayImg, CV_BGR2GRAY);
	cvtColor(bkgImg, bkgImg, CV_BGR2GRAY);
	Mat backBoard(bkgImg.size(), bkgImg.depth(), Scalar(0));
	int nThreshold = 50;
	vector<string> note;
	char Cstack = 0;
	char Dstack = 0;
	char Estack = 0;
	char Gstack = 0;
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
		if (Dstack == 3) {
			note.push_back("레"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		if (Estack == 3) {
			note.push_back("미"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
		}
		if (Gstack == 3) {
			note.push_back("솔"); Cstack = 0; Dstack = 0; Estack = 0; Gstack = 0; Xstack = 0;
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

int main(int argc, char** argv) {

	MidiFile midifile;
	int track = 0;
	int channel = 0;
	int instr = 0;
	midifile.addTimbre(track, 0, channel, 0);

	int tpq = midifile.getTPQ();

	/*
	for (int i = 0; i<count; i++) {
	int starttick = int(starttime(mt) / 4.0 * tpq);
	int key = pitch(mt);
	int endtick = starttick + int(duration(mt) / 4.0 * tpq);
	midifile.addNoteOn(track, starttick, channel, key, velocity(mt));
	midifile.addNoteOff(track, endtick, channel, key);
	}*/


	midifile.addNoteOn(0, 0 * tpq, 0, E4, 100);
	midifile.addNoteOff(0, 1.5 * tpq, 0, E4);

	midifile.addNoteOn(0, 1.5 * tpq, 0, D4, 100);
	midifile.addNoteOff(0, 2 * tpq, 0, D4);

	midifile.addNoteOn(0, 2 * tpq, 0, C4, 100);
	midifile.addNoteOff(0, 3 * tpq, 0, C4);

	midifile.addNoteOn(0, 3 * tpq, 0, D4, 100);
	midifile.addNoteOff(0, 4 * tpq, 0, D4);




	midifile.sortTracks();  // Need to sort tracks since added events are
							// appended to track in random tick order.


	cout << midifile;
	string filename = "test.mid";

	midifile.write(filename);

	return 0;
}
