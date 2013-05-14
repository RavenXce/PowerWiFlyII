/*
* main.cpp
*
* Created: 27/1/2013 3:35:12 PM
*  Author: RavenXce
*/

#include "main.h"

bool sync;
bool state[4];
MODE mode;
WiFlyRNXV wifly(WIFLY_RX_PIN, WIFLY_TX_PIN);
AuthTable authTable;

void setup(){
	DDRD = 0b11111000;
	state[0] = state[1] = state[2] = state[3] = sync = false;
	
	sei();
	
	Serial.begin(WIFLY_DEFAULT_BAUD_RATE);
	Serial.println("Beginning..");
	
	Serial.println("Setting up WiFi driver..");	
	wifly.start();
	
	delay(2000);
	Serial.println("run");
	
	wifly.ExitCommandMode(); // in case already in cmd mode
	wifly.RebootWiFly();	 // re-enter cmd and reboot
	wifly.FactoryRESET();
	Serial.println("Setting up adhoc mode..");	
	wifly.EnterAdHoc();
	mode = ADHOC;
	// mode = UDP; // FOR DEBUG ONLY
}

void loop()
{	
	if(mode == UDP) 
	{
		if(sync == false)
		{			
			// Register arduino prototype on server
			wifly.SendUDP("<Prototype:pw9999:0:0000>");
			delay(500);
			sync = true;
		}
		int new_switch_status = wifly.CheckUART();	// Poll UART
		if (new_switch_status > 0)
		{
			Serial.print("Updating switch status now. New switch status code is: ");
			new_switch_status << 4;
			Serial.println(new_switch_status);
			PORTD |= new_switch_status & POWER_MASK;
			// compiler sucks? have to do this step separately.
			int inverse_mask = ~POWER_MASK;
			PORTD &= ~(new_switch_status | inverse_mask);
		}		
	}
		
	if (mode == ADHOC)
	{	
		//wifly.ForceConnect(); // debug force connect
		// Check if app has set wifly to networked mode
		if(wifly.NetworkConnected()){
			mode = NETWORKED;
			wifly.SetUDPMode();
			mode = UDP;
		}
	}	
}

void DecryptBuffer( char* buffer )
{
	// search the map for mac address of sender. use hash.
	return;
}

void AddDeviceToAuthTable()
{
	return;
}
