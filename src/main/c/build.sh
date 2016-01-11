JDK_HOME=/usr/lib/jvm/jdk-8-oracle-arm-vfp-hflt

echo "Compile"
gcc -I $JDK_HOME/include -I $JDK_HOME/include/linux -o dht11native.so -shared dht11native.c dht11sensor.c -l wiringPi

echo "Copy so file to /usr/lib/"
sudo cp dht11native.so /usr/lib/

echo "Create symbolic link for the library"
sudo rm /usr/lib/libdht11native.so
sudo ln -s /usr/lib/dht11native.so /usr/lib/libdht11native.so

echo "Updated library:"
ls -lart /usr/lib/libdht11native.so
