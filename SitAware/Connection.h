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
};