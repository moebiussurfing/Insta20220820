#pragma once

#include "ofMain.h"
#include "WaveformObject.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	WaveformObject o;
};