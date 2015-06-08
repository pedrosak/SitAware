#include <stdafx.h>


//Constructor. Defines HANDLE variable
Connection::Connection()
{
	hSimConnect = NULL;
}

//Initiate connectino with SimConnect
void Connection::Connect()
{
	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (hSimConnect == NULL)
	{
		printf("Attempting to connect to Flight Simulator X\n");

		//Call SimConnect function to open a new connection.
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0)))
		{
			//SimConnect_Open returned S_OK
			printf("Connection established with Flight Simulator X\n");

			//Process connection to the simmulator
			//call a function to prepare the data and the events needed from the simmulator
			//findout the initial position of the aircraft
		}
	}
}

void Connection::Disconnect()
{
	if (hSimConnect != NULL)
	{
		//Attempt to close connection by calling SimConnect_Close()
		if (SUCCEEDED(hr = SimConnect_Close(hSimConnect)))
		{
			hSimConnect = NULL;
			printf("Connection with Flight Simulator X has been terminated \n");
		}
		else
		{
			//Unsuccesfull closing connection.
			printf("Error has occured during termination of connection \n");
		}
	}
}

/*The functino MyDispatchProc is a fork from the project FollowThisPlane, (c) N0800. 
	I, Kurt Pedrosa, take no credit for the contents of said function. All credit hereby given
	to the creator(s)/owner(s) of FollowThisPlane project.
*/
// SimConnect sends a message to this client, dispatch it.
void CALLBACK Connection::MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_EXCEPTION:
		// SimConnect cannot process a previous request
		//Process_Exception(pData);
		break;

	case SIMCONNECT_RECV_ID_QUIT:
		// FSX is stopping
		Connection::Disconnect();
		printf("Flight Simmulator has quit.");
		break;

	case SIMCONNECT_RECV_ID_EVENT:
		// One of the FSX events this client has subscribed to
		printf("Connected to FSX");
		break;

	case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
		// An AI object has been added to or removed from the simulation
		//Process_Object_Added_Removed(pData);
		break;

	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		// An answer to a data request on an object
		//Process_Data(pData);
		break;

	case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
		// an answer to a AI creation request
		//Process_Object_ID_Assignment(pData);
		break;

	case SIMCONNECT_RECV_ID_OPEN:
		// the connection process is complete
		//Connection::Process_Connected();
		break;

	case SIMCONNECT_RECV_ID_EVENT_FILENAME:
	case SIMCONNECT_RECV_ID_EVENT_FRAME:
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
	case SIMCONNECT_RECV_ID_WEATHER_OBSERVATION:
	case SIMCONNECT_RECV_ID_CLOUD_STATE:
	case SIMCONNECT_RECV_ID_RESERVED_KEY:
	case SIMCONNECT_RECV_ID_CUSTOM_ACTION:
	case SIMCONNECT_RECV_ID_SYSTEM_STATE:
	case SIMCONNECT_RECV_ID_CLIENT_DATA:
	default:
		printf("\n Message ignored (Unknown RECV_ID)");
		break;
	}
}