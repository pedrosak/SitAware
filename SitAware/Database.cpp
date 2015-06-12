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
int Database::Query(const char columns[], char table[], char condition[], Questions * questions)
{	
	//Row count variable (hold count of how may rows have been read by FetchNext()
	//Always one because FetchNext must alwas return at least one row of value(s)
	int row_count = 1;

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

			//For loop for testing the query of the data
			////For the size of the returned value
			//for (int j = 1; j <= cmd.FieldCount(); j++)
			//{
			//	//Print each of the results
			//	printf("%s\n", (const char *)cmd.Field(j).asString());
			//}
			
			//Store question text by reading column TEXT_COLUMN_POSITION
			questions->storeQuestionText(row_count, (const char *)cmd.Field(TEXT_COLUMN_POSITION).asString());
			//Store wav directory by reading column WAV_COLUMN_POSITION
			questions->storeQuestionWav(row_count, (const char *)cmd.Field(WAV_COLUMN_POSITION).asString());
			//Store variable by reading column VARIABLE_COLUMN_POSITION
			questions->storeQuestionVariable(row_count, (const char *)cmd.Field(VARIABLE_COLUMN_POSITION).asString());
			//Store variable by reading column UNITS_COLUMN_POSITION
			questions->storeQuestionUnits(row_count, (const char *)cmd.Field(UNITS_COLUMN_POSITION).asString());

			//Increase row count variable 
			row_count++;

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

	return 1;
}
