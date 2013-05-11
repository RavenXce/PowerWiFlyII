/*
 * WiFlyRNXV.cpp
 *
 * Created: 2/19/2013 11:39:21 PM
 *  Author: Raaj
 */ 

#include "WiFlyRNXV.h"

//Constructor-Start listen on uart
WiFlyRNXV::WiFlyRNXV(byte pinReceive, byte pinSend) : uart (pinReceive, pinSend){
	inCommandMode=false;
	wifiStatus=false;
}

void WiFlyRNXV::start(){
	uart.begin(WIFLY_DEFAULT_BAUD_RATE);
	uart.listen();
	uart.flush();
}

void WiFlyRNXV::end(){
	uart.end();
}

//Check if two strings/char* are the same
boolean checkForString(char* responseBuffer,char* compare){
	char * pch=NULL;
	pch = strstr (responseBuffer,compare);
	if(pch==NULL)
	return false;
	else
	return true;
}

//Simply empty the buffer into the a char* and return (64 size)
char* WiFlyRNXV::getBufferResponse(){
	
	//Variables
	char* responseBuffer;
	int  bufpos = 0;											//Buffer position
	char chResponse = 'A';										//Initial character response
	int  bufsize = RESPONSE_BUFFER_SIZE -1;						//terminating null for bufsize
	
	//Reset the buffer
	responseBuffer = (char*) malloc(RESPONSE_BUFFER_SIZE);
	memset (responseBuffer, '\0', bufsize);
	
	//Fill the buffer
	while(uart.available()){
		
		chResponse = uart.read();
		Serial.print(chResponse);
		if(bufpos<bufsize){
			responseBuffer[bufpos]=chResponse;
			bufpos++;
		}else{
			Serial.println("buffer overflow");
			bufpos=0;
		}
	}	
	
	//Serial.println(responseBuffer);
	Serial.println("");
	uart.flush();
	return responseBuffer;
}

//Check buffer with a particular 
boolean WiFlyRNXV::checkBufferResponse(char* compareValue,int timeout){
	
	//Variables
	char* responseBuffer;										//Buffer for response
	boolean bufRead = true;										//Finish Reading
	int  bufpos = 0;											//Buffer position
	char chResponse = 'A';										//Initial character response
	int  bufsize = RESPONSE_BUFFER_SIZE -1;						//terminating null for bufsize
	boolean compareSuccess=false;								//Compare Success

	//Reset the buffer
	responseBuffer = (char*) malloc(RESPONSE_BUFFER_SIZE);
	memset (responseBuffer, '\0', bufsize);

	//Fill the buffer
	unsigned long startTime = millis();
	while(bufRead){
		
		//Start getting values
		if(uart.available()){
			chResponse = uart.read();
			Serial.print(chResponse);
			//Check this, buffer overflow
			if(bufpos<bufsize){
				responseBuffer[bufpos]=chResponse;
				bufpos++;
			}else{
				Serial.println("Buffer overflow");
				bufpos=0;
			}
		}

		//Check for existence of the comparison string, or if timeout stop
		if(checkForString(responseBuffer,compareValue)){
			compareSuccess=true;
			bufRead=false;
		}else if((millis()-startTime)>timeout){
			compareSuccess=false;
			bufRead=false;
		}
	}
	
	//Serial.println(responseBuffer);
	Serial.println("");
	uart.flush();
	return compareSuccess;
}	

//Enter Command Mode
boolean WiFlyRNXV::EnterCommandMode(){
	uart.flush();
	uart.print(COMMAND_MODE);
	delay(COMMAND_MODE_GUARD_TIME);
	if(checkBufferResponse("CMD",TIMEOUT_TIME))
		inCommandMode=true;
	else
		inCommandMode=false;	
		
	return inCommandMode;
}

//Exit Command Mode
boolean WiFlyRNXV::ExitCommandMode(){
	uart.println("exit");
	delay(COMMAND_MODE_GUARD_TIME);
	inCommandMode=false;
	
	return inCommandMode;
}

//Exit Command Mode
void WiFlyRNXV::RebootWiFly(){
	if(!inCommandMode)	EnterCommandMode();
	delay(100);
	uart.print("reboot\r");
	inCommandMode=false;
}

//Check WiFi Status
boolean WiFlyRNXV::CheckWiFiStatus(){
	
	if(inCommandMode){
		ExitCommandMode();
		delay(1000);
	}
	
	if(checkBufferResponse("OK",TIMEOUT_TIME))
		wifiStatus=true;
	else
		wifiStatus=false;
	
		
	return wifiStatus;

}

void WiFlyRNXV::FactoryRESET(){
	if(!inCommandMode)	EnterCommandMode();
	int delayW=500;
	delay(delayW);
	
	Serial.println("factory RESET"); uart.flush();
	uart.println("factory RESET"); delay(delayW); getBufferResponse();
	Serial.println("reboot"); uart.flush();
	uart.println("reboot"); delay(delayW); getBufferResponse();
	delay(2000);
	Serial.println("Factory RESET done");
	inCommandMode=false;
}

void WiFlyRNXV::EnterAdHoc(){
	
	int delayW=500;
	
	if(!inCommandMode)	EnterCommandMode();
	Serial.println("Attempting cmd mode");
	
	delay(1000);
	// Setup adhoc network
	Serial.println("set ip address 169.254.1.1"); uart.flush();
	uart.println("set ip address 169.254.1.1"); delay(delayW);
	Serial.println("set ip netmask 255.255.0.0"); uart.flush();
	uart.println("set ip netmask 255.255.0.0"); delay(delayW);
	Serial.println("set ip dhcp 0"); uart.flush();
	uart.println("set ip dhcp 0"); delay(delayW);	
	Serial.println("set ip proto 2"); uart.flush();
	uart.println("set ip proto 2"); delay(delayW);
	Serial.println("set wlan ssid WiFly-GSX-XX"); uart.flush();
	uart.println("set wlan ssid WiFly-GSX-XX"); delay(delayW);
	Serial.println("set wlan channel 1"); uart.flush();
	uart.println("set wlan channel 1"); delay(delayW);

	// Create adhoc network
	Serial.println("set wlan join 4"); uart.flush();
	uart.println("set wlan join 4"); delay(delayW);
	Serial.println("save"); uart.flush();
	uart.println("save"); delay(delayW);
	Serial.println("reboot"); uart.flush();
	uart.println("reboot"); delay(delayW);
	delay(2000);
	
	inCommandMode=false;
	
	uart.flush();
	Serial.println("Done AdHoc");
}

boolean WiFlyRNXV::NetworkConnected(){
	boolean check=false;
	if(uart.available()){
		Serial.println("Checking if connected");		
		check=checkBufferResponse("OK",TIMEOUT_TIME);
	}		
	if(check){
		Serial.println("Connected!!!");
		return true;
	}else{
		return false;
	}
}

boolean WiFlyRNXV::AdHocEnded(){
	boolean check=false;
	if(uart.available()){	
		 check=true;
		 Serial.println("datarec");
		 delay(4000);
		 uart.flush();
	}		 
	if(check){
		Serial.println("READY!");
		return true;
	}else{
		return false;
	}
}

void WiFlyRNXV::SetUDPMode(){
	int delayW=500;
	
	delay(2000);
	if(!inCommandMode)	EnterCommandMode();
	Serial.println("Attempting cmd mode");
	
	delay(delayW);
	Serial.println("set ip proto 1"); uart.flush();
	uart.println("set ip proto 1"); delay(delayW);
	Serial.println("set ip host 192.168.1.8"); uart.flush();
	uart.println("set ip host 192.168.1.8"); delay(delayW);
	Serial.println("set ip remote 2005"); uart.flush();
	uart.println("set ip remote 2005"); delay(delayW);
	Serial.println("set ip local 2000"); uart.flush();
	uart.println("set ip local 2000"); delay(delayW);

	Serial.println("save"); uart.flush();
	uart.println("save"); delay(delayW);
	Serial.println("reboot"); uart.flush();
	uart.println("reboot"); delay(delayW);
	delay(delayW);
	
	inCommandMode=false;
	Serial.println("Done SetUDPMode");
	uart.flush();
	delay(2000);
}

void WiFlyRNXV::SendUDP(char* value){
	Serial.println("sending udp string:");
	Serial.println(value);
	uart.flush();
	uart.println(value);
}

int WiFlyRNXV::ProcessCommand()
{	
	if(!uart.available()) return -1;
	
	Serial.println("Processing command..");
	
	char* buffer;
	
	buffer = getBufferResponse();
	
	// Update switches if just rebooted
	if(checkForString(KEYWORD_SYNC,buffer))
	{		
		SendUDP("<Prototype:pw9999:2:0000>");
		return -1;
	}
	// Check if is switch toggle command.	
	else
	{
		int i;
		int switch_status = 0;
		bool found_command = false;
		// Check for delimiting symbols.	
		for(i=0; i++; i<RESPONSE_BUFFER_SIZE-(MAX_SWITCHES+2)-1)
		{
			if(*(buffer+i) == KEYWORD_FRONT_DELIMITER &&
				*(buffer+i+MAX_SWITCHES+2) == KEYWORD_END_DELIMITER)
				{
					found_command = true;
					break;				
				}
		}		
		if(found_command == true)
		{
			int j;
			for(j=0; j++; j<MAX_SWITCHES)
			{
				// Arrange flags
				if (*(buffer+i+j+1) == '1')
				switch_status += 1;
				// If zero do nothing
				// If not zero then don't adjust switch. nyi.
				// else if (*(buffer+i+j+1) == '1')
				
				switch_status << 10;
			}
		}
		else return -1;
	}
	
	/*
	// State request and atomic individual toggle with error checking. nyi.
	if (checkBufferResponse(KEYWORD_REQUEST_STATE,1000))
	{		
		// send power relay state
	}
    else if (checkBufferResponse(KEYWORD_TOGGLE_SWITCHES,1000))
	{
		// get switch number
		// toggle and save state => make atomic!!
		switch(buffer[0])
		{
			case 'A':
			// switch A to buffer[1].
			break;
			case 'B':
			break;
			case 'C':
			break;
			case 'D':
			break;
			default:
			// invalid cmd, clear buffer
			break;
		}
	}
	*/
	
	// possible cmds:
	// send state
	// toggle power switches => check for consistancy with sender.
	// set a timer
	// sync (encryption):
	// check for sync (pushbutton). => generate encrypted pw seed and send.
	// phone app will decrypt and use seed to hash(encrypt) all cmds.
}

void WiFlyRNXV::ForceConnect()
{
	EnterCommandMode();
	uart.println("set wlan ssid gopaln");
	uart.println("set wlan phrase 6562868014");
	uart.println("set wlan join 1");
	uart.println("set wlan channel 0");
	uart.println("set ip dhcp 1");
	uart.println("reboot");	
	
	return;
}