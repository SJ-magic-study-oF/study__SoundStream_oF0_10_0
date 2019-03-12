/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/


/******************************
******************************/
ofApp::ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId)
: b_EnableAudioOut(false)
, soundStream_Input_DeviceId(_soundStream_Input_DeviceId)
, soundStream_Output_DeviceId(_soundStream_Output_DeviceId)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("study:SoundStream");
	// ofSetVerticalSync(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	
	/********************
	settings.setInListener(this);
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.bufferSize = bufferSize;
	
	soundStream.setup(settings);
	********************/
	ofSoundStreamSettings settings;
	
	soundStream.printDeviceList();
	
	if( soundStream_Input_DeviceId == -1 ){
		ofExit();
		return;
		
	}else{
		vector<ofSoundDevice> devices = soundStream.getDeviceList();
		
		settings.setInDevice(devices[soundStream_Input_DeviceId]);
		
		if( soundStream_Output_DeviceId != -1 ){
			settings.setOutDevice(devices[soundStream_Output_DeviceId]);
			b_EnableAudioOut = true;
			
			/*
			if(devices[soundStream_Output_DeviceId].name == "Apple Inc.: Built-in Output"){
				printf("!!!!! prohibited to use [%s] for output ... by SJ for safety !!!!!\n", devices[soundStream_Output_DeviceId].name.c_str());
			}else{
				settings.setOutDevice(devices[soundStream_Output_DeviceId]);
				b_EnableAudioOut = true;
			}
			*/
		}
	
		settings.setInListener(this);
		settings.sampleRate = AUDIO_SAMPLERATE;
		
		if(b_EnableAudioOut){
			settings.numOutputChannels = AUDIO_BUFFERS;
			settings.setOutListener(this); /* Don't forget this */
		}else{
			settings.numOutputChannels = 0;
		}
		
		settings.numInputChannels = AUDIO_BUFFERS;
		settings.bufferSize = AUDIO_BUF_SIZE;
	}
	
	/********************
	********************/
	vol_l.assign(AUDIO_BUF_SIZE, 0.0);
	vol_r.assign(AUDIO_BUF_SIZE, 0.0);
	
	// fft_thread->setup();
	
	/********************
	soundStream.setup()の位置に注意:最後
		setup直後、audioIn()/audioOut()がstartする.
		これらのmethodは、fft_threadにaccessするので、start前にReStart()によって、fft_threadが初期化されていないと、不正accessが発生してしまう.
	********************/
	soundStream.setup(settings);
	
	/********************
	********************/
	printf("> start process\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::update(){
}

/******************************
******************************/
void ofApp::draw(){
	ofBackground(30);
}

/******************************
audioIn/ audioOut
	同じthreadで動いている様子。
	また、audioInとaudioOutは、同時に呼ばれることはない(多分)。
	つまり、ofAppからaccessがない限り、変数にaccessする際にlock/unlock する必要はない。
	ofApp側からaccessする時は、threadを立てて、安全にpassする仕組みが必要(cf:NotUsed__thread_safeAccess.h)
	...ReferenceとSetで違う変数を用意し、このpassはthreadの向こう側で行う。
******************************/
void ofApp::audioIn(ofSoundBuffer & buffer){
	for (size_t i = 0; i < buffer.getNumFrames(); i++){
		vol_l[i] = buffer[i*2];
		vol_r[i] = buffer[i*2+1];
	}
}

/******************************
******************************/
void ofApp::audioOut(ofSoundBuffer & buffer){
	for (size_t i = 0; i < buffer.getNumFrames(); i++){
		if(b_EnableAudioOut){
			buffer[i*2]		= vol_l[i];
			buffer[i*2 + 1]	= vol_r[i];
		}else{
			buffer[i*2]		= 0;
			buffer[i*2 + 1]	= 0;
		}
	}
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 's':
			soundStream.start();
			break;
			
		case 'e':
			soundStream.stop();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
