#include "../midi/MidiFile.h"
#include "../midi/Options.h"
#include "custom.h"
#include "Key.h"
#include <random>
#include <iostream>


using namespace std;
using namespace smf;

void jinsoo(std::vector<std::pair<int, int>>& preNote) {

	
	MidiFile midifile;
	int track = 0;
	int channel = 0;
	int instr = 0;
	midifile.addTimbre(track, 0, channel, 0);
	midifile.absoluteTicks();

	int tpq = midifile.getTPQ();

	/*
	for (int i = 0; i<count; i++) {
	int starttick = int(starttime(mt) / 4.0 * tpq);
	int key = pitch(mt);
	int endtick = starttick + int(duration(mt) / 4.0 * tpq);
	midifile.addNoteOn(track, starttick, channel, key, velocity(mt));
	midifile.addNoteOff(track, endtick, channel, key);
	}*/
	
	for (int i = 0; i < preNote.size(); i++) {

		if (preNote[i + 1].second - preNote[i].second <= 1) {
			midifile.addNoteOn(0, preNote[i].second*tpq, 0, preNote[i].first, 100);
		}
		
		if (preNote[i + 1].second - preNote[i].second > 1) {
			midifile.addNoteOff(0, (preNote[i + 1].second - preNote[i].second)*tpq, 0, preNote[i].first);
		}

	}


	midifile.sortTracks();  // Need to sort tracks since added events are
							// appended to track in random tick order.

	cout << "¹¹³Ä" << preNote[0].first << endl;
	//cout << midifile;
	string filename = "test.mid";

	midifile.write(filename);

	
}