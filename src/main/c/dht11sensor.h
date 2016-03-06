/* 
 * File:   dht11sensor.h
 * Author: jianwu
 *
 * Created on January 10, 2016, 5:55 PM
 */

#ifndef DHT11SENSOR_H
#define	DHT11SENSOR_H
typedef void (*onDataReady_t)(int n, int status, float hum, float temp);
void setOnDataReady(onDataReady_t dataReady);
void read_dht11_dat(int n);
#endif	/* DHT11SENSOR_H */

