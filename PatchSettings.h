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

void saveInstrInSong();

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
        //else if(settingPointer == 16)
            //save instrument
        //else if(settingPointer == 17)
            //load instrument
        else if(settingPointer == 18)
            tracker.saveSong();
            saveInstrInSong();
        //else if(settingPointer == 19)
            //load song
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
    pok.display.drawBitmap(130, (settingPointer * fontH) + settingPointer, pointBitmap);
}

void saveInstrInSong(){
    for (uint8_t j = 0; j < tracker.getLP() + 1; j++){
        if (instr[j].name[0] == 0) tracker.filePutInt(j);
        else tracker.filePrint(instr[j].name, sizeof(instr[j].name));
        tracker.NL();
        tracker.filePrint(waveChar, sizeof(waveChar));
        tracker.filePutInt(instr[j].wave);
        tracker.NL();
        tracker.filePrint(volChar, sizeof(volChar));
        tracker.filePutInt(instr[j].volume);
        tracker.NL();
        tracker.filePrint(pitchRateChar, sizeof(pitchRateChar));
        tracker.filePutInt(instr[j].pitchRate);
        tracker.NL();
        tracker.filePrint(pitchMaxChar, sizeof(pitchMaxChar));
        tracker.filePutInt(instr[j].pitchMax);
        tracker.NL();
        tracker.filePrint(vibChar, sizeof(vibChar));
        tracker.filePutInt(0);
        tracker.NL();
        tracker.filePrint(arpChar, sizeof(arpChar));
        tracker.filePutInt(instr[j].arpMode);
        tracker.NL();
        tracker.filePrint(ADSRChar, sizeof(ADSRChar));
        tracker.filePutInt(instr[j].ADSR);
        tracker.NL();
        tracker.filePrint(attackChar, sizeof(attackChar));
        tracker.filePutInt(instr[j].attack);
        tracker.NL();
        tracker.filePrint(decayChar, sizeof(decayChar));
        tracker.filePutInt(instr[j].decay);
        tracker.NL();
        tracker.filePrint(sustainChar, sizeof(sustainChar));
        tracker.filePutInt(instr[j].sustain);
        tracker.NL();
        tracker.filePrint(releaseChar, sizeof(releaseChar));
        tracker.filePutInt(instr[j].release);
        tracker.NL();
        tracker.filePrint(loopChar, sizeof(loopChar));
        tracker.filePutInt(instr[j].loop);
        tracker.NL();
        tracker.filePrint(echoChar, sizeof(echoChar));
        tracker.filePutInt(instr[j].echo);
        tracker.NL();
        tracker.filePrint(overdriveChar, sizeof(overdriveChar));
        tracker.filePutInt(instr[j].distort);
        tracker.NL();
        tracker.filePrint(drumChar, sizeof(drumChar));
        tracker.filePutInt(instr[j].normalize);
        tracker.NL();
    }
    fileClose();
}
