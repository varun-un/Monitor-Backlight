#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>
#include "SerialPort.hpp"

// g++ test-serial.cpp SerialPort.cpp -lgdi32 -o serial.exe

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
	while (arduino.isConnected()) {
		arduino.writeSerialPort("c", 1);
        cout<<"sent char"<<endl;
		arduino.readSerialPort(output, MAX_DATA_LENGTH);
        Sleep(5000);
	}
	return 0;
}