#include "../midi/MidiFile.h"
#include "../midi/Options.h"
#include "custom.h"
#include "Key.h"
#include <random>
#include <iostream>


using namespace std;
using namespace smf;

void makeMusicSheet(std::vector<std::pair<int, float>>& notePairVec) {

	
	MidiFile midifile;
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
	
	string filename = "test.mid";

	midifile.write(filename);
}