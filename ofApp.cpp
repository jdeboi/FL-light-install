#include "ofApp.h"

int previewWidth = 640;
int previewHeight = 480;

//--------------------------------------------------------------
void ofApp::setup() {
    // Building Data DMX frame
    // DMX Frame is 512 bytes with value 0 to 255. Here we set every channel to 0.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 512; j++) {
			dmxData[i][j] = 0;
		}
    }
    
    
    initButtons();
	
	initSettings();
    
    anNode.setup("10.206.235.123");
    
    setupSimulation();
    ofSetWindowShape(previewWidth * 2, previewHeight * 2);
   
	gui.setup(); // most of the time you don't need a name
	gui.add(offLights.setup("OFF", false));

    ofSetFrameRate(30);
	off = true;
   
}





//--------------------------------------------------------------
void ofApp::update() {
        setDMXTributaries();
    //changeMode();
        for (int i = 0; i < 8; i++) {
            anNode.sendDmx("10.206.235.122", 0x0, i, dmxData[i], 512);
        }
}

void ofApp::setDMXTributaries() {
    int decoderUnis[] = {
        0,
        0,
        0,
        0,
        1,
        1,
        2,
        2,
        2,
        2,
        2,
        3,
        3,
        3,
        3,
        3,
        4,
        4,
        4,
        5,
        5,
        5,
        5,
        5,
        6,
        6,
        6,
        6,
        6,
        7,
        7
    };
    int uniIndex[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for (int i = 0; i < 31; i++) {
        int u = decoderUnis[i];
        for (int j = 0; j < tributaries[i].pixels.size(); j++) {
            if (uniIndex[u] < 512 - 3) {
                dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getRed();
                dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getGreen();
                dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getBlue();
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
	setModeColors();

    glPushMatrix();
      ofTranslate(100, 100);
	  if (!off) drawSolid(0, 0, 0);
	  else if (lightsMode == DEFAULT) drawPacket();
	  else if (lightsMode == WHITE || lightsMode == SOLID) drawSolid();
	  else if (lightsMode == CUSTOM_GRADIENT || lightsMode == RAINBOW) drawGradientFlow();
	  else if (lightsMode == RAINBOW_SOLID) drawRainbow();
    drawLabels();
    glPopMatrix();
	

	//gui.draw();
	
}

void ofApp::setModeColors() {
	if (lightsMode == DEFAULT) setDefaultColors();
	else if (lightsMode == WHITE) {
		gradientColors[0] = ofColor(255);
		setStoredColors();
	}
	else if (lightsMode == SOLID) {
		gradientColors[0] = ofColor(storedSolid[0], storedSolid[1], storedSolid[2]);
		setStoredColors();
	}
	else if (lightsMode == CUSTOM_GRADIENT) setGradientListColors();
	else if (lightsMode == RAINBOW) {
		gradientColors[0] = ofColor(255, 0, 0);
		gradientColors[1] = ofColor(150, 255, 0);
		gradientColors[2] = ofColor(0, 255, 50);
		gradientColors[3] = ofColor(0, 50, 255);
		setStoredColors();
	}
}


void ofApp::drawGradientFlow() {
    pulseGradient(numSelect);
}

void ofApp::setGradientListColors() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			gradientColors[i][j] = storedGradientColors[i][j];
		}
	}
	setGradientColors();
}

/*void ofApp::drawSolid() {
	drawSolid(storedColors[0][0], storedColors[0][1], storedColors[0][2]);
}*/

void ofApp::drawSolid(int r, int g, int b) {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].draw(ofColor(r, g, b));
	}
}

void ofApp::drawSolid() {
	for (int i = 0; i < tributaries.size(); i++) {
		setBrightnessColors(i);
		tributaries[i].draw(gradientColors[0]);
	}
}


void ofApp::drawPacket() {
    updateSimulation();
	setDefaultColors();
    for (int i = 0; i < tributaries.size(); i++) {
		setBrightnessColors(i);
        tributaries[i].drawPacket(gradientColors);
    }
}

void ofApp::drawLabels() {
    for (int i = 0; i < tributaries.size(); i++) {
        tributaries[i].drawLabel();
    }
}

//--------------------------------------------------------------
void ofApp::setupSimulation() {
    ofSetFrameRate(40);
    ofBackground(0);
    ofSetColor(0);
    
    
    // SVG
    vector<ofPolyline> outlines;
    svg.load("lightsupdate.svg");
    
    /////// SET THE TRIBS
    vector<int> pathOrder = {
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        30,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29
        
    };
    int pixelsPerTrib[] = {
        24,
        22,
        15,
        85,
        110,
        49,
        37,
        39,
        32,
        24,
        22,
        39,
        15,
        24,
        24,
        20,
        137,
        15,
        15,
        15,
        73,
        20,
        24,
        15,
        24,
        85,
        24,
        12,
        15,
        61,
        48
    };
    int directions[] = {
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1
    };
    for (int k = 0; k < svg.getNumPath(); k++) {
        ofPath& mypath = svg.getPathAt(pathOrder[k]);
        mypath.scale(0.5, .5);
        const vector<ofPolyline>& polylines = mypath.getOutline();
		//  pixelsPerTrib[pathOrder[k]-1]
		// google doc sorted from 1-30
		// svgs sorted from 0->14, 1
        tributaries.push_back(*new Tributary(k, pathOrder[k], polylines[0], pixelsPerTrib[k], directions[k]));
        cout << polylines[0].getPerimeter() << endl;
    }
    
    //////// SET GROUP
    for (int i = 0; i < tributaries.size(); i++) {
        tributaries[i].setGroup(i);
    }
    
    gradientColors[0] = ofColor(0);
    gradientColors[1] = ofColor(0);
    gradientColors[2] = ofColor(0, 255, 0);
    gradientColors[3] = ofColor(0, 0, 255);
}

//--------------------------------------------------------------
void ofApp::setBrightnessColors(int trib) {
	int slideVal = 0;
    if (trib == 15) slideVal = storedBrightness[2];
	else if (trib < 7) slideVal = storedBrightness[0];
	else if (trib < 16) slideVal = storedBrightness[1];
	else if (trib < 20) slideVal = storedBrightness[1];
	else slideVal = storedBrightness[2];

	for (int i = 0; i < 9; i++) {
		int br = slideVal;
		ofColor s = ofColor(storedColors[i][0], storedColors[i][1], storedColors[i][2]);
		if (s.getBrightness() == 0) br = 0;
		gradientColors[i] = ofColor::fromHsb(s.getHue(), s.getSaturation(), br);
	}
}

void ofApp::drawRainbow() {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].draw(ofColor::fromHsb(i*1.0/tributaries.size()*255, 255, 255));
	}
}


void ofApp::drawRainbowGradient() {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].draw();
	}
}

void ofApp::setDefaultColors() {
	numSelect = 4;
	gradientColors[0] = ofColor(0, 0, 0);
	gradientColors[1] = ofColor(0, 0, 0);
	gradientColors[2] = ofColor(0, 0, 255);
	gradientColors[3] = ofColor(0, 255, 0);
	setStoredColors();
}
//--------------------------------------------------------------
void ofApp::updateSimulation() {

        for (int i = 0; i < tributaries.size(); i++) {
            tributaries[i].updatePacket(100-speedVal);
        }
}

//--------------------------------------------------------------

void ofApp::initButtons() {
	buttons.rowwidth = 1000;
	numSelect = 4;
	buttons.setup(); // this sets up the events etc..
		
	ButtonPanel * panel1 = buttons.addButtonPanel("  Set Mode  ");
	panel1->addToggleItem("ON/OFF", off);
	panel1->addSelectionItem("Default", DEFAULT, lightsMode);
	panel1->addSelectionItem("White", WHITE, lightsMode);
	panel1->addSelectionItem("Solid Color", SOLID, lightsMode);
	//panel1->addSelectionItem("Rainbow Solid", RAINBOW_SOLID, lightsMode);
	panel1->addSelectionItem("Rainbow Gradient", RAINBOW, lightsMode);
	panel1->addSelectionItem("Custom Gradient", CUSTOM_GRADIENT, lightsMode);
	//panel1->addSelectionItem("Random Gradient Pulse", GRADIENT, modeSelect);


	ButtonPanel * panel2 = buttons.addButtonPanel("  Settings  ");
	panel2->addSliderItem("Speed", 0, 255, speedVal);
	panel2->addListItem("Brightness ");
	panel2->addSliderItem("Bright1", 0, 255, storedBrightness[0]);
	panel2->addSliderItem("Bright2", 0, 255, storedBrightness[1]);
	panel2->addSliderItem("Bright3", 0, 255, storedBrightness[2]);

	ButtonPanel * panel3 = buttons.addButtonPanel("  Solid Colors  ");
	panel3->addListItem("Color ");
	panel3->addSliderItem("R", 0, 255, storedSolid[0]);
	panel3->addSliderItem("G", 0, 255, storedSolid[1]);
	panel3->addSliderItem("B", 0, 255, storedSolid[2]);

	ButtonPanel * panel4 = buttons.addButtonPanel("  Gradient Colors  ");
	for (int i = 0; i < 4; i++) {
		panel4->addListItem("Color " + to_string(i));
		panel4->addSliderItem("R", 0, 255, storedGradientColors[i][0]);	
		panel4->addSliderItem("G", 0, 255, storedGradientColors[i][1]);
		panel4->addSliderItem("B", 0, 255, storedGradientColors[i][2]);
	}


	
}

void ofApp::pulseGradient(int num) {
    for (int i = 0; i < tributaries.size(); i++) {
		setBrightnessColors(i);
        tributaries[i].pulseGradient(num, 100 - speedVal, previousColors, gradientColors);
    }
}
void ofApp::exit() {

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
}
//--------------------------------------------------------------
bool ofApp::checkTransitioning() {
    for (int i = 0; i < tributaries.size(); i++) {
        if (tributaries[i].transitioning) return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::startTransitioning() {
    for (int i = 0; i < tributaries.size(); i++) {
        tributaries[i].setGradientTransition();
    }
}

//--------------------------------------------------------------
void ofApp::setMode() {
	// this mode will last between 45s and 2min
	for (int i = 0; i < 9; i++) {
		previousColors[i] = gradientColors[i];
	}
	
	// TODO
	setStoredColors();

}

//--------------------------------------------------------------
void ofApp::changeMode() {
    if (ofGetElapsedTimef() < modeTime) modeTime = 0;
    else if (ofGetElapsedTimef() > modeTime + modeDuration && !checkTransitioning()) {
        modeSelect = int(ofRandom(64));
        modeTime = ofGetElapsedTimef();
        modeDuration = int(ofRandom(1, 11));
        //modeDuration = 1;
        startTransitioning();
        setMode();
    }
}

//--------------------------------------------------------------

void ofApp::initSettings() {
	initColors();
	 storedBrightness[0] = 15;
	 storedBrightness[1] = 15;
	 storedBrightness[2] = 15;
	 storedBrightness[3] = 0;
	storedSolid[0] = 150;
	storedSolid[1] = 50;
	storedSolid[2] = 10;
	speedVal = 27;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			storedGradientColors[i][j] = i * 25 + (2 - j) * 50;
		}
	}
}
void ofApp::initColors() {
    transitioning = false;
    numSelect = 4;
    gradientColors[0] = ofColor(153, 153, 255);
    gradientColors[1] = ofColor(178, 102, 255);
    gradientColors[2] = ofColor(255, 51, 255);
    gradientColors[3] = ofColor(255, 0, 127);
    gradientColors[4] = ofColor(204, 229, 255);
    gradientColors[5] = ofColor(153, 255, 255);
    gradientColors[6] = ofColor(102, 255, 178);
    gradientColors[7] = ofColor(51, 255, 51);
    gradientColors[8] = ofColor(128, 255, 0);
    
    setStoredColors();
}

//--------------------------------------------------------------
void ofApp::setGradientColors() {
    for (int i = 0; i < 9; i++) {
        gradientColors[i] = ofColor(storedGradientColors[i][0], storedGradientColors[i][1], storedGradientColors[i][2]);
    }
	setStoredColors();
}

//--------------------------------------------------------------
void ofApp::setStoredColors() {
    for (int i = 0; i < 9; i++) {
        storedColors[i][0] = gradientColors[i].r;
        storedColors[i][1] = gradientColors[i].g;
        storedColors[i][2] = gradientColors[i].b;
    }
}



