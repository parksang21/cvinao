#include "MidiFile.h"
#include "Options.h"
#include <random>
#include <iostream>

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




using namespace std;
using namespace smf;

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
	midifile.addNoteOff(0, 2 * tpq,0, D4);

	midifile.addNoteOn(0, 2 * tpq, 0, C4, 100);
	midifile.addNoteOff(0, 3 * tpq, 0, C4);

	midifile.addNoteOn(0, 3 * tpq, 0, D4, 100);
	midifile.addNoteOff(0, 4 * tpq,0, D4);




	midifile.sortTracks();  // Need to sort tracks since added events are
							// appended to track in random tick order.


	cout << midifile;
	string filename = "test.mid";
	
	midifile.write(filename);

	return 0;
}