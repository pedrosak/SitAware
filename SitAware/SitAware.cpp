// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Main constructor.
void SitAware()
{

	Connection connection = Connection::Connection();	//Create connection obj
	Database database = Database::Database();			//Create databse obj
	Questions questions = Questions::Questions();		//Create question obj
	Questions *questionPtr;								//pointer of Question type

	//assign question pointer to the question object
	questionPtr = &questions;

	//Connect to database
	database.Connect();
	//Query the database to get the questions data
	database.Query("*", "questions", "", questionPtr);
	//disconnect from database
	database.Disconnect();

	//Connect to the FSX simconnect server
	connection.Connect(questionPtr);

}

void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

