// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//Main constructor.
void SitAware()
{
	std::string student_id;
	std::string flight_number;
	std::string file_name_input;
	int file_flag = 0;

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

	file_loop: while(file_flag == 0)
	{
		//Ask user for file name
		//clear file_name_input string in case it is not empty.
		file_name_input.clear();
		//User enter student if
		std::cout << "Enter student identification number: ";
		std::getline(std::cin, student_id);	//read user input

		//User enter flight number for that student
		std::cout << "Enter flight number: ";
		std::getline(std::cin, flight_number);


		//Append user inputs for file name
		file_name_input.append(student_id);
		file_name_input.append("-");
		file_name_input.append(flight_number);
		file_name_input.append(".txt");


		//Check if file with that name already exists.
		if (std::ifstream(file_name_input))
		{
			//If exists, notify user.
			std::cout << "File with that name already exists." << std::endl;
		}
		else
		{	
			//If file with that name does not exist then create it.
			std::ofstream file(file_name_input);
			if (file.fail())
			{
				//Notify user if problem occur during creation of file
				std::cout << "File could not be created." << std::endl;
				goto file_loop;	//go back to while loop to redo the file name
			}
			//File was successfully created. Get out of loop
			std::cout << "File was created successfully." << std::endl;
			file_flag = 1;
		}
	}

	//Connect to the FSX simconnect server
	connection.Connect(questionPtr, file_name_input);

}


void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

