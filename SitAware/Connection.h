#ifndef Connection_h
#define Connection_h
#endif

class Connection
{
private:
	HANDLE hSimConnect;
	HRESULT hr;

public:
	Connection::Connection();
	void Connection::Connect();				//Initiates connection with SimConnect
	void Connection::Disconnect();			//Disconnects from SimConnect and cleansup
	void CALLBACK Connection::MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
};