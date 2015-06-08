#include "stdafx.h"

//Constructor.
Interrogator::Interrogator()
{
}


//Alerts the pilot that questions are ready
unsigned int Interrogator::Alert()
{

	//Ask user if he is ready to recieve a questions (alert via tone and print on command line for the moment)
	std::cout << "\a \a \a" <<std::endl;									//audio alert
	std::cout << "Are you ready for a questions (Y or N)? \n";				//command line alert
	std::cin.get(ready_for_questions);

	if (ready_for_questions == 'Y' || ready_for_questions == 'y')
	{
		//Pilot is ready for questions
		return 1;
	}
	else if (ready_for_questions == 'N' || ready_for_questions == 'n')
	{
		//Pilot is not ready for questions
		return 0;
	}
	return 0;
}

//asks the questions. Version 1 will ask via command window. Goal is to do it via speech.
void Interrogator::Interrogate(const char columns[], char table[], char condition[])
{
	//Fetch the questiosn to be asked from database
	Database database;
	std::string variable_FSX = database.Query(columns, table, condition);
	//Ask the questions via text and audio
	//Check the answer by querying the flight simm

	//Check the answer from user via input and voice (voice to be implemented later)
}
