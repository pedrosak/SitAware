#ifndef Connection_h
#define Connection_h
#endif

#include "Questions.h"

//Class Connection
class Connection
{
public:

	//Variable used to notifie that FSX has quit
	static unsigned int quit;
	//Variables used with Simconnect
	static HANDLE hSimConnect;


	Connection::Connection();	//Constructor
	void Connection::Connect(Questions *questions);				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server

	void Connection::Pause();
};