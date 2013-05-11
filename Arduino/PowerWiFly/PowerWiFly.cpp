/*
 * RoomTraq.cpp
 *
 * Created: 2/19/2013 11:10:55 PM
 *  Author: Raaj
 */ 

/************************************************************************
 * Room Traq
 
 This is a program together with an iOS App I wrote in 24 hours for the
 NUS Hackers Hackathon 2012. It intends to capture how many people have
 entered a room or event space, and send the data to a server, which can
 then be read via the App.
 
 (HOW IT WORKS)
 0. Device first checks if WiFi is available, if not keep checking
 1. Hitting the button to interrupt will make it enter Ad-Hoc mode
	allowing you to use the App to send your Wifi SSID and password
 2. Once Wifi is available, it pulses the URM37 ultrasonic sensors
 3. Using internal algorithms, a count is made depending on which
	sensor was hit first
 4. Every 30s, this data is sent via HTTP through the WiFly chip
 ************************************************************************/

//Includes and Definitions
#define F_CPU 16000000
#define ARDUINO 100
#include <Arduino.h>
#include <avr/io.h>
#include "Libs\WiFlyRNXV.h"
#include "Libs\DistanceDriver.h"

#define ADHOCPIN2		0
#define WIFLYRX			4
#define WIFLYTX			5
#define ENTRYSENSORRX	6
#define ENTRYSENSORTX	7
#define EXITSENSORRX	10
#define EXITSENSORTX	11

//Main Objects
WiFlyRNXV wiFly(WIFLYRX,WIFLYTX);
DistanceDriver distanceDriver(ENTRYSENSORRX,ENTRYSENSORTX,EXITSENSORRX,EXITSENSORTX);

//Program States
enum State {
	CONNECTED1,		//Still setting HTTP Mode
	CONNECTED2,		//Ready! Pinging sensors and sending data
	DISCONNECTED,	//No WiFi
	ADHOCCONNECT,	//In Ad-hoc mode, waiting for connection to WiFly
	ADHOCON,		//Connected, waiting for username and password response from App
	ADHOCEND,		//Data received, moving to check Wi-fi state
	UNKNOWN			//Unknown state, currently no handle
};
State state=UNKNOWN;

//Check WiFi Status
void updateWiFi(){
	boolean wiFiStatus=wiFly.CheckWiFiStatus();
	if(wiFiStatus)
		state=CONNECTED1;
	else
		state=DISCONNECTED;
		
	Serial.print("WiFi: ");
	Serial.println(wiFiStatus);
}

//Interrupt to set up in ad-hoc mode
bool buttonPress=false;
bool enterADHOCnow=false;
void InterruptAdHoc(){
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	if (interrupt_time - last_interrupt_time > 200)
	{
		Serial.println("INTERRUPT");
		
		enterADHOCnow=true;
	}
	last_interrupt_time = interrupt_time;
}

//Setup
void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(9600);
	Serial.println("ready");
	
	wiFly.start();
	wiFly.uart.println("exit");
	delay(2000);
	Serial.println("run");
	wiFly.RebootWiFly();
	updateWiFi();
	
	attachInterrupt(ADHOCPIN2, InterruptAdHoc, RISING);
}

//Every 30s, pauses the sensors and updates the count via WiFly
boolean runOnce=false;
unsigned long prevTime=0;
void sendMode(){
	if(runOnce==false){
		prevTime=millis();
		runOnce=true;
	}
	
	unsigned long currTime = millis();
	if (currTime - prevTime > 30000)
	{
		Serial.println("Pulse paused");
		wiFly.start();

		int x=distanceDriver.entryCount;
		char* str="aa";
		str=itoa(x, str, 10);
		
		wiFly.uart.println("exit");
		wiFly.uart.flush();
		Serial.println("hang");
		
		wiFly.SendResponse(str);
		prevTime=millis();
		wiFly.end();
	}
}

//Main Loop
void loop() {

	//Interrupted-Set up Ad-Hoc mode
	if(enterADHOCnow==true){
		enterADHOCnow=false;
		wiFly.start();
		wiFly.EnterAdHoc();
		state=ADHOCCONNECT;
		Serial.println("doneX");
		wiFly.uart.flush();
	}
	
	//State Modes
	switch(state){
		case DISCONNECTED:
			updateWiFi();
			break;
			
		case ADHOCCONNECT:{
			if(wiFly.AdHocConnected()){
				Serial.println("EnterAdhocon");
				state=ADHOCON;
				delay(3000);
				wiFly.uart.flush();
				Serial.println("Ready to receive");
			}			
			break;}
			
		case ADHOCON:{
			if(wiFly.AdHocEnded())
				state=DISCONNECTED;
			break;}
			
		case UNKNOWN:
			break;
			
		case CONNECTED1:
			wiFly.SetHTTP();
			wiFly.end();
			state=CONNECTED2;
			Serial.println("<S>");
			break;
			
		case CONNECTED2:
			sendMode();
			distanceDriver.pingCount();
			break;
			
		default:
			break;
	}
	
}