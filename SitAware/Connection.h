#ifndef Connection_h
#define Connection_h
#endif

#include <tuple>
#include "Questions.h"
#include "Interrogator.h"
#include "SimConnect.h"
//Class Connection
class Connection
{
public:

	//Variable used to notifie that FSX has quit
	static unsigned int quit;
	//Variables used with Simconnect
	static HANDLE hSimConnect;

	std::tuple<long, long> answer_buffer;
	double fsx_answer_buffer;
	double result_buffer;

	static int started_flag;
	int input;		//user input for questions (1 to ask a questions)
	float user_input_answer;	//Answer if its a nubmer integer
	float  start_time_buffer;	//holds time of when question was asked
	float time_buffer; //Holds value of how long it took for user to answer question

	Connection::Connection();	//Constructor
	void Connection::Connect(Questions *questions);				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server
	double Connection::getAnswer();
	void Connection::Pause();
	clock_t Connection::startClock();
	clock_t Connection::stopClock(float start_time);
};