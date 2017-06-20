#include "Pokitto.h"
#include "Arrays.h"
#include "FileIO.h"
#include "Synth.h"
Pokitto::Core pok;
//BLOCK block[30];
//global variables
uint8_t screenW = 220, screenH = 176, frameRate = 30, fontW = 5, fontH = 7, screen = 0, buttonRepeatFrame = 2;
//tracker class
#include "Tracker.h"
Tracker tracker;
//Patch settings mode (screen 1)
#include "PatchSettings.h"
//main
int main(){
	pok.begin();
	pokInitSD();
	pok.display.width = screenW;
	pok.display.height = screenH;
	pok.display.setFont(font5x7);
	pok.setFrameRate(frameRate);
	tracker.fillArrays(); // there are 30 blocks and all block have 64 pitch and patch storage. Fill them -1
	while(pok.isRunning()){
        if (screen){ //Settings mode
            if(pok.update()){
                checkButtons();
                printSettings();
                drawPointer();
            }
        }
        else{ //Tracker mode
            if (tracker.play){
                tracker.playTracker();
            }
            if(pok.update()){
                tracker.checkButtons();
                tracker.drawIsPlaying();
                tracker.drawLines();
                tracker.printValues();
                tracker.printSettings();
                tracker.drawPointer();
            }
        }
	}
}
