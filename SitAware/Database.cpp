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
		std::cout << "\nWe are connected!\n" << std::endl;
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
		std::cout << "\nDisconnected from database\n" << std::endl;
	}
	catch (SAException &err)
	{
		//Error caused by Disconnect()
		std::cout << (const char*)err.ErrText() << std::endl;
	}

	return 0;
}

//Query current database obj
std::string Database::Query(const char columns[], char table[], char condition[])
{	

	//Command string will always select from database therfore initial value is SELECT
	cmdString_ = "SELECT";

	//Decided which command statement do produced depeding if the function was passed with a condition or not
	if (condition == "")
	{
		//Create string based on the parameters passed without any condition
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), columns);
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), "FROM");
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), table);

	}
	else
	{
		//Create string based on the parameters passed with condition
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), columns);
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), "FROM");
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), table);
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), "WHERE");
		cmdString_.Insert(cmdString_.GetLength(), " ");
		cmdString_.Insert(cmdString_.GetLength(), condition);

	}

	//Associate the command with the connection obj
	cmd.setConnection(&con);

	//creat command text
	cmd.setCommandText(cmdString_);


	//Try/catch for execution of the command
	try
	{
		//Executes command string
		cmd.Execute();

		//fetch the data from the execute()
		while (cmd.FetchNext())
		{
			//For the size of the returned value

			for (int j = 1; j <= cmd.FieldCount(); j++)
			{
				//Print each of the results
				printf("%s\n", (const char *)cmd.Field(j).asString());

				testing_1 = cmd.Field(j).asString();
				std::cout << testing_1.c_str() << std::endl;

			}

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

	return testing_1.c_str();
}
