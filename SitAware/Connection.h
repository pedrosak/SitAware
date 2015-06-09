#ifndef Connection_h
#define Connection_h
#endif

#include "Questions.h"

//Data definition for requests sent to SImConnect
static enum DATA_REQUEST_ID
{
	REQUEST,
};


//Data definition of client EVENTS with the SimConnect server
static enum DATA_DEFINE_ID
{
	DEFINITION,
};

static enum EVENT_ID
{
	EVENT_SIM_START
};
//Class Connection
class Connection
{
public:

	//Variable used to notifie that FSX has quit
	static unsigned int quit;
	//Variables used with Simconnect
	static HANDLE hSimConnect;

	Questions questions = Questions::Questions();
	Connection::Connection();	//Constructor
	static char* Exception_String(DWORD Recv_Exception);	//Deals with any expections thrown by SimCOnnect Server
	void Connection::Connect();				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server
};