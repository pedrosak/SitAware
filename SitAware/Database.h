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

	//command string
	SADataType_t cmdString;
	unsigned int question_number;
	std::string question_text;
	std::string question_wave;
	std::string question_variable;

public:
	Database::Database();
	int Database::Connect();
	int Database::Disconnect();
	int Database::Query(const std::string columns[], std::string& table, std::string& condition);
};