#ifndef Database_h
#define Database_h
#endif


class Database
{
private:
	//Connection object
	SAConnection con;
	//Command object
	SACommand cmd;

	//Define column position of each value
	static const int TEXT_COLUMN_POSITION = 2;
	static const int WAV_COLUMN_POSITION = 3;
	static const int VARIABLE_COLUMN_POSITION = 4;
	static const int UNITS_COLUMN_POSITION = 5;

	//command string
	char * cmdString = new char[];
	SAString cmdString_;

public:

	Database::Database();
	int Database::Connect();
	int Database::Disconnect();
	int Database::Query(const char columns[], char table[], char condition[], Questions *questions);
};
