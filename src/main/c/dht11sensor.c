#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include "dht11sensor.h"

#define MAX_PIN 30

void isr(uint8_t n);

void isr_0() {isr(0);}
void isr_1() {isr(1);}
void isr_2() {isr(2);}
void isr_3() {isr(3);}
void isr_4() {isr(4);}
void isr_5() {isr(5);}
void isr_6() {isr(6);}
void isr_7() {isr(7);}
void isr_8() {isr(8);}
void isr_9() {isr(9);}
void isr_10() {isr(10);}
void isr_11() {isr(11);}
void isr_12() {isr(12);}
void isr_13() {isr(13);}
void isr_14() {isr(14);}
void isr_15() {isr(15);}
void isr_16() {isr(16);}
void isr_17() {isr(17);}
void isr_18() {isr(18);}
void isr_19() {isr(19);}
void isr_20() {isr(20);}
void isr_21() {isr(21);}
void isr_22() {isr(22);}
void isr_23() {isr(23);}
void isr_24() {isr(24);}
void isr_25() {isr(25);}
void isr_26() {isr(26);}
void isr_27() {isr(27);}
void isr_28() {isr(28);}
void isr_29() {isr(29);}

void (*isrs[])(void) = {
	&isr_0, &isr_1, &isr_2, &isr_3, &isr_4, &isr_5, &isr_6, &isr_7, &isr_8, &isr_9, 
	&isr_10, &isr_11, &isr_12, &isr_13, &isr_14, &isr_15, &isr_16, &isr_17, &isr_18, &isr_19, 
	&isr_20, &isr_21, &isr_22, &isr_23, &isr_24, &isr_25, &isr_26, &isr_27, &isr_28, &isr_29, 
};

uint8_t data[MAX_PIN][5];
int lasttime[MAX_PIN];
uint8_t count[MAX_PIN];
uint8_t isrSetup[MAX_PIN] = {0};

onDataReady_t onDataReady;

void isr(uint8_t n) {
	if(count[n] >= 42)
		return;
	int time = micros();
	int delta = time - lasttime[n];
//	printf("count=%d, delta=%d\n", count, delta);
	lasttime[n] = time;
	count[n]++;
	
	if(count[n] <= 2)
		return;
	
	uint8_t idx = (count[n] - 3) >> 3;  // Skip first 2
	data[n][idx] <<= 1;
	if(delta > 105)
		data[n][idx] |= 1;
	
	if(count[n] < 42)
		return;

	int checksumOk = (((data[n][0] + data[n][1] + data[n][2] + data[n][3])&0xff) == data[n][4]);
	if(!checksumOk)
		printf("Wrong checksum: %d, %d, %d, %d, %d\n", data[n][0], data[n][1], data[n][2], data[n][3], data[n][4]);
	
	if(onDataReady)
		onDataReady(n, checksumOk ? 0 : 1, data[n][0], data[n][2]);
}

void setOnDataReady(onDataReady_t dataReady) {
	onDataReady = dataReady;
}

void read_dht11_dat(int n) {
	if(!isrSetup[n]) {
		if (wiringPiISR(n, INT_EDGE_FALLING, isrs[n])) {
			fprintf(stderr, "Unable to setup ISR : %s\n", strerror(errno));
			return;
		}
		isrSetup[n] = 1;
	}
	
	/* pull pin down for 18 milliseconds */
	pinMode(n, OUTPUT);
	digitalWrite(n, LOW);
	delay(18);
	/* then pull it up for 40 microseconds */
	digitalWrite(n, HIGH);
	count[n] = data[n][0] = data[n][1] = data[n][2] = data[n][3] = data[n][4] = 0;
	lasttime[n] = micros();
	delayMicroseconds(40);
	/* prepare to read the pin */
	pinMode(n, INPUT);
}