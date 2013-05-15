/*
 * WiFlyRNXV.h
 *
 * Created: 2/19/2013 11:29:18 PM
 *  Author: Raaj
 */ 

#ifndef WIFLYRNXV_H_
#define WIFLYRNXV_H_

/************************************************************************
 * WiFly
 
 1. Clean WiFly serial driver written from scratch
 2. Used to interface with main class to send and receive data
 ************************************************************************/

//Include Paths
#include <SoftwareSerial.h>
#include <Streaming.h>
#include <avr/pgmspace.h>

//RX TX Pins
#define ARDUINO_RX_PIN				2
#define ARDUINO_TX_PIN				3

//Hardware settings
#define MAX_SWITCHES				4

//Default ports settings for WiFly
#define WIFLY_DEFAULT_REMOTE_PORT	80
#define WIFLY_DEFAULT_LOCAL_PORT	2000
#define WIFLY_DEFAULT_BAUD_RATE		9600
#define WIFLY_DEFAULT_DELAY			2000

//Char Buffer Sizes for Command and Response
#define RESPONSE_BUFFER_SIZE		80

//CMD Wait Times based on manual
#define COMMAND_MODE_GUARD_TIME 	300
#define DEFAULT_WAIT_TIME			2000
#define DEFAULT_DELAY_TIME			500
#define TIMEOUT_TIME				3000

//Local Remote Server Address Port
const char* const REMOTE_ADDRESS="192.168.1.8";
const char* const REMOTE_PORT="2005";
const char* const LOCAL_PORT="2000";

//Communication Keywords
#define KEYWORD_SYNC "SYNC"
#define KEYWORD_REQUEST_STATE "STATE"
#define KEYWORD_TOGGLE_MODES "MODE"
#define KEYWORD_TOGGLE_SWITCHES "SWITCH"
#define KEYWORD_FRONT_DELIMITER '<'
#define KEYWORD_END_DELIMITER '>'

//Define commands
const char* const COMMAND_MODE="$$$";
const char* const COMMAND_EXIT="exit";

using namespace std;

class WiFlyRNXV{

	public:
	WiFlyRNXV(byte pinReceive, byte pinSend);					//Constructor with Pins for UART
	SoftwareSerial uart;										//SoftwareSerial driver
	
	void Begin();
	void End();
	void RebootWiFly();
	void EnterAdHoc();
	void FactoryRESET();
	void SetUDPMode();
	void SendUDP(char* value);
	void ForceConnect(); // for debug mode
	int CheckUART();
	int ProcessResponse(char* buffer);
	boolean NetworkConnected();
	boolean AdHocEnded();
	boolean EnterCommandMode();
	boolean ExitCommandMode();
	boolean CheckWiFiStatus();
	

	private:
	boolean inCommandMode;
	boolean wifiStatus;
	char* getBufferResponse();
	boolean checkBufferResponse(char* compareValue,int timeout);
	boolean checkForString(char* compare1, char* compare2);
};



#endif /* WIFLYRNXV_H_ */