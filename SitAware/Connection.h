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
	float fsx_calculated_answer; //Variable used to conver FSX answer to a diferent unit (Ex. Radian to Degree)

	//Answer and question buffer variables
	static float fsx_answer_buffer;	//FSX fetched answer
	float result_buffer;		//calculation of user and fsx answer
	float user_input_answer;	//User input answer
	
	std::string input_string;		//user input for questions (1 to ask a questions)

	//Clock timer variables
	float  start_time_buffer;	//holds time of when question was asked
	float time_buffer = 0; //Holds value of how long it took for user to answer question
	float time_buffer_sum = 0;	//used to sum all the respons times to get the average


	//counts and var for calculations
	float old_time_average = 0; //calculate current average of time response
	float new_time_average = 0;		//store new overall average
	float average_increase_multiplier;	// how large was teh average increase
	int incorrect_count_error = 0;		//counts how many incorrect answer 
	int incorrect_count_time = 0;		//count how many incorrect due to time
	int number_questions_asked = 0;			// counts how many questions were asked
	int improvement_count = 0;		// how many times has the user shown impovement
	int suggestion_count = 0;

	//individual question buffer var
	double current_question_increase_multiplier = 0;
	double current_question_average_buffer = 0;

	static int started_flag;	//Flag to control initiation of FSX simulation

	//Functions
	Connection::Connection();	//Constructor
	void Connection::Connect(Questions *questions, std::string file_name);				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	static void CALLBACK MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext);	//Callback function to handel all the communcations with the simconnect server
	float Connection::getAnswer();
	void Connection::Pause();
	float Connection::startClock();
	float Connection::stopClock(float start_time);
	std::string Connection::localTime();
};