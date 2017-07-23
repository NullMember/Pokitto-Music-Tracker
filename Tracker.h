#ifndef TRACKER_H
#define TRACKER_H

#include "Pokitto.h"
#include "Arrays.h"
#include "Synth.h"

uint8_t screenW = 220, screenH = 176, frameRate = 30, fontW = 5, fontH = 7, screen = 0, buttonRepeatFrame = 4;
uint8_t maxColumn = 3, maxRow = 64;
int8_t colPointer = 0, rowPointer = 0;
int8_t screenMaxInit = 20, screenMin = 0, screenMax = screenMaxInit, screenPointer = 0, playPointer = 0;
int16_t _tempo = 250;
uint32_t playTime = 0;
uint8_t mode = 0; //mode0 = travel mode, mode1 = edit pitches and patches, mode2 = screen1 settings, mode3 = play, pause
int8_t edit = 0;
uint8_t vLines[4] = {((fontW + 1) * 2) + 1, (((fontW + 1) * 9) + vLines[0]) + 1, (((fontW + 1) * 9) + vLines[1]) + 1, (((fontW + 1) * 9) + vLines[2]) + 1}; //(((fontW + 1) * number of chars) + left line) + 1
uint8_t s_vLines[2] = {vLines[3] + 2, 0};
int8_t instrPointer = 0;
int8_t settingPointer = 0;
bool play = 0;
BLOCK blocks[30];

char patchname[15][10];

//function prototypes
void checkButtons();
void tfillArrays();
void tdrawLines();
void tdrawPointer();
void tdrawIsPlaying();
void tprintValues();
void tprintSettings();
void tplayTracker();

void pchangeValues(int8_t i);
void pprintSettings();
void pdrawPointer();
void initTracker();
void Tracker();
void _playNote();
void _playPattern();
void _playSong();
void _pause();

int minMax(int _val, int _min, int _max);
void draw();

void Tracker() {
    checkButtons();
    draw();
}

//functions
void checkButtons(){
    if(screen == 0){
    if(mode == 0){ // travel mode
        if (Pokitto::Buttons::repeat(BTN_LEFT, buttonRepeatFrame))
            colPointer = minMax(colPointer-1, 0, maxColumn - 1);
    	if (Pokitto::Buttons::repeat(BTN_RIGHT, buttonRepeatFrame))
            colPointer = minMax(colPointer+1, 0, maxColumn - 1);
		if (Pokitto::Buttons::repeat(BTN_UP, buttonRepeatFrame)){
    		rowPointer--;
    		screenPointer--;
            rowPointer = minMax(rowPointer, 0, maxRow - 1);
            if ((screenPointer + screenMin) < screenMin){
                screenMin--;
                screenMax--;
                screenPointer++;
            }
            if(screenMin < 0){
                screenMin = maxRow - screenMaxInit; // maxRow - 13
                screenMax = maxRow - 1; // maxRow - 1
                screenPointer = screenMaxInit - 1;
            }
    	}
    	if (Pokitto::Buttons::repeat(BTN_DOWN, buttonRepeatFrame)){
    		rowPointer++;
    		screenPointer++;
            rowPointer = minMax(rowPointer, 0, maxRow - 1);
            if ((screenPointer + screenMin) > screenMax){
                screenMin++;
                screenMax++;
                screenPointer--;
            }
            if(screenMax > (maxRow - 1)){
                screenMin = 0;
                screenMax = screenMaxInit;
                screenPointer = 0;
            }
    	}
    	if (Pokitto::Buttons::pressed(BTN_A)){
            mode = 1;
    	}
    	if (Pokitto::Buttons::pressed(BTN_B)){
            _playNote();
    	}
    	if (Pokitto::Buttons::released(BTN_B)){
            _pause();
    	}
    	if (Pokitto::Buttons::pressed(BTN_C)){
            mode = 3;
    	}
    }
    else if (mode == 1){ // edit pitches and patches
        if (Pokitto::Buttons::pressed(BTN_B)){
            mode = 0;
            edit = 0;
        }
        if (Pokitto::Buttons::pressed(BTN_LEFT))
            edit = minMax(edit-1, 0, 1);
    	if (Pokitto::Buttons::pressed(BTN_RIGHT))
            edit = minMax(edit+1, 0, 1);
    	if (Pokitto::Buttons::repeat(BTN_UP, buttonRepeatFrame)){
            if (edit == 0)
                blocks[song.block_sequence[colPointer][sequencepos]].notenumber[rowPointer] = minMax(blocks[song.block_sequence[colPointer][sequencepos]].notenumber[rowPointer]+1, 0, 99);
            if (edit == 1)
                blocks[song.block_sequence[colPointer][sequencepos]].instrument[rowPointer] = minMax(blocks[song.block_sequence[colPointer][sequencepos]].instrument[rowPointer]+1, 0, 14);
        }
        if (Pokitto::Buttons::repeat(BTN_DOWN, buttonRepeatFrame)){
            if (edit == 0)
                blocks[song.block_sequence[colPointer][sequencepos]].notenumber[rowPointer] = minMax(blocks[song.block_sequence[colPointer][sequencepos]].notenumber[rowPointer]-1, 0, 99);
            if (edit == 1)
                blocks[song.block_sequence[colPointer][sequencepos]].instrument[rowPointer] = minMax(blocks[song.block_sequence[colPointer][sequencepos]].instrument[rowPointer]-1, 0, 14);
        }
    }
    else if(mode == 2){ // this screen settings
        if (Pokitto::Buttons::pressed(BTN_C)){
            screen = 1;
            mode = 0;
            edit = 0;
        }
        if (Pokitto::Buttons::pressed(BTN_UP))
            edit = minMax(edit-1, 0, 9);
    	if (Pokitto::Buttons::pressed(BTN_DOWN))
            edit = minMax(edit+1, 0, 9);
    	if (Pokitto::Buttons::repeat(BTN_LEFT, buttonRepeatFrame)){
            if (edit == 0){
                song.song_bpm = minMax(song.song_bpm-1, 1, 300);
                _tempo = (60000 / song.song_bpm) / 4;
            }
            if (edit == 1) sequencepos = minMax(sequencepos-1, 0, song.song_end);
            else if (edit == 2) song.song_end = minMax(song.song_end-1, 0, 9);
            else if (edit == 3) song.song_loop = minMax(song.song_loop-1, -1, 9);
            else if (edit == 4) song.block_sequence[0][sequencepos] = minMax(song.block_sequence[0][sequencepos]-1, 0, 29);
            else if (edit == 5) song.block_sequence[1][sequencepos] = minMax(song.block_sequence[1][sequencepos]-1, 0, 29);
            else if (edit == 6) song.block_sequence[2][sequencepos] = minMax(song.block_sequence[2][sequencepos]-1, 0, 29);
            else if (edit == 7) osc1.on = minMax(osc1.on-1, 0, 1);
            else if (edit == 8) osc2.on = minMax(osc2.on-1, 0, 1);
            else if (edit == 9) osc3.on = minMax(osc3.on-1, 0, 1);
        }
        if (Pokitto::Buttons::repeat(BTN_RIGHT, buttonRepeatFrame)){
            if (edit == 0){
                song.song_bpm = minMax(song.song_bpm+1, 1, 300);
                _tempo = (60000 / song.song_bpm) / 4;
            }
            if (edit == 1) sequencepos = minMax(sequencepos+1, 0, song.song_end);
            if (edit == 2) song.song_end = minMax(song.song_end+1, 0, 9);
            if (edit == 3) song.song_loop = minMax(song.song_loop+1, -1, 9);
            if (edit == 4) song.block_sequence[0][sequencepos] = minMax(song.block_sequence[0][sequencepos]+1, 0, 29);
            if (edit == 5) song.block_sequence[1][sequencepos] = minMax(song.block_sequence[1][sequencepos]+1, 0, 29);
            if (edit == 6) song.block_sequence[2][sequencepos] = minMax(song.block_sequence[2][sequencepos]+1, 0, 29);
            else if (edit == 7) osc1.on = minMax(osc1.on+1, 0, 1);
            else if (edit == 8) osc2.on = minMax(osc2.on+1, 0, 1);
            else if (edit == 9) osc3.on = minMax(osc3.on+1, 0, 1);
        }
    }
    else if(mode == 3){ // play, pause
        if (Pokitto::Buttons::pressed(BTN_A)){
            play = !play;
            if (play) {
                playTime = Pokitto::Core::getTime();
                rowPointer = 0;
                screenPointer = 0;
                screenMin = 0;
                screenMax = screenMaxInit;
            }
			else _pause();
    	}
    	if (Pokitto::Buttons::pressed(BTN_B)){
            play = 0;
            sequencepos = 0;
            rowPointer = 0;
            screenPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
			_pause();
    	}
    	if (Pokitto::Buttons::pressed(BTN_C)){
            play = 0;
            mode = 2;
			_pause();
    	}
    }
    }
    else if(screen == 1){
    if(Pokitto::Buttons::repeat(BTN_DOWN, buttonRepeatFrame))
        settingPointer = minMax(settingPointer+1, 0, 19);
    else if(Pokitto::Buttons::repeat(BTN_UP, buttonRepeatFrame))
        settingPointer = minMax(settingPointer-1, 0, 19);
    else if(Pokitto::Buttons::aBtn() && Pokitto::Buttons::repeat(BTN_LEFT, buttonRepeatFrame))
        pchangeValues(-10);
    else if(Pokitto::Buttons::aBtn() && Pokitto::Buttons::repeat(BTN_RIGHT, buttonRepeatFrame))
        pchangeValues(10);
    else if(Pokitto::Buttons::bBtn() && Pokitto::Buttons::repeat(BTN_LEFT, buttonRepeatFrame))
        pchangeValues(-100);
    else if(Pokitto::Buttons::bBtn() && Pokitto::Buttons::repeat(BTN_RIGHT, buttonRepeatFrame))
        pchangeValues(100);
    else if(Pokitto::Buttons::repeat(BTN_LEFT, buttonRepeatFrame))
    {
        if (settingPointer == 0) instrPointer = minMax(instrPointer-1, 0, 14);
        else if (settingPointer == 4) patch[instrPointer].loop = minMax(patch[instrPointer].loop-1, 0, 1);
        else if (settingPointer == 5) patch[instrPointer].echo = minMax(patch[instrPointer].echo-1, 0, 1);
        else if (settingPointer == 6) patch[instrPointer].overdrive = minMax(patch[instrPointer].overdrive-1, 0, 1);
        else if (settingPointer == 7) patch[instrPointer].kick = minMax(patch[instrPointer].kick-1, 0, 1);
        else if (settingPointer == 8) patch[instrPointer].adsr = minMax(patch[instrPointer].adsr-1, 0, 1);
        pchangeValues(-1);
    }
    else if(Pokitto::Buttons::repeat(BTN_RIGHT, buttonRepeatFrame))
    {
        if (settingPointer == 0) instrPointer = minMax(instrPointer+1, 0, 14);
        else if (settingPointer == 4) patch[instrPointer].loop = minMax(patch[instrPointer].loop+1, 0, 1);
        else if (settingPointer == 5) patch[instrPointer].echo = minMax(patch[instrPointer].echo+1, 0, 1);
        else if (settingPointer == 6) patch[instrPointer].overdrive = minMax(patch[instrPointer].overdrive+1, 0, 1);
        else if (settingPointer == 7) patch[instrPointer].kick = minMax(patch[instrPointer].kick+1, 0, 1);
        else if (settingPointer == 8) patch[instrPointer].adsr = minMax(patch[instrPointer].adsr+1, 0, 1);
        pchangeValues(1);
    }
    else if(Pokitto::Buttons::pressed(BTN_A))
    {
        if(settingPointer == 1)
            Pokitto::Core::keyboard(patchname[instrPointer], 10);
        else if(settingPointer == 2){
            uint8_t old = patch[instrPointer].wave;
            patch[instrPointer].wave = Pokitto::Core::menu(waveNames, 6);
            if (patch[instrPointer].wave == -1) patch[instrPointer].wave = old;
        }
        else if(settingPointer == 3){
            uint8_t old = patch[instrPointer].arpmode;
            patch[instrPointer].arpmode = Pokitto::Core::menu(arpModes, 16);
            if (patch[instrPointer].arpmode == -1) patch[instrPointer].arpmode = old;
        }
        //else if(settingPointer == 16)
            //save instrument
        //else if(settingPointer == 17)
            //load instrument
        //else if(settingPointer == 18)
            //save song
        //else if(settingPointer == 19)
            //load song
    }
    else if(Pokitto::Buttons::pressed(BTN_C)){
        screen = 0;
        settingPointer = 0;
    }
    }
}

void draw(){
    if (screen){
        pprintSettings();
        pdrawPointer();
    }
    else{
        tdrawIsPlaying();
        tdrawLines();
        tprintValues();
        tprintSettings();
        tdrawPointer();
    }
}

void tfillArrays(){
    for(uint8_t i = 0; i < 30; i++){
        for(uint8_t j = 0; j < 64; j++){
            blocks[i].instrument[j] = 255;
            blocks[i].notenumber[j] = 255;
        }
    }
    for(uint8_t i = 0; i < 3; i++){
        for(uint8_t j = 0; j < 10; j++){
            song.block_sequence[i][j] = 0;
        }
    }
}

void tdrawLines(){ // draw lines in tracker mode
    Pokitto::Display::setColor(3);
    Pokitto::Display::drawFastVLine(vLines[0], 0, screenH);
    Pokitto::Display::drawFastVLine(vLines[1], 0, screenH);
    Pokitto::Display::drawFastVLine(vLines[2], 0, screenH);
    Pokitto::Display::drawFastVLine(vLines[3], 0, screenH);
    Pokitto::Display::drawFastHLine(0, fontH + 1, vLines[3]);
    Pokitto::Display::setColor(1);
    Pokitto::Display::setCursor(vLines[0] + 1, 0);
    Pokitto::Display::print("  Track 1");
    Pokitto::Display::setCursor(vLines[1] + 1, 0);
    Pokitto::Display::print("  Track 2");
    Pokitto::Display::setCursor(vLines[2] + 1, 0);
    Pokitto::Display::print("  Track 3");
}

void tdrawPointer(){
    Pokitto::Display::setColor(2);
    if (mode == 0){
        Pokitto::Display::drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (9 * fontW) + 10, fontH + 1);
    }
    if (mode == 1){
        if (edit == 0){
            Pokitto::Display::drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (3 * fontW) + 5, fontH + 1);
        }
        if (edit == 1){
            Pokitto::Display::drawRect(vLines[colPointer] + ((fontH * 3) + 1), (fontH + 1) * (screenPointer + 1), (2 * fontW) + 5, fontH + 1);
        }
    }
    if (mode == 2){
        Pokitto::Display::drawRect(vLines[3] + 1, (fontH + 1) * ((edit * 2) + 1), (6 * fontW) + 7, fontH + 1);
    }
    if (mode == 3){
        Pokitto::Display::drawRect(vLines[0], (fontH + 1) * (screenPointer + 1), vLines[3] - vLines[0], fontH + 1);
    }
}

void tdrawIsPlaying(){
    if (play){
        Pokitto::Display::drawBitmap(2, 1, playBitmap);
    }
    else {
        Pokitto::Display::drawBitmap(2, 1, pauseBitmap);
    }
}

void tprintValues(){
    uint8_t counter = 0;
    for (uint8_t i = screenMin; i <= screenMax; i++){
        if ((i % 4) == 0) Pokitto::Display::setColor(3);
        else Pokitto::Display::setColor(1);
        Pokitto::Display::setCursor(0, ((fontH + 1) * (counter + 1)) + 1);
        Pokitto::Display::print(screenMin + counter);
        Pokitto::Display::setCursor(vLines[0] + 2, ((fontH + 1) * (counter + 1)) + 1); // first col
        if (blocks[song.block_sequence[0][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("---");//pitch
        else{
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[0][sequencepos]].notenumber[i] % 12][0]);
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[0][sequencepos]].notenumber[i] % 12][1]);
            Pokitto::Display::print(blocks[song.block_sequence[0][sequencepos]].notenumber[i] / 12);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[0][sequencepos]].instrument[i] == 255) Pokitto::Display::print("--");//patch
        else{
            if (blocks[song.block_sequence[0][sequencepos]].instrument[i] < 10) Pokitto::Display::print("0");
            Pokitto::Display::print(blocks[song.block_sequence[0][sequencepos]].instrument[i], 10);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[0][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("--");//ref number
        else Pokitto::Display::print(blocks[song.block_sequence[0][sequencepos]].notenumber[i], 10);
        Pokitto::Display::setCursor(vLines[1] + 2, ((fontH + 1) * (counter + 1)) + 1); // second col
        if (blocks[song.block_sequence[1][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("---");//pitch
        else{
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[1][sequencepos]].notenumber[i] % 12][0]);
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[1][sequencepos]].notenumber[i] % 12][1]);
            Pokitto::Display::print(blocks[song.block_sequence[1][sequencepos]].notenumber[i] / 12);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[1][sequencepos]].instrument[i] == 255) Pokitto::Display::print("--");//patch
        else{
            if (blocks[song.block_sequence[1][sequencepos]].instrument[i] < 10) Pokitto::Display::print("0");
            Pokitto::Display::print(blocks[song.block_sequence[1][sequencepos]].instrument[i], 10);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[1][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("--");//ref number
        else Pokitto::Display::print(blocks[song.block_sequence[1][sequencepos]].notenumber[i], 10);
        Pokitto::Display::setCursor(vLines[2] + 2, ((fontH + 1) * (counter + 1)) + 1); // third col
        if (blocks[song.block_sequence[2][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("---");//pitch
        else{
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[2][sequencepos]].notenumber[i] % 12][0]);
            Pokitto::Display::print(noteLetters[blocks[song.block_sequence[2][sequencepos]].notenumber[i] % 12][1]);
            Pokitto::Display::print(blocks[song.block_sequence[2][sequencepos]].notenumber[i] / 12);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[2][sequencepos]].instrument[i] == 255) Pokitto::Display::print("--");//patch
        else{
            if (blocks[song.block_sequence[2][sequencepos]].instrument[i] < 10) Pokitto::Display::print("0");
            Pokitto::Display::print(blocks[song.block_sequence[2][sequencepos]].instrument[i], 10);
        }
        Pokitto::Display::print(" ");
        if (blocks[song.block_sequence[2][sequencepos]].notenumber[i] == 255) Pokitto::Display::print("--");//ref number
        else Pokitto::Display::print(blocks[song.block_sequence[2][sequencepos]].notenumber[i], 10);
        counter++;
    }
}

void tprintSettings(){
    Pokitto::Display::setColor(1);
    Pokitto::Display::setCursor(s_vLines[0], 1);
    Pokitto::Display::print("Tempo");
    Pokitto::Display::setCursor(s_vLines[0], fontH + 2);
    Pokitto::Display::print(song.song_bpm, 10);
    Pokitto::Display::setCursor(s_vLines[0], (2 * fontH) + 3);
    Pokitto::Display::print("SngPos");
    Pokitto::Display::setCursor(s_vLines[0], (3 * fontH) + 4);
    Pokitto::Display::print(sequencepos, 10);
    Pokitto::Display::setCursor(s_vLines[0], (4 * fontH) + 5);
    Pokitto::Display::print("SngEnd");
    Pokitto::Display::setCursor(s_vLines[0], (5 * fontH) + 6);
    Pokitto::Display::print(song.song_end, 10);
    Pokitto::Display::setCursor(s_vLines[0], (6 * fontH) + 7);
    Pokitto::Display::print("LoopTo");
    Pokitto::Display::setCursor(s_vLines[0], (7 * fontH) + 8);
    Pokitto::Display::print(song.song_loop, 10);
    Pokitto::Display::setCursor(s_vLines[0], (8 * fontH) + 9);
    Pokitto::Display::print("T1Blck");
    Pokitto::Display::setCursor(s_vLines[0], (9 * fontH) + 10);
    Pokitto::Display::print(song.block_sequence[0][sequencepos], 10);
    Pokitto::Display::setCursor(s_vLines[0], (10 * fontH) + 11);
    Pokitto::Display::print("T2Blck");
    Pokitto::Display::setCursor(s_vLines[0], (11 * fontH) + 12);
    Pokitto::Display::print(song.block_sequence[1][sequencepos], 10);
    Pokitto::Display::setCursor(s_vLines[0], (12 * fontH) + 13);
    Pokitto::Display::print("T3Blck");
    Pokitto::Display::setCursor(s_vLines[0], (13 * fontH) + 14);
    Pokitto::Display::print(song.block_sequence[2][sequencepos], 10);
    Pokitto::Display::setCursor(s_vLines[0], (14 * fontH) + 15);
    Pokitto::Display::print("Trk1On");
    Pokitto::Display::setCursor(s_vLines[0], (15 * fontH) + 16);
    Pokitto::Display::print(osc1.on, 10);
    Pokitto::Display::setCursor(s_vLines[0], (16 * fontH) + 17);
    Pokitto::Display::print("Trk2On");
    Pokitto::Display::setCursor(s_vLines[0], (17 * fontH) + 18);
    Pokitto::Display::print(osc2.on, 10);
    Pokitto::Display::setCursor(s_vLines[0], (18 * fontH) + 19);
    Pokitto::Display::print("Trk3On");
    Pokitto::Display::setCursor(s_vLines[0], (19 * fontH) + 20);
    Pokitto::Display::print(osc3.on, 10);
}

int minMax(int _val, int _min, int _max){
    if (_val < _min) return _max;
    else if (_val > _max) return _min;
    else return _val;
}

void pchangeValues(int8_t i){
    if (settingPointer == 9) patch[instrPointer].attack = minMax(patch[instrPointer].attack+i, 0, 255);
    else if (settingPointer == 10) patch[instrPointer].decay = minMax(patch[instrPointer].decay+i, 0, 255);
    else if (settingPointer == 11) patch[instrPointer].sustain = minMax(patch[instrPointer].sustain+i, 0, 255);
    else if (settingPointer == 12) patch[instrPointer].release = minMax(patch[instrPointer].release+i, 0, 255);
    else if (settingPointer == 13) patch[instrPointer].bendrate = minMax(patch[instrPointer].bendrate+i, -1000, 1000);
    else if (settingPointer == 14) patch[instrPointer].maxbend = minMax(patch[instrPointer].maxbend+i, -6000, 6000);
    else if (settingPointer == 15) patch[instrPointer].vol = minMax(patch[instrPointer].vol+i, 0, 300);
}

void pprintSettings(){
    Pokitto::Display::setColor(1);
    Pokitto::Display::setCursor(2, 0);
    Pokitto::Display::print("Patch: ");
    Pokitto::Display::print(instrPointer, 10);
    Pokitto::Display::setCursor(2, fontH + 1);
    Pokitto::Display::print("Name: ");
    Pokitto::Display::print(patchname[instrPointer]);
    Pokitto::Display::setCursor(2, (2 * fontH) + 2);
    Pokitto::Display::print("Wave: ");
    Pokitto::Display::print(waveNames[patch[instrPointer].wave]);
    Pokitto::Display::setCursor(2, (3 * fontH) + 3);
    Pokitto::Display::print("Arp Mode: ");
    Pokitto::Display::print(arpModes[patch[instrPointer].arpmode]);
    Pokitto::Display::setCursor(2, (4 * fontH) + 4);
    Pokitto::Display::print("Loop: ");
    Pokitto::Display::print(patch[instrPointer].loop, 10);
    Pokitto::Display::setCursor(2, (5 * fontH) + 5);
    Pokitto::Display::print("Echo: ");
    Pokitto::Display::print(patch[instrPointer].echo, 10);
    Pokitto::Display::setCursor(2, (6 * fontH) + 6);
    Pokitto::Display::print("Dist: ");
    Pokitto::Display::print(patch[instrPointer].overdrive, 10);
    Pokitto::Display::setCursor(2, (7 * fontH) + 7);
    Pokitto::Display::print("Norm: ");
    Pokitto::Display::print(patch[instrPointer].kick, 10);
    Pokitto::Display::setCursor(2, (8 * fontH) + 8);
    Pokitto::Display::print("ADSR: ");
    Pokitto::Display::print(patch[instrPointer].adsr, 10);
    Pokitto::Display::setCursor(2, (9 * fontH) + 9);
    Pokitto::Display::print("Attack: ");
    Pokitto::Display::print(patch[instrPointer].attack, 10);
    Pokitto::Display::setCursor(2, (10 * fontH) + 10);
    Pokitto::Display::print("Decay: ");
    Pokitto::Display::print(patch[instrPointer].decay, 10);
    Pokitto::Display::setCursor(2, (11 * fontH) + 11);
    Pokitto::Display::print("Sustain: ");
    Pokitto::Display::print(patch[instrPointer].sustain, 10);
    Pokitto::Display::setCursor(2, (12 * fontH) + 12);
    Pokitto::Display::print("Release: ");
    Pokitto::Display::print(patch[instrPointer].release, 10);
    Pokitto::Display::setCursor(2, (13 * fontH) + 13);
    Pokitto::Display::print("Pitch Bend Rate: ");
    Pokitto::Display::print(patch[instrPointer].bendrate, 10);
    Pokitto::Display::setCursor(2, (14 * fontH) + 14);
    Pokitto::Display::print("Pitch Bend Max: ");
    Pokitto::Display::print(patch[instrPointer].maxbend, 10);
    Pokitto::Display::setCursor(2, (15 * fontH) + 15);
    Pokitto::Display::print("Volume: ");
    Pokitto::Display::print(patch[instrPointer].vol, 10);
    Pokitto::Display::setCursor(2, (16 * fontH) + 16);
    Pokitto::Display::print("Save Instrument");
    Pokitto::Display::setCursor(2, (17 * fontH) + 17);
    Pokitto::Display::print("Load Instrument");
    Pokitto::Display::setCursor(2, (18 * fontH) + 18);
    Pokitto::Display::print("Save Song");
    Pokitto::Display::setCursor(2, (19 * fontH) + 19);
    Pokitto::Display::print("Load Song");
}

void pdrawPointer(){
    Pokitto::Display::drawBitmap(130, (settingPointer * fontH) + settingPointer, pointBitmap);
}

void initTracker(){
    Pokitto::Display::persistence = false;
	Pokitto::Display::width = screenW;
	Pokitto::Display::height = screenH;
	Pokitto::Display::setFont(font5x7);
	Pokitto::Core::setFrameRate(frameRate);
	tfillArrays(); // there are 30 blocks and all block have 64 pitch and patch storage. Fill them -1
    song.song_bpm = 60;
    song.num_patches = 15;
    song.num_patterns = 10;
    song.num_channels = 3;
    sequencepos = 0;
    song.song_loop = -1;
    song.song_end = 0;
    osc1.on = 1;
    osc2.on = 1;
    osc3.on = 1;
};

void _playNote(){
	if(play){
		uint8_t _instr1 = blocks[song.block_sequence[0][sequencepos]].instrument[rowPointer];
		uint8_t _notenum1 = blocks[song.block_sequence[0][sequencepos]].notenumber[rowPointer];
		uint8_t _instr2 = blocks[song.block_sequence[1][sequencepos]].instrument[rowPointer];
		uint8_t _notenum2 = blocks[song.block_sequence[1][sequencepos]].notenumber[rowPointer];
		uint8_t _instr3 = blocks[song.block_sequence[2][sequencepos]].instrument[rowPointer];
		uint8_t _notenum3 = blocks[song.block_sequence[2][sequencepos]].notenumber[rowPointer];
		setOSC(&osc1, osc1.on, patch[_instr1].wave, patch[_instr1].loop, patch[_instr1].echo, patch[_instr1].adsr, _notenum1, \
				patch[_instr1].vol, patch[_instr1].attack, patch[_instr1].decay, patch[_instr1].sustain, patch[_instr1].release, \
				patch[_instr1].maxbend, patch[_instr1].bendrate, patch[_instr1].arpmode, patch[_instr1].overdrive, patch[_instr1].kick);
		setOSC(&osc2, osc2.on, patch[_instr2].wave, patch[_instr2].loop, patch[_instr2].echo, patch[_instr2].adsr, _notenum2, \
				patch[_instr2].vol, patch[_instr2].attack, patch[_instr2].decay, patch[_instr2].sustain, patch[_instr2].release, \
				patch[_instr2].maxbend, patch[_instr2].bendrate, patch[_instr2].arpmode, patch[_instr2].overdrive, patch[_instr2].kick);
		setOSC(&osc3, osc3.on, patch[_instr3].wave, patch[_instr3].loop, patch[_instr3].echo, patch[_instr3].adsr, _notenum3, \
				patch[_instr3].vol, patch[_instr3].attack, patch[_instr3].decay, patch[_instr3].sustain, patch[_instr3].release, \
				patch[_instr3].maxbend, patch[_instr3].bendrate, patch[_instr3].arpmode, patch[_instr3].overdrive, patch[_instr3].kick);
	}
	else{
		if (screen){
			setOSC(&osc1, 1, patch[instrPointer].wave, patch[instrPointer].loop, patch[instrPointer].echo, patch[instrPointer].adsr, 69, \
					patch[instrPointer].vol, patch[instrPointer].attack, patch[instrPointer].decay, patch[instrPointer].sustain, patch[instrPointer].release, \
					patch[instrPointer].maxbend, patch[instrPointer].bendrate, patch[instrPointer].arpmode, patch[instrPointer].overdrive, patch[instrPointer].kick);
		}
		else{
			uint8_t _instr = blocks[song.block_sequence[colPointer][sequencepos]].instrument[rowPointer];
			uint8_t _notenum = blocks[song.block_sequence[colPointer][sequencepos]].notenumber[rowPointer];
			switch(colPointer){
			case 0:
				setOSC(&osc1, 1, patch[_instr].wave, patch[_instr].loop, patch[_instr].echo, patch[_instr].adsr, _notenum, \
					   patch[_instr].vol, patch[_instr].attack, patch[_instr].decay, patch[_instr].sustain, patch[_instr].release, \
					   patch[_instr].maxbend, patch[_instr].bendrate, patch[_instr].arpmode, patch[_instr].overdrive, patch[_instr].kick);

			case 1:
				setOSC(&osc2, 1, patch[_instr].wave, patch[_instr].loop, patch[_instr].echo, patch[_instr].adsr, _notenum, \
					   patch[_instr].vol, patch[_instr].attack, patch[_instr].decay, patch[_instr].sustain, patch[_instr].release, \
					   patch[_instr].maxbend, patch[_instr].bendrate, patch[_instr].arpmode, patch[_instr].overdrive, patch[_instr].kick);

			case 2:
				setOSC(&osc3, 1, patch[_instr].wave, patch[_instr].loop, patch[_instr].echo, patch[_instr].adsr, _notenum, \
					   patch[_instr].vol, patch[_instr].attack, patch[_instr].decay, patch[_instr].sustain, patch[_instr].release, \
					   patch[_instr].maxbend, patch[_instr].bendrate, patch[_instr].arpmode, patch[_instr].overdrive, patch[_instr].kick);

			}
		}
	}
}

void _playPattern(){
	if(play){
		//
		uint8_t a = screenMaxInit / 2;
        uint8_t b = ((maxRow - 1) - (screenMaxInit / 2)) - 1;
        if (rowPointer < a){
            rowPointer++;
            screenPointer++;
        }
        else if ((rowPointer >= a) && (rowPointer <= b)){
            rowPointer++;
            screenMin++;
            screenMax++;
        }
        else if (rowPointer > b){
            rowPointer++;
            screenPointer++;
        }
		if (rowPointer == maxRow){
            sequencepos++;
            rowPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
            screenPointer = 0;
        }
        if (sequencepos == (song.song_end + 1)){
            if (song.song_loop == -1) {
                play = 0;
                sequencepos -= 1;
            }
            else sequencepos = song.song_loop;
        }
	}
}

void _playSong(){
	if(play){
		//
		_playPattern();
		_playNote();
	}
}

void _pause(){
    bool o1 = osc1.on;
    bool o2 = osc2.on;
    bool o3 = osc3.on;
    emptyOscillators();
    osc1.on = o1;
    osc2.on = o2;
    osc3.on = o3;
}

#endif
