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
	char * cmdString = new char[];
	SAString cmdString_;

	unsigned int question_number;
	std::string question_text;
	std::string question_wave;
	std::string question_variable;
	std::string  testing_1;

public:
	Database::Database();
	int Database::Connect();
	int Database::Disconnect();
	std::string Database::Query(const char columns[], char table[], char condition[]);
	//int Database::Query();
};