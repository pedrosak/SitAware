#ifndef Connection_h
#define Connection_h
#endif

#include <tuple>
#include "Questions.h"
#include "Interrogator.h"

//Class Connection
class Connection
{
public:

	//Variable used to notifie that FSX has quit
	static unsigned int quit;
	//Variables used with Simconnect
	static HANDLE hSimConnect;

	static double fsx_result;
	std::tuple<double, float> answer_buffer;
	double fsx_answer_buffer;
	double result_buffer;

	static int started_flag;

	Connection::Connection();	//Constructor
	void Connection::Connect(Questions *questions, Interrogator *interrogator);				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server
	double Connection::getAnswer();
	void Connection::Pause();
};