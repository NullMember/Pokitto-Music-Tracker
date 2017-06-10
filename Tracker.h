#include "Pokitto.h"

struct Tracker{
    private:
        uint8_t maxColumn = 3, maxRow = 64;
        int8_t colPointer = 0, rowPointer = 0;
        int8_t screenMaxInit = 20, screenMin = 0, screenMax = screenMaxInit, screenPointer = 0, playPointer = 0;
        int16_t bpm = 60, _tempo = 250;
        uint32_t playTime = 0;
        int8_t songPos = 0, lastPattern = 0, loopTo = 0;
        uint8_t mode = 0; //mode0 = travel mode, mode1 = edit pitches and patches, mode2 = screen1 settings, mode3 = play, pause, stop
        int8_t edit = 0;
        uint8_t vLines[4] = {((fontW + 1) * 2) + 1, (((fontW + 1) * 9) + vLines[0]) + 1, (((fontW + 1) * 9) + vLines[1]) + 1, (((fontW + 1) * 9) + vLines[2]) + 1}; //(((fontW + 1) * number of chars) + left line) + 1
        uint8_t s_vLines[2] = {vLines[3] + 2, 0};
        int8_t _patch[30][64];
        int8_t _pitch[30][64];
        int8_t _songPos[10][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        bool t1Mute = 0, t2Mute = 0, t3Mute = 0;
    public:
        bool play = 0, stop = 0;
        void checkButtons();
        void fillArrays();
        void drawLines();
        void drawPointer();
        void drawIsPlaying();
        void printValues();
        void printSettings();
        void playTracker();
        int16_t getBPM();
        int minMax(int _val, int _min, int _max);
};

int16_t Tracker::getBPM(){
    return bpm;
}

void Tracker::checkButtons(){
    if(Tracker::mode == 0){ // travel mode
        if (pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame))
            colPointer = minMax(colPointer-1, 0, maxColumn - 1);
    	if (pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame))
            colPointer = minMax(colPointer+1, 0, maxColumn - 1);
		if (pok.buttons.repeat(BTN_UP, buttonRepeatFrame)){
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
    	if (pok.buttons.repeat(BTN_DOWN, buttonRepeatFrame)){
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
    	if (pok.buttons.pressed(BTN_A)){
            mode = 1;
    	}
    	if (pok.buttons.pressed(BTN_C)){
            mode = 3;
    	}
    }
    else if (mode == 1){ // edit pitches and patches
        if (pok.buttons.pressed(BTN_B)){
            mode = 0;
            edit = 0;
        }
        if (pok.buttons.pressed(BTN_LEFT))
            edit = minMax(edit-1, 0, 1);
    	if (pok.buttons.pressed(BTN_RIGHT))
            edit = minMax(edit+1, 0, 1);
    	if (pok.buttons.repeat(BTN_UP, buttonRepeatFrame)){
            if (edit == 0)
                _pitch[_songPos[songPos][colPointer]][rowPointer] = minMax(_pitch[_songPos[songPos][colPointer]][rowPointer]+1, 0, 99);
            if (edit == 1)
                _patch[_songPos[songPos][colPointer]][rowPointer] = minMax(_patch[_songPos[songPos][colPointer]][rowPointer]+1, 0, 14);
        }
        if (pok.buttons.repeat(BTN_DOWN, buttonRepeatFrame)){
            if (edit == 0)
                _pitch[_songPos[songPos][colPointer]][rowPointer] = minMax(_pitch[_songPos[songPos][colPointer]][rowPointer]-1, 0, 99);
            if (edit == 1)
                _patch[_songPos[songPos][colPointer]][rowPointer] = minMax(_patch[_songPos[songPos][colPointer]][rowPointer]-1, 0, 14);
        }
    }
    else if(mode == 2){ // this screen settings
        if (pok.buttons.pressed(BTN_C)){
            screen = 1;
            mode = 0;
            edit = 0;
        }
        if (pok.buttons.pressed(BTN_UP))
            edit = minMax(edit-1, 0, 9);
    	if (pok.buttons.pressed(BTN_DOWN))
            edit = minMax(edit+1, 0, 9);
    	if (pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame)){
            if (edit == 0){
                bpm = minMax(bpm-1, 1, 300);
                _tempo = (60000 / bpm) / 4;
            }
            if (edit == 1) songPos = minMax(songPos-1, 0, 9);
            else if (edit == 2) lastPattern = minMax(lastPattern-1, 0, 9);
            else if (edit == 3) loopTo = minMax(loopTo-1, 0, 9);
            else if (edit == 4) _songPos[songPos][0] = minMax(_songPos[songPos][0]-1, 0, 29);
            else if (edit == 5) _songPos[songPos][1] = minMax(_songPos[songPos][1]-1, 0, 29);
            else if (edit == 6) _songPos[songPos][2] = minMax(_songPos[songPos][2]-1, 0, 29);
            else if (edit == 7) t1Mute = minMax(t1Mute-1, 0, 1);
            else if (edit == 8) t2Mute = minMax(t2Mute-1, 0, 1);
            else if (edit == 9) t3Mute = minMax(t3Mute-1, 0, 1);
        }
        if (pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame)){
            if (edit == 0){
                bpm = minMax(bpm+1, 1, 300);
                _tempo = (60000 / bpm) / 4;
            }
            if (edit == 1) songPos = minMax(songPos+1, 0, 9);
            if (edit == 2) lastPattern = minMax(lastPattern+1, 0, 9);
            if (edit == 3) loopTo = minMax(loopTo+1, 0, 9);
            if (edit == 4) _songPos[songPos][0] = minMax(_songPos[songPos][0]+1, 0, 29);
            if (edit == 5) _songPos[songPos][1] = minMax(_songPos[songPos][1]+1, 0, 29);
            if (edit == 6) _songPos[songPos][2] = minMax(_songPos[songPos][2]+1, 0, 29);
            else if (edit == 7) t1Mute = minMax(t1Mute+1, 0, 1);
            else if (edit == 8) t2Mute = minMax(t2Mute+1, 0, 1);
            else if (edit == 9) t3Mute = minMax(t3Mute+1, 0, 1);
        }
    }
    else if(mode == 3){ // play, pause, stop
        if (pok.buttons.pressed(BTN_A)){
            stop = 0;
            play = !play;
            if (play) {
                playTime = pok.getTime();
                rowPointer = 0;
                screenPointer = 0;
                screenMin = 0;
                screenMax = screenMaxInit;
            }
    	}
    	if (pok.buttons.pressed(BTN_B)){
            stop = 1;
            play = 0;
            songPos = 0;
            rowPointer = 0;
            screenPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
    	}
    	if (pok.buttons.pressed(BTN_C)){
            play = 0;
            mode = 2;
    	}
    }
}

void Tracker::fillArrays(){
    for(uint8_t i = 0; i < 30; i++){
        for(uint8_t j = 0; j < 64; j++){
            _patch[i][j] = -1;
            _pitch[i][j] = -1;
        }
    }
}

void Tracker::drawLines(){ // draw lines in tracker mode
    pok.display.setColor(3);
    pok.display.drawFastVLine(vLines[0], 0, screenH);
    pok.display.drawFastVLine(vLines[1], 0, screenH);
    pok.display.drawFastVLine(vLines[2], 0, screenH);
    pok.display.drawFastVLine(vLines[3], 0, screenH);
    pok.display.drawFastHLine(0, fontH + 1, vLines[3]);
    pok.display.setColor(1);
    pok.display.setCursor(vLines[0] + 1, 0);
    pok.display.print("  Track 1");
    pok.display.setCursor(vLines[1] + 1, 0);
    pok.display.print("  Track 2");
    pok.display.setCursor(vLines[2] + 1, 0);
    pok.display.print("  Track 3");
}

void Tracker::drawPointer(){
    pok.display.setColor(2);
    if (mode == 0){
        pok.display.drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (9 * fontW) + 10, fontH + 1);
    }
    if (mode == 1){
        if (edit == 0){
            pok.display.drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (3 * fontW) + 5, fontH + 1);
        }
        if (edit == 1){
            pok.display.drawRect(vLines[colPointer] + ((fontH * 3) + 1), (fontH + 1) * (screenPointer + 1), (2 * fontW) + 5, fontH + 1);
        }
    }
    if (mode == 2){
        pok.display.drawRect(vLines[3] + 1, (fontH + 1) * ((edit * 2) + 1), (6 * fontW) + 7, fontH + 1);
    }
    if (mode == 3){
        pok.display.drawRect(vLines[0], (fontH + 1) * (screenPointer + 1), vLines[3] - vLines[0], fontH + 1);
    }
}

void Tracker::drawIsPlaying(){
    if (stop){
        pok.display.drawBitmap(2, 1, stopBitmap);
    }
    else{
        if (play){
            pok.display.drawBitmap(2, 1, playBitmap);
        }
        else {
            pok.display.drawBitmap(2, 1, pauseBitmap);
        }
    }
}

void Tracker::printValues(){
    uint8_t counter = 0;
    for (uint8_t i = screenMin; i <= screenMax; i++){
        if ((i % 4) == 0) pok.display.setColor(3);
        else pok.display.setColor(1);
        pok.display.setCursor(0, ((fontH + 1) * (counter + 1)) + 1);
        pok.display.print(screenMin + counter);
        pok.display.setCursor(vLines[0] + 2, ((fontH + 1) * (counter + 1)) + 1); // first col
        if (_pitch[_songPos[songPos][0]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][0]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][0]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][0]][i] / 12);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][0]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][0]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][0]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][0]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][0]][i]);
        pok.display.setCursor(vLines[1] + 2, ((fontH + 1) * (counter + 1)) + 1); // second col
        if (_pitch[_songPos[songPos][1]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][1]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][1]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][1]][i] / 12);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][1]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][1]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][1]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][1]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][1]][i]);
        pok.display.setCursor(vLines[2] + 2, ((fontH + 1) * (counter + 1)) + 1); // third col
        if (_pitch[_songPos[songPos][2]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][2]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][2]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][2]][i] / 12);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][2]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][2]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][2]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][2]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][2]][i]);
        counter++;
    }
}

void Tracker::printSettings(){
    pok.display.setColor(1);
    pok.display.setCursor(s_vLines[0], 1);
    pok.display.print("Tempo");
    pok.display.setCursor(s_vLines[0], fontH + 2);
    pok.display.print(bpm, 10);
    pok.display.setCursor(s_vLines[0], (2 * fontH) + 3);
    pok.display.print("SngPos");
    pok.display.setCursor(s_vLines[0], (3 * fontH) + 4);
    pok.display.print(songPos, 10);
    pok.display.setCursor(s_vLines[0], (4 * fontH) + 5);
    pok.display.print("Pattrn");
    pok.display.setCursor(s_vLines[0], (5 * fontH) + 6);
    pok.display.print(lastPattern, 10);
    pok.display.setCursor(s_vLines[0], (6 * fontH) + 7);
    pok.display.print("LoopTo");
    pok.display.setCursor(s_vLines[0], (7 * fontH) + 8);
    pok.display.print(loopTo, 10);
    pok.display.setCursor(s_vLines[0], (8 * fontH) + 9);
    pok.display.print("T1Blck");
    pok.display.setCursor(s_vLines[0], (9 * fontH) + 10);
    pok.display.print(_songPos[songPos][0], 10);
    pok.display.setCursor(s_vLines[0], (10 * fontH) + 11);
    pok.display.print("T2Blck");
    pok.display.setCursor(s_vLines[0], (11 * fontH) + 12);
    pok.display.print(_songPos[songPos][1], 10);
    pok.display.setCursor(s_vLines[0], (12 * fontH) + 13);
    pok.display.print("T3Blck");
    pok.display.setCursor(s_vLines[0], (13 * fontH) + 14);
    pok.display.print(_songPos[songPos][2], 10);
    pok.display.setCursor(s_vLines[0], (14 * fontH) + 15);
    pok.display.print("MuteT1");
    pok.display.setCursor(s_vLines[0], (15 * fontH) + 16);
    pok.display.print(t1Mute, 10);
    pok.display.setCursor(s_vLines[0], (16 * fontH) + 17);
    pok.display.print("MuteT2");
    pok.display.setCursor(s_vLines[0], (17 * fontH) + 18);
    pok.display.print(t2Mute, 10);
    pok.display.setCursor(s_vLines[0], (18 * fontH) + 19);
    pok.display.print("MuteT3");
    pok.display.setCursor(s_vLines[0], (19 * fontH) + 20);
    pok.display.print(t3Mute, 10);
}

void Tracker::playTracker(){
    if ((pok.getTime() - playTime) >= _tempo){
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
            songPos++;
            rowPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
            screenPointer = 0;
        }
        if (songPos == (lastPattern + 1)){
            songPos = loopTo;
        }
        playTime = pok.getTime();
    }
}

int Tracker::minMax(int _val, int _min, int _max){
    if (_val < _min) return _max;
    else if (_val > _max) return _min;
    else return _val;
}
