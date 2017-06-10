struct Instrument{
    char name[10];
    int8_t wave;
    int8_t arpMode;
    int8_t loop;
    int8_t echo;
    int8_t distort;
    int8_t normalize;
    int8_t ADSR;
    uint8_t attack; //0-255
    uint8_t decay; //0-255
    uint8_t sustain; //0-255
    uint8_t release; //0-255
    int16_t volume; //0-300
    int16_t pitchRate; //-1000, 1000
    int16_t pitchMax; //-6000, 6000
    Instrument() : wave(0), arpMode(0), loop(0), echo(0), distort(0), normalize(0), ADSR(0),
    attack(0), decay(0), sustain(0), release(0), volume(0), pitchRate(0), pitchMax(0) {}
};
struct Instrument instr[15];
int8_t instrPointer = 0;
int8_t settingPointer = 0;

void saveSong();
char intToChar(int _int, uint8_t digit);
uint8_t digitLength(int _int);

void changeValues(int8_t i){
    if (settingPointer == 9) instr[instrPointer].attack += i;
    else if (settingPointer == 10) instr[instrPointer].decay += i;
    else if (settingPointer == 11) instr[instrPointer].sustain += i;
    else if (settingPointer == 12) instr[instrPointer].release += i;
    else if (settingPointer == 13) instr[instrPointer].pitchRate = tracker.minMax(instr[instrPointer].pitchRate+i, -1000, 1000);
    else if (settingPointer == 14) instr[instrPointer].pitchMax = tracker.minMax(instr[instrPointer].pitchMax+i, -6000, 6000);
    else if (settingPointer == 15) instr[instrPointer].volume = tracker.minMax(instr[instrPointer].volume+i, 0, 300);
}

void checkButtons(){
    if(pok.buttons.repeat(BTN_DOWN, buttonRepeatFrame))
        settingPointer = tracker.minMax(settingPointer+1, 0, 19);
    else if(pok.buttons.repeat(BTN_UP, buttonRepeatFrame))
        settingPointer = tracker.minMax(settingPointer-1, 0, 19);
    else if(pok.buttons.aBtn() && pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame))
        changeValues(-10);
    else if(pok.buttons.aBtn() && pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame))
        changeValues(10);
    else if(pok.buttons.bBtn() && pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame))
        changeValues(-100);
    else if(pok.buttons.bBtn() && pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame))
        changeValues(100);
    else if(pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame))
    {
        if (settingPointer == 0) instrPointer = tracker.minMax(instrPointer-1, 0, 14);
        else if (settingPointer == 4) instr[instrPointer].loop = tracker.minMax(instr[instrPointer].loop-1, 0, 1);
        else if (settingPointer == 5) instr[instrPointer].echo = tracker.minMax(instr[instrPointer].echo-1, 0, 1);
        else if (settingPointer == 6) instr[instrPointer].distort = tracker.minMax(instr[instrPointer].distort-1, 0, 1);
        else if (settingPointer == 7) instr[instrPointer].normalize = tracker.minMax(instr[instrPointer].normalize-1, 0, 1);
        else if (settingPointer == 8) instr[instrPointer].ADSR = tracker.minMax(instr[instrPointer].ADSR-1, 0, 1);
        changeValues(-1);
    }
    else if(pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame))
    {
        if (settingPointer == 0) instrPointer = tracker.minMax(instrPointer+1, 0, 14);
        else if (settingPointer == 4) instr[instrPointer].loop = tracker.minMax(instr[instrPointer].loop+1, 0, 1);
        else if (settingPointer == 5) instr[instrPointer].echo = tracker.minMax(instr[instrPointer].echo+1, 0, 1);
        else if (settingPointer == 6) instr[instrPointer].distort = tracker.minMax(instr[instrPointer].distort+1, 0, 1);
        else if (settingPointer == 7) instr[instrPointer].normalize = tracker.minMax(instr[instrPointer].normalize+1, 0, 1);
        else if (settingPointer == 8) instr[instrPointer].ADSR = tracker.minMax(instr[instrPointer].ADSR+1, 0, 1);
        changeValues(1);
    }
    else if(pok.buttons.pressed(BTN_A))
    {
        if(settingPointer == 1)
            pok.keyboard(instr[instrPointer].name, 10);
        else if(settingPointer == 2){
            uint8_t old = instr[instrPointer].wave;
            instr[instrPointer].wave = pok.menu(waveNames, 6);
            if (instr[instrPointer].wave == -1) instr[instrPointer].wave = old;
        }
        else if(settingPointer == 3){
            uint8_t old = instr[instrPointer].arpMode;
            instr[instrPointer].arpMode = pok.menu(arpModes, 16);
            if (instr[instrPointer].arpMode == -1) instr[instrPointer].arpMode = old;
        }
        /*else if(settingPointer == 16)
            //save instrument
        else if(settingPointer == 17)
            //load instrument
        else if(settingPointer == 18)
            //save song
        else if(settingPointer == 19)
            //load song
        */
    }
    else if(pok.buttons.pressed(BTN_C))
    {
        screen = 0;
        settingPointer = 0;
    }
}

void printSettings(){
    pok.display.setColor(1);
    pok.display.setCursor(2, 0);
    pok.display.print("Patch: ");
    pok.display.print(instrPointer, 10);
    pok.display.setCursor(2, fontH + 1);
    pok.display.print("Name: ");
    pok.display.print(instr[instrPointer].name);
    pok.display.setCursor(2, (2 * fontH) + 2);
    pok.display.print("Wave: ");
    pok.display.print(waveNames[instr[instrPointer].wave]);
    pok.display.setCursor(2, (3 * fontH) + 3);
    pok.display.print("Arp Mode: ");
    pok.display.print(arpModes[instr[instrPointer].arpMode]);
    pok.display.setCursor(2, (4 * fontH) + 4);
    pok.display.print("Loop: ");
    pok.display.print(instr[instrPointer].loop);
    pok.display.setCursor(2, (5 * fontH) + 5);
    pok.display.print("Echo: ");
    pok.display.print(instr[instrPointer].echo);
    pok.display.setCursor(2, (6 * fontH) + 6);
    pok.display.print("Dist: ");
    pok.display.print(instr[instrPointer].distort);
    pok.display.setCursor(2, (7 * fontH) + 7);
    pok.display.print("Norm: ");
    pok.display.print(instr[instrPointer].normalize);
    pok.display.setCursor(2, (8 * fontH) + 8);
    pok.display.print("ADSR: ");
    pok.display.print(instr[instrPointer].ADSR);
    pok.display.setCursor(2, (9 * fontH) + 9);
    pok.display.print("Attack: ");
    pok.display.print(instr[instrPointer].attack, 10);
    pok.display.setCursor(2, (10 * fontH) + 10);
    pok.display.print("Decay: ");
    pok.display.print(instr[instrPointer].decay, 10);
    pok.display.setCursor(2, (11 * fontH) + 11);
    pok.display.print("Sustain: ");
    pok.display.print(instr[instrPointer].sustain, 10);
    pok.display.setCursor(2, (12 * fontH) + 12);
    pok.display.print("Release: ");
    pok.display.print(instr[instrPointer].release, 10);
    pok.display.setCursor(2, (13 * fontH) + 13);
    pok.display.print("Pitch Bend Rate: ");
    pok.display.print(instr[instrPointer].pitchRate, 10);
    pok.display.setCursor(2, (14 * fontH) + 14);
    pok.display.print("Pitch Bend Max: ");
    pok.display.print(instr[instrPointer].pitchMax, 10);
    pok.display.setCursor(2, (15 * fontH) + 15);
    pok.display.print("Volume: ");
    pok.display.print(instr[instrPointer].volume, 10);
    pok.display.setCursor(2, (16 * fontH) + 16);
    pok.display.print("Save Instrument");
    pok.display.setCursor(2, (17 * fontH) + 17);
    pok.display.print("Load Instrument");
    pok.display.setCursor(2, (18 * fontH) + 18);
    pok.display.print("Save Song");
    pok.display.setCursor(2, (19 * fontH) + 19);
    pok.display.print("Load Song");
}

void drawPointer(){
    pok.display.drawBitmap(100, (settingPointer * fontH) + settingPointer, pointBitmap);
}
// following functions are not ready and not used
void saveSong(){
    fileOpen("SavedSong.rbs", FILE_MODE_READWRITE);
    for(uint8_t i = 0; i < (sizeof(rboyChar) - 1); i++){
        filePutChar(rboyChar[i]);
    }
    for(uint8_t i = 0; i < (sizeof(unknownChar) - 1); i++){
        filePutChar(unknownChar[i]);
    }
    for(uint8_t i = 0; i < (sizeof(BPMChar) - 1); i++){
        filePutChar(BPMChar[i]);
    }
    for(int8_t i = digitLength(tracker.getBPM()); i >= 0; i--){
        filePutChar(intToChar(tracker.getBPM(), i));
    }
    fileClose();
}

char intToChar(int _int, uint8_t digit){
    return ((_int % int(pow(10, (digit + 1)))) / int(pow(10, digit)))  + '0';
}

uint8_t digitLength(int _int){
    for (uint8_t i = 9; i > 0; i--){
        if((_int % int(pow(10, (i + 1))) / int(pow(10, i))) != 0) return i;
    }
}
