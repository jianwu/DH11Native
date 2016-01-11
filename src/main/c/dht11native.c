#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "com_zhm_dht11native_DHT11Native.h"
#include "dht11sensor.h"

JavaVM * g_vm;
JNIEnv * g_env;
jclass g_class;
jmethodID g_mid;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	return JNI_VERSION_1_6;
}

void dataReady(int n, int state, float hum, float temp) {
	JNIEnv * g_env;
	int getEnvStat = (*g_vm)->GetEnv(g_vm, (void **)&g_env, JNI_VERSION_1_6);
	if (getEnvStat == JNI_EDETACHED) {
		if ((*g_vm)->AttachCurrentThread(g_vm, (void **) &g_env, NULL) != 0) {
			printf("Failed to attach\n");
			return;
		}
	}else if (getEnvStat == JNI_EVERSION) {
		printf("GetEnv: version not supported\n");
		return;
	}
	
	(*g_env)->CallStaticVoidMethod(g_env, g_class, g_mid, n, state+2, hum, temp);
	if ((*g_env)->ExceptionCheck(g_env)) {
		(*g_env)->ExceptionDescribe(g_env);
	}

	(*g_vm)->DetachCurrentThread(g_vm);
}

JNIEXPORT void JNICALL Java_com_zhm_dht11native_DHT11Native_readNative (JNIEnv * env, jclass cls, jint pin) {
	(*env)->GetJavaVM(env, &g_vm);
	g_class = cls;
	g_mid = (*env)->GetStaticMethodID(env, cls, "onReady", "(IIFF)V");
	setOnDataReady(dataReady);
	read_dht11_dat(pin);
}