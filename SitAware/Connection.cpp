#include "stdafx.h"

unsigned int Connection::quit = 0;

HANDLE Connection::hSimConnect = NULL;

double Connection::fsx_result = 0;

int Connection::started_flag = 0;

int z;

//Data definition for requests sent to SImConnect
static enum DATA_REQUEST_ID
{
	REQUEST,
};
struct Answer
 {
	double answer_;
	double answer_test;
};

//Data definition of client EVENTS with the SimConnect server
static enum DATA_DEFINE_ID
{
	DEFINITION,
};

static enum EVENT_ID
{
	EVENT_SIM_START,
	EVENT_TIMER
};


//Constructor. Defines HANDLE variable
Connection::Connection()
{

}

//Initiate connectino with SimConnect
void Connection::Connect(Questions *questions, Interrogator *interrogator)
{
	HRESULT hr;

	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (Connection::hSimConnect == NULL)
	{
		printf("\nAttempting to connect to Flight Simulator X\n");

		hr = SimConnect_Open(&Connection::hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0);
		//Call SimConnect function to open a new connection.
		if (hr == S_OK)
		{
			//SimConnect_Open returned S_OK
			printf("\nConnection established with Flight Simulator X\n");
			 
			//data definition set up. This returnst the airplane name being used.
			//hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);

			//data definition set up. This sets the FSX simulation variable to a client defined object definition (in this case enum DEFINITION is the client defined data definition)
			for (int i = 1; i <= questions->getNumberofElements(); i++)
			{
				hr = SimConnect_AddToDataDefinition(Connection::hSimConnect, DEFINITION, questions->getQuestionVariable(i).c_str(), questions->getQuestionUnits(i).c_str());
			}

			//Requesting an FSX event. This even is to see if the simulation has started.
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_TIMER, "1sec");

			//Requests the data from the FSX server. The last argument dictates how ofter the data is requested.
			//SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);

				while (quit == 0)
				{
					if (Connection::started_flag == 1)
					{
						//for(z = 1; z <= questions->getNumberofElements(); z++)
						//{

						//	//ask question and save answer to a answer buffer
						//	answer_buffer = interrogator->askQuestion(questions);

						//	//Process the next SimConnect messaged recieved thorugh the spcified callback function MyDispatchProc
						//	SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);

						//	//calculate the difference between user answer and FSX answer
						//	fsx_answer_buffer = Connection::getAnswer();

						//	result_buffer = abs(fsx_answer_buffer - std::get<0>(answer_buffer));

						//	printf("\nFSX answer: %f. User answer: %f. Differance is %f. Response time: %f\n", fsx_answer_buffer,std::get<0>(answer_buffer), 
						//		result_buffer, std::get<1>(answer_buffer));

						//	//Give time for other process to execute.
						//	Sleep(1);
						//}
						//quit = 1;
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

					// Now the sim is running, request information on the user aircraft
					hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, 0, SIMCONNECT_PERIOD_ONCE);
					std::cout << "Event Sim Start" << std::endl;
					started_flag = 1;

					break;
				case EVENT_TIMER:
				{
					if (started_flag == 1)
					{
						// Now the sim is running, request information on the user aircraft
						hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, 0, SIMCONNECT_PERIOD_ONCE);
						std::cout << "Event Timer" << std::endl;
					}
				}
				break;

				default:
				break;
			}
		break;
		}

		//An sim ojbect was recieved. Check and see what to do with it.
		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			//Simobject pointer
			SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

			switch (pObjData->dwRequestID)
			{
			//Object requested by client
			case REQUEST:
			{
				//DWORD pointer
				DWORD ObjectID = pObjData->dwObjectID;

				//Answer pointer
				Answer *Answerptr = (Answer*)&pObjData->dwData;

				std::cout << pObjData->dwentrynumber << std::endl;
				//Connection::fsx_result = Answerptr->test_map[0];

			break;
			}

			default:
				break;
			}
			break;
		}

		//Deals with quitting FSX
		case SIMCONNECT_RECV_ID_QUIT:
			Connection::quit = 1;
			printf("\nFSX has been terminated.\n");
		break;

		case SIMCONNECT_RECV_ID_OPEN:
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

double Connection::getAnswer()
{
	
	return Connection::fsx_result;

}