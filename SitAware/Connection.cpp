#include "stdafx.h"
#include <bitset>

unsigned int Connection::quit;

HANDLE Connection::hSimConnect = NULL;

int Connection::started_flag = 0;

float Connection::fsx_answer_buffer = 0;

int z;

//Data definition for requests sent to SImConnect
static enum DATA_REQUEST_ID
{
	REQUEST,
	REQUEST_AIRPORT
};

//Struct for a single item returned by FSX
struct SingleAnswer
{
	int answer_id;
	float answer_value;
};

//Struct for tagged received data
struct Answers
{
	//Memory space for 1000 answers
	SingleAnswer answer[1000];
};

struct Airport
{
	char Icao_near[9];
	double Lat_near;
	double Long_near;
	double Alt_near;
};

//Data definition of client EVENTS with the SimConnect server
static enum DATA_DEFINE_ID
{
	DEFINITION,
};

static enum EVENT_ID
{
	EVENT_SIM_START,
	EVENT_SIM_STOP,
	EVENT_TIMER
};


//Constructor. Defines HANDLE variable
Connection::Connection()
{
	//Future versions use constructor and destructor to connect and disconnect to FSX
}

//Initiate connectino with SimConnect
void Connection::Connect(Questions *questions)
{
	HRESULT hr;

	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (Connection::hSimConnect == NULL)
	{
		printf("\nAttempting to connect to Flight Simulator X\n");

		//Call SimConnect function to open a new connection.
		hr = SimConnect_Open(&Connection::hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0);

		if (hr == S_OK)
		{
			//SimConnect_Open returned S_OK
			printf("\nConnection established with Flight Simulator X\n");
			 
			//data definition set up. This sets the FSX simulation variable to a client defined object definition (in this case enum DEFINITION is the client defined data definition)
			for (int i = 1; i <= questions->getNumberofElements(); i++)
			{

				hr = SimConnect_AddToDataDefinition(Connection::hSimConnect, DEFINITION, questions->getQuestionVariable(i).c_str(), questions->getQuestionUnits(i).c_str(), SIMCONNECT_DATATYPE_FLOAT32, 
					std::stof(questions->getVariableChange(i)));
			}

			//Requesting  FSX event.
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");

			while (quit == 0)
			{
			skip_loop: if (Connection::started_flag == 1)
				{
					for(z = 1; z <= questions->getNumberofElements(); z++)
					{

						//ask question and save answer to a answer buffer
						//Ask user to press one to ask a questions
						std::cout << "\nPress 1 to ask a questions: ";
						std::cin >> input;	//read user input

						//When user enters 1 (input == 1)
						if (input == 1)
						{
							//display text of questions being asked
							std::cout << questions->getQuestionText(z).c_str() << std::endl;
							//start timer
							start_time_buffer = Connection::startClock();
							//write text of questions being ask onto file

							//clear stream and wait for input
							std::cin.clear();
							std::cin.ignore(100, '\n');
							//take in input and save into variable
							std::cin >> user_input_answer;
							//stop timer and return it
							time_buffer = Connection::stopClock(start_time_buffer);

							//write user input to file
							//write time_buffer to file

						}
						//when user inputs anything besides a 1 (this MAY cause problems when expecting answer as string)
						else if ((std::cin.fail()) || (input != 1))
						{
							//reset for counter to previews number
							z = z - 1;
							//Notify it was incorrect entry and clear cin buffer
							std::cout << "\nInvalid input. Try again!\n" << std::endl;
							std::cin.clear();
							std::cin.ignore(100, '\n');	//needed?
						}

						//Process the next SimConnect messaged recieved thorugh the spcified callback function MyDispatchProc
						SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);

						//Calculate answer different
						result_buffer = abs(user_input_answer - Connection::getAnswer());
						
						if (result_buffer > std::stof(questions->getVariableChange(z)))
						{
							//incorrect answer
							//count how many incorrect answer were provided
							//if this is the second incorrect answer
						}
						std::cout << result_buffer << std::endl;
						std::cout << time_buffer << std::endl;

						//Check to see if sim flight is still active
						if (started_flag != 1)
						{	
							//If it is not active exit out of this loop and reset the questioning session
							goto skip_loop;
						}

					}
					//You have asked all of the questions. Let's exit the loop
					std::cout << "\nNo more questions in the database!\n" << std::endl;
					quit = 1;
					SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);
				}
				else
				{
					SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);
				}
			}

			//FSX has quit,or ran out of questiosn in the database. disconnect.
			Connection::Disconnect();
		}
		else
		{
			printf("\nUnable to connect to Flight Simulator X\n");
		}
	}

	Connection::Pause();
}

//Disconnect from SimConnect Server
void Connection::Disconnect()
{
	HRESULT hr;

	if (Connection::hSimConnect != NULL)
	{
		//Attempt to close connection by calling SimConnect_Close()
		if (SUCCEEDED(hr = SimConnect_Close(Connection::hSimConnect)))
		{
			Connection::hSimConnect = NULL;
			printf("Connection with Flight Simulator X has been terminated \n");
		}
		else
		{
			//Unsuccesfull closing connection.
			printf("Error has occured during termination of connection \n");
		}
	}
}

//handels each event ID, all communications with SimConnect Server
void CALLBACK Connection::MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext)
{
	//Simconnect result handle
	HRESULT hr;

	//read each event and determine what to do with it
	switch (pData->dwID)
	{
		//An event was recieved. Check to see if it is relevent
		case SIMCONNECT_RECV_ID_EVENT:
		{
			//Event poitner
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

			//Check which even it was and do something with it.
			switch (evt->uEventID)
			{
				case EVENT_SIM_START:
				{
					// Now the sim is running, request information on the user aircraft
					//Calls for data everysecond but only if the data has changed and only the data that changed.
					hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND,
						SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
					started_flag = 1;

					break;
				}
				case EVENT_SIM_STOP:
				{
					//Sim stopped running. Restart the questions.
					started_flag = 0;
					z = 1;
					break;
				}

				default:
				break;
			}
		break;
		}

		//An sim ojbect was recieved. Check and see what to do with it.
		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			//Simobject pointer
			SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

			switch (pObjData->dwRequestID)
			{
			//Object requested by client
				case REQUEST:
				{
					Answers *pS = (Answers *)&pObjData->dwData;

					Connection::fsx_answer_buffer = pS->answer[z - 1].answer_value;

					break;
				}

				default:
					break;
			}
			break;
		}

		//Aiport return list
		case SIMCONNECT_RECV_ID_AIRPORT_LIST:
		{
			SIMCONNECT_RECV_AIRPORT_LIST * airport_list_ptr = (SIMCONNECT_RECV_AIRPORT_LIST *)pData;
			printf("  Num of airports: %d Test: %c %d\n",
				airport_list_ptr->dwArraySize, airport_list_ptr->rgData[0], airport_list_ptr->rgData[1]);

			
			for (int aa = 0; aa < (int)airport_list_ptr->dwArraySize; aa++)
			{
				std::cout << airport_list_ptr->rgData[aa].Icao << " " ;
				std::cout << airport_list_ptr->rgData[aa].Longitude << " ";
				std::cout << airport_list_ptr->rgData[aa].Latitude << " ";
				std::cout << airport_list_ptr->rgData[aa].Altitude << std::endl;
			}

			break;
		}
		//Deals with quitting FSX
		case SIMCONNECT_RECV_ID_QUIT:
			Connection::quit = 1;
			printf("\nFSX has been terminated.\n");
		break;
		
		//Identify that FSX is open
		case SIMCONNECT_RECV_ID_OPEN:
			Connection::quit = 0;
			std::cout << "\nFSX is currently open.\n";
		break;

		//Ignore any even that the client does not care for
		default:
			printf("\nIgnored Event ID. Received: %d\n", pData->dwID);
		break;
	}
}

//After ending the program pause it and only quit when user presses a key.
void Connection::Pause()
{
	std::cin.sync();	//Flush the input buffer
	std::cin.ignore();	//Dont store any input by the user.
}

//Start the clock
float Connection::startClock()
{
	float start_time = clock();
	return start_time;
}

//(Wow wow wow) Stop the clock
float Connection::stopClock(float start_time)
{
	return (((float)clock() - start_time)/CLOCKS_PER_SEC);
}

float Connection::getAnswer()
{
	return Connection::fsx_answer_buffer;
}