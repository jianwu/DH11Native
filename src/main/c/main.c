/* 
 * File:   main.cpp
 * Author: jianwu
 *
 * Created on January 10, 2016, 11:44 AM
 */
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "dht11sensor.h"

void dataReady(int n, int state, float hum, float temp) {
	printf("n:%d, state:%d, hum:%f, temp:%f\n", n, state, hum, temp);
}

int main(void) {
	int i = 0;
	printf("Raspberry Pi wiringPi DHT11 Temperature test program\n");
	setOnDataReady(dataReady);

	if (wiringPiSetup() == -1)
		exit(1);

	while (1) {
		read_dht11_dat(5);
		delay(1000); /* wait 1sec to refresh */
		printf("===============\n");
	}
}