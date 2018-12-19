#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxArtnet.h"
#include "Tributary.h"
#include "ofxGui.h"
#include "ofxButtons.h"

#define OFF 11
#define DEFAULT 0
#define WHITE	1
#define SOLID	2
#define CUSTOM_GRADIENT 3
#define RAINBOW 4
#define RAINBOW_SOLID 5

class ofApp : public ofBaseApp{

	public:

		// built-in to openframeworks
		void setup();
		void update();
		void exit();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);



		// simulation
		void setupSimulation();
		void updateSimulation();
		void drawSimulation();
		void pulseGradient(int num);
		void drawTributary(int ind);
		void setRandomPulse(int ms, int ps, int sep);
		bool pulsing;
		uint64_t lastChecked;
		int pulseTime;
		int numTracked;
		ofColor gradientColors[9];


		// SVG
		ofxSVG svg;
		float step;
		vector<Tributary> tributaries;
		vector<ofPoint> points;

		//DECLARE AN ARTNET NODE
		ofxArtnet anNode;
		//DECLARE DMX DATA = 512 BYTES
		unsigned char dmxData[8][512];
		void setDMXTributaries();

		// button
		ButtonManager buttons;
		void initButtons();
		bool bReset;
		int numSelect;
		int modeSelect;
		void setMode();
		void changeMode();
		float modeTime;
		int modeDuration;
		bool off;

		ofxPanel gui;
		ofxToggle offLights;


		// colors
		float storedColors[9][3];
		float storedDefaultColors[4][3];
		float storedGradientColors[4][3];
		float storedBrightness[4];
		float storedSolid[3];

		void initColors();
		void setStoredColors();
		void setGradientColors();
        void setBrightnessColors(int trib);
		void setBrightnessSections();
		void setGradientListColors();
		void initSettings();
		void setModeColors();

		void drawRainbow();
		void drawRainbowGradient();
		void drawSolid();

		// transitioning
		bool transitioning;
		ofColor previousColors[9];
		void transitionColors();
		void pulseGradientTransition(int num);
		bool checkTransitioning();
		void startTransitioning();

        // drawing
        void drawGradientFlow();
        void drawPacket();
        void drawLabels();
		void setDefaultColors();
		void drawSolid(int r, int g, int b);
		void drawSolid(ofColor c);
    
   
		float speedVal;
		int lightsMode = 0;
	
};
