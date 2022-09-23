#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update() {
	o.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	o.drawGui();
	o.draw();
}