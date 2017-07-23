#include "Pokitto.h"
#include "Tracker.h"
Pokitto::Core pok;

//main
int main(){
	pok.begin();
	initTracker();
	while(pok.isRunning()){
        if(pok.update()){
            Tracker();
        }
        if (play){
			if ((pok.getTime() - playTime) >= _tempo){
				_playSong();
				playTime = pok.getTime();
			}
        }
	}
}


