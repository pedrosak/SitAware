#include "stdafx.h"


//Constructor
Questions::Questions()
{

}

//Store the value of text for each questions in database
void Questions::storeQuestionText(int number, std::string value)
{
	//Using the map variable to store the values of text for each questions
	question_text.insert(std::pair<int, std::string>(number, value));
}

//Store the value of the directory for the wav file for each questions in database
void Questions::storeQuestionWav(int number, std::string value)
{
	//Using the map variable to store the values of directory that holds the wave file for each questions
	question_wav.insert(std::pair<int, std::string>(number, value));
}

//Store the value of simconnect variable for each questions in database
void Questions::storeQuestionVariable(int number, std::string value)
{
	//Using the map variable to store the values of simconnect variable for each questions
	question_variable.insert(std::pair<int, std::string>(number, value));
}

//Store the value of variable unit for each questions in database
void Questions::storeQuestionUnits(int number, std::string value)
{
	//Using the map variable to store the values of variable unit for each questions
	question_units.insert(std::pair<int, std::string>(number, value));
}

//Returns the value of text for specific question number
std::string Questions::getQuestionText(int number)
{
	return question_text[number];
}

//Return the value of the wav directory for specific question
std::string Questions::getQuestionWav(int number)
{
	return question_wav[number];
}

//Returns the value of the simconnect variable for a specific question
std::string Questions::getQuestionVariable(int number)
{
	return question_variable[number];
}

//Returns the value of the variable unit for a specific question
std::string Questions::getQuestionUnits(int number)
{
	return question_units[number];
}