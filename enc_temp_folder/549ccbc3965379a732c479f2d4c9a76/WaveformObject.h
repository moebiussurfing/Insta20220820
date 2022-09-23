#pragma once

#include "ofMain.h"
#include "ofxSurfingImGui.h"

class WaveformObject
{

public:

	WaveformObject::WaveformObject()
	{
		setup();
	}

	WaveformObject::~WaveformObject()
	{
	}

private:

	ofEasyCam cam;
	ofMesh face, frame;

	ofxSurfingGui ui;

	int R = 300;
	int v_span = 8;
	int u_span = 16;

	float g1 = 1.f;
	float g2 = 1.f;
	float g3 = 1.f;
	float g4 = 1.f;
	float control = 0.f;

	float speed = 1.f;
	bool bRotate = false;

	//-

private:

	void setup() {


		this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

		ui.setup();
	};

public:

	void update() {
		ofSeedRandom(39);

		this->face.clear();
		this->frame.clear();

		for (int v = 0; v < 360; v += v_span) {

			for (auto u = 0; u < 360; u += u_span) {

				auto noise_seed = g1 * ofRandom(1000);
				auto r = 80.f;
				auto noise_r = r;
				auto noise_location = this->make_point(R, 90, u + u_span * 0.5, v);

				auto noise_value = g2 * ofNoise(
					noise_location.x * 0.001,
					noise_location.z * 0.001,
					noise_location.y * 0.002 + ofGetFrameNum() * 0.02);

				if (noise_value > 0.5) {

					auto random_noise_value = g3 *
						ofNoise(noise_seed, ofGetFrameNum() * 0.005);

					noise_r = g4 * 80.f +
						ofMap(random_noise_value, 0, 1, 0, 270) *
						ofMap(noise_value, 0.5, 1, 0, 1);
				}

				vector<glm::vec3> vertices;
				vertices.push_back(this->make_point(R, r, u, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u + u_span, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u + u_span, v + v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u, v + v_span * 0.5));

				vertices.push_back(this->make_point(R, noise_r, u + u_span * 0.5, v));

				int index = face.getNumVertices();
				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); face.addIndex(index + 1); face.addIndex(index + 4);
				this->face.addIndex(index + 1); face.addIndex(index + 2); face.addIndex(index + 4);
				this->face.addIndex(index + 2); face.addIndex(index + 3); face.addIndex(index + 4);
				this->face.addIndex(index + 3); face.addIndex(index + 0); face.addIndex(index + 4);

				this->frame.addVertices(vertices);

				this->frame.addIndex(index + 0); this->frame.addIndex(index + 1);
				this->frame.addIndex(index + 1); this->frame.addIndex(index + 2);
				this->frame.addIndex(index + 2); this->frame.addIndex(index + 3);
				this->frame.addIndex(index + 3); this->frame.addIndex(index + 0);

				this->frame.addIndex(index + 0); this->frame.addIndex(index + 4);
				this->frame.addIndex(index + 1); this->frame.addIndex(index + 4);
				this->frame.addIndex(index + 2); this->frame.addIndex(index + 4);
				this->frame.addIndex(index + 3); this->frame.addIndex(index + 4);
			}
		}
	};

public:

	void drawGui() {

		ofDisableDepthTest();

		ui.Begin();

		if (ui.BeginWindow("ofApp")) {
			ui.Add(ui.bMinimize, OFX_IM_TOGGLE_ROUNDED);

			ImGui::SliderFloat("g1", &g1, 0, 1);
			ImGui::SliderFloat("g2", &g2, 0, 1);
			ImGui::SliderFloat("g3", &g3, 0, 1);
			ImGui::SliderFloat("g4", &g4, 0, 1);
			ui.AddSpacingSeparated();
			if (ImGui::SliderFloat("control", &control, 0, 1)) {
				ui.AddToLog("control: " + ofToString(control, 1));

				//g1 = ofMap(control, 0, 1, 0, 1, true);
				g2 = ofMap(control, 0, 1, 0.75, 1, true);
				g3 = ofMap(control, 0, 1, 0.75, 1, true);
				g4 = ofMap(control, 0, 1, 0.75, 1, true);
			};
			ui.AddSpacingSeparated();

			ImGui::SliderInt("R", &R, 10, 1000);
			ui.AddSpacing();
			ImGui::SliderInt("v_span", &v_span, 1, 50);
			ImGui::SliderInt("u_span", &u_span, 1, 50);
			ui.AddSpacingSeparated();

			if (ui.AddToggle("Rotate", bRotate, OFX_IM_TOGGLE)) {
			};
			if (bRotate) ImGui::SliderFloat("speed", &speed, 0, 1);

			static bool b;
			b = cam.getMouseInputEnabled();
			if (ui.AddToggle("mouseCam", b, OFX_IM_TOGGLE)) {
				b ? cam.enableMouseInput() : cam.disableMouseInput();
			};

			ui.AddSpacingSeparated();
			ui.Add(ui.bAdvanced, OFX_IM_TOGGLE_BUTTON_ROUNDED_MINI);

			ui.EndWindow();
		}

		//ui.DrawWindowAdvanced();
		//ui.drawWindowAlignHelpers();
		//ui.DrawWindowLog();

		ui.End();
	};

	void draw() {
		ofEnableDepthTest();

		ofPushStyle();
		ofSetLineWidth(1);

		this->cam.begin();
		{
			if (bRotate) ofRotateY(ofGetFrameNum() * 0.666666 * speed);

			ofSetColor(0);
			this->face.draw();

			ofSetColor(128);
			this->frame.drawWireframe();
		}
		this->cam.end();

		ofPopStyle();
	};

private:

	//--------------------------------------------------------------
	glm::vec3 make_point(float R, float r, float u, float v)
	{
		// 数学デッサン教室 描いて楽しむ数学たち　P.31

		u *= DEG_TO_RAD;
		v *= DEG_TO_RAD;

		auto x = (R + r * cos(u)) * cos(v);
		auto y = (R + r * cos(u)) * sin(v);
		auto z = r * sin(u);

		return glm::vec3(x, y, z);
	};

};

