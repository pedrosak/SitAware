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
	const int TEXT_COLUMN_POSITION = 2;
	const int WAV_COLUMN_POSITION = 3;
	const int VARIABLE_COLUMN_POSITION = 4;
	const int UNITS_COLUMN_POSITION = 5;

	int row_count;

	//command string
	char * cmdString = new char[];
	SAString cmdString_;

	int Database::Connect();
	int Database::Disconnect();

public:

	Database::Database();
	Database::~Database();
	int Database::Query(const char columns[], char table[], char condition[], Questions *questions);
	int Database::getRowNumber();
};
