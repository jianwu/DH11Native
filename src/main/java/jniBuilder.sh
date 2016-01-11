echo "Compile java class"
javac com/zhm/dht11native/DHT11Native.java

echo "Create JNI header"
javah -d com/zhm/dht11native/ com.zhm.dht11native.DHT11Native
cp com/zhm/dht11native/*.h ../c

#echo "Compile org/mandfer/dht11/dht11sensor.c with wiringPi library"
#gcc -I /usr/lib/jvm/java-1.7.0-openjdk-armhf/include/ -o org/mandfer/dht11/dht11sensor.so -shared org/mandfer/dht11/dht11sensor.c -l wiringPi

#echo "Copy new org/mandfer/dht11/dht11sensor.so in /usr/lib/"
#sudo cp org/mandfer/dht11/dht11sensor.so /usr/lib/
#
#echo "Create symbolic link for the library"2
#sudo ln -s /usr/lib/dht11sensor.so /usr/lib/libdht11sensor.so
#
#echo "Updated library:"
#ls -lart /usr/lib/libdht*

# Run the example class with 
#sudo java org.mandfer.dht11.DHT11SensorReader
# 
# The output should be similar to
# INFO: temp: 22.0, hum 35.0


