#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>
#include "SerialPort.hpp"

// g++ test-serial.cpp -lgdi32 -o serial.exe

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];

// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them
char port[] = "\\\\.\\COM6";

int main(int argc, char **argv) {

	SerialPort arduino(port);
	if(arduino.isConnected()){
		cout<<"Connection made"<<endl<<endl;
	}
	else{
		cout<<"Error in port name"<<endl<<endl;
	}
	if (arduino.isConnected()){
		arduino.writeSerialPort("charArray", MAX_DATA_LENGTH);
		arduino.readSerialPort(output, MAX_DATA_LENGTH);
	}
	return 0;
}