#include "stdafx.h"

unsigned int Connection::quit = 0;

HANDLE Connection::hSimConnect = NULL;

//Data definition for requests sent to SImConnect
static enum DATA_REQUEST_ID
{
	REQUEST,
};

struct Answer
{
	char title[256];
	double answer_value;
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


//Constructor. Defines HANDLE variable
Connection::Connection()
{
}

//Initiate connectino with SimConnect
void Connection::Connect(Questions *questions)
{
	HRESULT hr;

	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (Connection::hSimConnect == NULL)
	{
		printf("Attempting to connect to Flight Simulator X\n");

		hr = SimConnect_Open(&Connection::hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0);
		//Call SimConnect function to open a new connection.
		if (hr == S_OK)
		{
			//SimConnect_Open returned S_OK
			printf("Connection established with Flight Simulator X\n");
			 
			//data definition set up. This returnst the airplane name being used.
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
			
			//data definition set up. This sets the FSX simulation variable to a client defined object definition (in this case enum DEFINITION is the client defined data definition)
			hr = SimConnect_AddToDataDefinition(Connection::hSimConnect, DEFINITION, questions->getQuestionVariable(3).c_str(), questions->getQuestionUnits(3).c_str());

			//Requesting an FSX event. This even is to see if the simulation has started.
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

			//Requests the data from the FSX server. The last argument dictates how ofter the data is requested.
			SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);

			while (quit == 0)
			{
				//Process the next SimConnec messaged recieved thorugh the spcified callback function MyDispatchProc
				SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);


				//Give time for other process to execute.
				Sleep(1);
			}

			//FSX has quit, disconnect.
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
	HRESULT hr;

	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
		{
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

			switch (evt->uEventID)
			{
			case EVENT_SIM_START:

				// Now the sim is running, request information on the user aircraft
				hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, 0, SIMCONNECT_PERIOD_ONCE);

				break;

			default:
				break;
			}
			break;
		}

		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;
			switch (pObjData->dwRequestID)
			{
			case REQUEST:
			{
				DWORD ObjectID = pObjData->dwObjectID;
				Answer *Answerptr = (Answer*)&pObjData->dwData;
				if (SUCCEEDED(StringCbLengthA(&Answerptr->title[0], sizeof(Answerptr->title), NULL))) // security check
				{
					printf("\nObjectID = %d  Title = \"%s\"\nAnswer = %f \n", ObjectID, Answerptr->title, Answerptr->answer_value);
				}
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
			printf("FSX has been terminated.\n");
		break;

		default:
			printf("Ignored Event ID. Received: %d\n", pData->dwID);
		break;
	}
}

void Connection::Pause()
{
	std::cin.sync();	//Flush the input buffer
	std::cin.ignore();	//Dont store any input by the user.
}