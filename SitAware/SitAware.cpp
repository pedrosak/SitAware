// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Main constructor.
void SitAware()
{
	//make question then pass ptr to the database and connection objects
	Questions questions = Questions::Questions();

	Questions *questionPtr;

	questionPtr = &questions;

	Connection connection = Connection::Connection();

	Database database = Database::Database();

	database.Connect();

	database.Query("*", "questions", "", questionPtr);

	database.Disconnect();
	
	connection.Connect(questionPtr);

	//system("Pause");
}

void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

