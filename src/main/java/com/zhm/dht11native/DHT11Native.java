/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.zhm.dht11native;

import java.util.function.Consumer;

/**
 * @author jianwu
 */
public class DHT11Native {

	enum State { INITIAL, REQUESTED, SUCCESSED, FAILED_CHECKSUM, FAILED_TIMEOUT }

	public static class Data {
		int pin;
		float temperature;
		float humidity;
		State state = State.INITIAL;
		long lastReadTimestamp;	
		public Data(int pin) { this.pin = pin; }		
	}
	
	public final static int MAX_GPIO = 30;
	static Data[] data = new Data[MAX_GPIO];

	public static Consumer<Data> callback;

	static {
		System.loadLibrary("dht11native");
	}
	
	public static void read(int pin) {
		if(pin >= MAX_GPIO)
			throw new IllegalArgumentException("Invalid PIN, PIN should less than: " + MAX_GPIO);
		
		if(data[pin] == null)
			data[pin] = new Data(pin);
		
		long now = System.currentTimeMillis();
		if((now - data[pin].lastReadTimestamp) < 500)
			throw new IllegalStateException("Read too frequently, please wait for at least 500ms to issue another reading");
		
		data[pin].state = State.REQUESTED;
		data[pin].lastReadTimestamp = now;
		
		readNative(pin);
	}

	private static native void readNative(int pin);

	public static void onReady(int pin, int stateId, float humidity, float temperature) {
		data[pin].state = State.values()[stateId];
		data[pin].temperature = temperature;
		data[pin].humidity = humidity;
		if(callback != null)
			callback.accept(data[pin]);
	}
	
	public static void main(String[] args) throws Exception {
		callback = (data)-> {
			System.out.println("state:" + data.state + "; Temp:" + data.temperature + "; hum:" + data.humidity);
		};
		while(true) {
			read(5);
			Thread.sleep(1000);
		}
	}
}
