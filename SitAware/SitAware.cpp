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

	Interrogator interrogator = Interrogator::Interrogator();
	Interrogator *interrogatorPtr;


	//assign question pointer to the question object
	questionPtr = &questions;
	interrogatorPtr = &interrogator;

	//Connect to the database
	database.Connect();

	//Query the database to get the questions data
	database.Query("*", "questions", "", questionPtr);

	//Disconnect to the database
	database.Disconnect();
	
	//Connect to the FSX simconnect server
	connection.Connect(questionPtr, interrogatorPtr);

	//Disconnect from the FSX (not needed because Connect() alwaays disconnects when done.
	//connection.Disconnect();

}

void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

