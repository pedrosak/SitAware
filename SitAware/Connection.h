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

	//Variables
	static unsigned int quit; //Variable used to notifie that FSX has quit
	static HANDLE hSimConnect;	//Variables used with Simconnect

	//Answer and question buffer variables
	static float fsx_answer_buffer;	//FSX fetched answer
	float result_buffer;		//calculation of user and fsx answer
	float user_input_answer;	//User input answer

	int input;		//user input for questions (1 to ask a questions)

	//Clock timer variables
	float  start_time_buffer;	//holds time of when question was asked
	float time_buffer; //Holds value of how long it took for user to answer question

	static int started_flag;	//Flag to control initiation of FSX simulation

	//Functions
	Connection::Connection();	//Constructor
	void Connection::Connect(Questions *questions);				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server
	float Connection::getAnswer();
	void Connection::Pause();
	float Connection::startClock();
	float Connection::stopClock(float start_time);
};