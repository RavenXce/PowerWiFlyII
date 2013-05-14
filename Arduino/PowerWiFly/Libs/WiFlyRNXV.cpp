/*
* WiFlyRNXV.cpp
*
* Created: 2/19/2013 11:39:21 PM
*  Author: Raaj, Ivan
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

//Check if two strings/char* are the same
boolean WiFlyRNXV::checkForString(char* responseBuffer,char* compare){
	char* pch = NULL;
	pch = strstr (responseBuffer,compare);
	if(pch == NULL)
	return false;
	else
	return true;
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

int WiFlyRNXV::CheckUART()
{
	if(uart.available()){
		
		Serial.println("UDP command received..");
		
		char* buffer;
		char chResponse;
		int  bufpos = 0;
		
		while(uart.available()){
			
			// Read uart buffer
			chResponse = uart.read();
			Serial.print(chResponse);
			
			// Check for matching responses
			if (chResponse == KEYWORD_FRONT_DELIMITER)
			{
				Serial.println("Found start of response phrase.");
				
				// Reset the memory buffer
				buffer = (char*) malloc(RESPONSE_BUFFER_SIZE);
				memset (buffer, '\0', RESPONSE_BUFFER_SIZE-1);
				buffer[0] = chResponse;
				buffer++;
				
				// Fill memory buffer
				bool timeout = false;
				unsigned long startTime = millis();
				while(uart.available() && chResponse != KEYWORD_END_DELIMITER && !timeout)
				{
					chResponse = uart.read();
					Serial.print(chResponse);
					if(bufpos < RESPONSE_BUFFER_SIZE-1)
					{
						buffer[bufpos] = chResponse;
						bufpos++;
					}
					else
					{
						Serial.println("buffer overflow");
						bufpos=0;
					}
					if ((millis()-startTime) > TIMEOUT_TIME)
					timeout = true;
				}
				buffer[bufpos] = '\0'; // end of buffer
				
				// Flush uart buffer
				Serial.println("");
				uart.flush();
				
				return ProcessResponse(buffer);
			}
		}
	}
	
	// Nothing on UART
	else return -1;
}

int WiFlyRNXV::ProcessResponse(char* buffer)
{
	Serial.println("Processing following response:");
	Serial.println(buffer);
	
	// Check if sync command issued
	if(checkForString(KEYWORD_SYNC,buffer))
	{
		Serial.println("Sending UDP sync request.");
		SendUDP("<Prototype:pw9999:2:0000>");
		return -1;
	}
	// Check if switches should be toggled
	else
	{
		int i;
		int switch_status = 0;
		bool found_command = false;
		// Check for delimiting symbols.
		if(*(buffer) == KEYWORD_FRONT_DELIMITER &&
			*(buffer+MAX_SWITCHES+1) == KEYWORD_END_DELIMITER)
		{
			found_command = true;
			for(i=0; i<MAX_SWITCHES; i++)
			{
				if(*(buffer+i+1) != '1' && *(buffer+i+1) != 0)
					found_command = false;
			}
		}
		if(found_command == true)
		{
			Serial.println("Found switch data!");
			int j;
			for(j=0; j++; j<MAX_SWITCHES)
			{
				// Arrange flags
				if (*(buffer+i+j+1) == '1')
				switch_status += 1;
				// If zero do nothing
				// If not zero then don't adjust switch. nyi.
				// else if (*(buffer+i+j+1) == '1')
				switch_status << 1;
			}
			return switch_status;
		}
		else
		{
			Serial.println("Could not find valid data.");
			return -1;
		}
	}
}

// Procedure to enter a WIFI network for debug purposes
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