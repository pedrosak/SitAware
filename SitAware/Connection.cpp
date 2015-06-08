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

void CALLBACK dispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext)
{
	//case SIMCONNECT_RECV_ID_QUIT:
	//	//User ended FSX end client connection
	//	Connection::Disconnect();
	//	break;

	//	case SIMCONNECT_RECV_ID_EVENT

}