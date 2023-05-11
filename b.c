#include <stdio.h>
#include <wiringPi.h>

#define BUTTON_PIN 29

int main(void) {
    wiringPiSetup();
    pinMode(BUTTON_PIN, INPUT);
    pullUpDnControl(BUTTON_PIN, PUD_UP);

    while (1) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            printf("你好\n");
            delay(500);
     	}
	}
	return 0;
}
