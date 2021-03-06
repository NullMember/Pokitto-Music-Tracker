/*
    Tracker Arrays
    Updated 5.7.2017
*/

const char noteLetters[12][2] = {{'C', '-'}, {'C', '#'}, {'D', '-'}, {'D', '#'}, {'E', '-'}, {'F', '-'},
                           {'F', '#'}, {'G', '-'}, {'G', '#'}, {'A', '-'}, {'A', '#'}, {'B', '-'}};

const char* waveNames[]={"OFF", "SQUARE", "SAW", "TRIANGLE", "NOISE", "TONE NOISE"};
const char* arpModes[]={"OFF" ,"M Slow", "M Med", "M Fast", "m Slow", "m Med", "m Fast", "Vib Slow", "Vib Med", "Vib Fast", "Oct Slow", "Oct Med", "Oct Fast", "Funk Slow", "Funk Med", "Funk Fast"};

const uint8_t pauseBitmap[] = {8, 5,
0b00010100,0b00010100,
0b00010100,0b00010100,
0b00010100,0b00010100,
0b00010100,0b00010100,
0b00010100,0b00010100};

const uint8_t playBitmap[] = {8, 5,
0b00010101,0b01000000,
0b00010101,0b01010000,
0b00010101,0b01010100,
0b00010101,0b01010000,
0b00010101,0b01000000};

const uint8_t stopBitmap[] = {8, 5,
0b00010101,0b01010100,
0b00010101,0b01010100,
0b00010101,0b01010100,
0b00010101,0b01010100,
0b00010101,0b01010100};

const uint8_t pointBitmap[] = {8, 5,
0b00000001,0b01010100,
0b00000101,0b01010100,
0b00010101,0b01010100,
0b00000101,0b01010100,
0b00000001,0b01010100};
