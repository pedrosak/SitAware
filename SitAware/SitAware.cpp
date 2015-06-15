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

	//Connect to the database
	database.Connect();

	//Query the database to get the questions data
	database.Query("*", "questions", "", questionPtr);

	//Disconnect to the database
	database.Disconnect();
	
	//Connect to the FSX simconnect server
	//connection.Connect(questionPtr);

	//Press 1 to ask a questions.
	//This may all going into a class function


	int input;		//user input for questions (1 to ask a questions)
	std::string answer_string = "";		//Answer if it saved to a string
	int answer_int;	//Answer if its a nubmer integer
	int flag = 0;	//to exit the loop
	int i = 1;	//To iterrate thought the question in the database
	
	//Start while loop to ask questions.
	while (flag == 0)
	{
		//Ask user to press one to ask a questions
		std::cout << "Press 1 to ask a questions: ";
		std::cin >> input;	//reads user input

		//Check input to see if its balid
		if((input == 1) && (i < (database.getRowNumber())))
		{
			//Query the database to get the questions 
			std::cout << questions.getQuestionText(i).c_str() << std::endl;


			std::cin.clear();
			std::cin.ignore(100, '\n');

			std::getline(std::cin, answer_string);
			std::cout << answer_string << "Hey there you!" << std::endl;
			
			
			i++;
		}
		else if ( (std::cin.fail()) || (input != 1) )
		{
			std::cout << "Invalid input. Try again!" << std::endl;
			std::cin.clear();
			std::cin.ignore(100 , '\n');
		}
		else
		{
			std::cout << "No more questions in the database!" << std::endl;
			flag = 1;
		}
	}

	//connection.Disconnect();

	system("Pause");

}

void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

