/*
 * AuthTable.h
 *
 * Created: 11/3/2013 10:47:16 AM
 *  Author: RavenXce
 */ 


#ifndef AUTHTABLE_H_
#define AUTHTABLE_H_

#include <Wstring.h>

class AuthTable{
	
	public:
		AuthTable();
		bool AddDevice(char*);
	private:
		struct Device
		{
			String MAC;
			String hash;
			Device* next;
		};
		Device* head;
	
};

#endif /* AUTHTABLE_H_ */