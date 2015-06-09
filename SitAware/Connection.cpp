#include "stdafx.h"

unsigned int Connection::quit = 0;

HANDLE Connection::hSimConnect = NULL;

//Constructor. Defines HANDLE variable
Connection::Connection()
{
}

//Initiate connectino with SimConnect
void Connection::Connect()
{
	HRESULT hr;

	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (Connection::hSimConnect == NULL)
	{
		printf("Attempting to connect to Flight Simulator X\n");

		//Call SimConnect function to open a new connection.
		if (SUCCEEDED(SimConnect_Open(&Connection::hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0)))
		{
			//SimConnect_Open returned S_OK
			printf("Connection established with Flight Simulator X\n");
			 
			hr = SimConnect_AddToDataDefinition(Connection::hSimConnect, DEFINITION, questions.getQuestionVariable(1).c_str(), questions.getQuestionUnits(1).c_str());


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
	}
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

	hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);

	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
		{
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
			switch (evt->uEventID)
			{
			case EVENT_SIM_START:

				// Now the sim is running, request information on the user aircraft
				hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);

				break;

			default:
				break;
			}
			break;
		}
		//SimConect cannot process a request
		//case SIMCONNECT_RECV_ID_EXCEPTION:
		//{
		//	SIMCONNECT_RECV_EXCEPTION * except = (SIMCONNECT_RECV_EXCEPTION *)pData;
		//	printf("Exception %d on packet %d, argument %d. (%s)", except->dwException, except->dwSendID, except->dwIndex, Connection::Exception_String(except->dwException));
		//}
		//break;

		//Deals with quitting FSX
		case SIMCONNECT_RECV_ID_QUIT:
			Connection::quit = 1;
			printf("FSX has been terminated.\n");
		break;

		default:
			printf("Ignored Event ID\n");
		break;
	}
}

/*This function was created by the owner of the FollowThisPlane project
I, Kurt Pedrosa, take no ownership of fucntion Exception_String(DWORD Recv_Exception).
I hearby give credit to the owner/programmer of FollowThisPlane for the following function
*/
// Returns the description of the exception number
char* Connection::Exception_String(DWORD Recv_Exception)
{
	// Names of Exceptions
	char* EXCEPTION_String[] =
	{
		"NONE", "ERROR", "SIZE_MISMATCH", "UNRECOGNIZED_ID", "UNOPENED", "VERSION_MISMATCH", "TOO_MANY_GROUPS",
		"NAME_UNRECOGNIZED", "TOO_MANY_EVENT_NAMES", "EVENT_ID_DUPLICATE", "TOO_MANY_MAPS",
		"TOO_MANY_OBJECTS", "TOO_MANY_REQUESTS", "WEATHER_INVALID_PORT", "WEATHER_INVALID_METAR",
		"WEATHER_UNABLE_TO_GET_OBSERVATION", "WEATHER_UNABLE_TO_CREATE_STATION",
		"WEATHER_UNABLE_TO_REMOVE_STATION", "INVALID_DATA_TYPE", "INVALID_DATA_SIZE",
		"DATA_ERROR", "INVALID_ARRAY", "CREATE_OBJECT_FAILED", "LOAD_FLIGHTPLAN_FAILED",
		"OPERATION_INVALID_FOR_OBJECT_TYPE", "ILLEGAL_OPERATION", "ALREADY_SUBSCRIBED", "INVALID_ENUM",
		"DEFINITION_ERROR", "DUPLICATE_ID", "DATUM_ID", "OUT_OF_BOUNDS", "ALREADY_CREATED",
		"OBJECT_OUTSIDE_REALITY_BUBBLE", "OBJECT_CONTAINER", "OBJECT_AI", "OBJECT_ATC", "OBJECT_SCHEDULE",
	};
	// Return "unknown" if out of bound
	return Recv_Exception > ARRAYSIZE(EXCEPTION_String) ? "unknown" : EXCEPTION_String[Recv_Exception];
}