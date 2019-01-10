#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetColor(0);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->fbo.begin();
	ofClear(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-bold.ttf", 300, true, true, true);
	string word = "CO";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 - font.stringHeight(word) * 0.1);
	word = "DE";
	font.drawString(word, ofGetWidth() * 0.5 - font.stringWidth(word) * 0.5, ofGetHeight() * 0.5 + font.stringHeight(word) * 1.1);

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	this->number_of_agent = 150;
	this->len_of_tail = 18;
	while (this->logs.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			deque<glm::vec2> log;
			log.push_front(glm::vec2(x, y));
			this->logs.push_back(log);
			this->directions.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
			ofColor color;
			color.setHsb(ofRandom(255), 255, 230);
			this->colors.push_back(color);
		}
	}

	ofSetLineWidth(1.5);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future = this->logs[i].front() + this->directions[i] * 5;
		int x = future.x;
		int y = future.y;

		while (this->pixels.getColor(x, y) == ofColor(0, 0)) {

			this->directions[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

			future = this->logs[i].front() + this->directions[i] * 10;
			x = future.x;
			y = future.y;
		}

		this->logs[i].push_front(future);
		while (this->logs[i].size() > this->len_of_tail) {

			this->logs[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->number_of_agent; i++) {

		for (int l = 0; l < this->logs[i].size() - 1; l++) {

			ofSetColor(255, ofMap(l, 0, this->len_of_tail, 255, 0));
			ofDrawLine(this->logs[i][l], this->logs[i][l + 1]);
		}

		ofSetColor(255);
		ofDrawCircle(this->logs[i].front(), 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}