#include "stdafx.h"
#include <array>

Database::Database()
{

}


//Connects to database obj
int Database::Connect()
{
	try
	{
		//Using Microsfot SQL Server requires to setClient outside the connect function
		con.setClient(SA_SQLServer_Client);
		//Connect to question_bank database using microsoft autheticator
		con.Connect("PC4292\\SQLEXPRESS@question_bank", "", "");
		//When connected notify the user
		std::cout << "We are connected!\n" << std::endl;
	}
	catch (SAException &err)
	{
		try
		{
			//Cancels any changes made during the current transaction and ends the transaction.
			//This may not be required because the program is not making changed to db
			con.Rollback();
		}
		catch (SAException &)
		{
			//Rollback() may throw an exception if there is a network error
		}
		std::cout << (const char*)err.ErrText() << std::endl;
	}
	return 0;
}

//Disconnect from database obj
int Database::Disconnect()
{
	try
	{
		//Disconnect from database
		con.Disconnect();
		//Notify the user that disconnection was established
		std::cout << "Disconnected from database\n" << std::endl;
	}
	catch (SAException &err)
	{
		//Error caused by Disconnect()
		std::cout << (const char*)err.ErrText() << std::endl;
	}

	return 0;
}

//Query current database obj
int Database::Query(const std::string columns[], std::string table, std::string condition)
//int Database::Query()
{
	
	//Flag to distinguish between conditon or no condition
	unsigned int no_condition_flag = 0;
	

	//Command string will always select from database therfore initial value is SELECT

	cmdString = "SELECT";
	//std::strcpy(cmdString, "SELECT");

	
	//Get the size of the columns[] array
	int size_of_columns = sizeof(columns) / sizeof(std::string);
	printf("%s", cmdString);
	//Each element of columns array will be added to the command string. ex: SELECT column1, column2...
	for (int i = 0; i <= size_of_columns; i++)
	{
		//if it is the last element of the array (or only column) then do not add the comma
		if (i == size_of_columns)
		{
			//strcpy(cmdString, " ");
			//strcpy(cmdString, columns[i].c_str());
			//cmdString = cmdString + *" ";// +columns[i];
			strcat(cmdString, " ");

		}
		//add the comma until you get to the last element of array
		else
		{
			//strcpy(cmdString, " ");
			//strcpy(cmdString, columns[i].c_str());
			//strcpy(cmdString, ",");
			//cmdString = cmdString + " " + columns[i] + ",";
		}
	}
	printf("%s", cmdString);
	/*
	//Decided which command statement do produced depeding if the function was passed with a condition or not
	if (condition == "")
	{
		//Create string based on the parameters passed without any condition
		strcpy(cmdString, " FROM");
		//strcpy(cmdString, table.c_str);
		//cmdString = cmdString + " FROM " + table;
		no_condition_flag = 1;
	}
	else
	{
		//Create string based on the parameters passed with condition
		strcpy(cmdString, " FROM");
		//strcpy(cmdString, table.c_str);
		strcpy(cmdString, " WHERE");
		//strcpy(cmdString, condition.c_str);
		//cmdString = cmdString + " FROM " + table + " WHERE " + condition;
	}

	//Associate the command with the connection obj
	cmd.setConnection(&con);

	//creat command text
	cmd.setCommandText(*cmdString_);


	//Try/catch for execution of the command
	try
	{
		//Executes command string
		cmd.Execute();

		//fetch the data from the execute()
		while (cmd.FetchNext())
		{

		}
	}
	catch (SAException &err)
	{
		try
		{
			//Rollback() can throw execption if there is a network error
			con.Rollback();
		}
		catch (SAException &)
		{
		}

		// print error message
		printf("%s\n", (const char*)err.ErrText());
	}
	*/

	return 0;
}
