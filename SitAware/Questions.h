#pragma once
#ifndef Questions_h
#define Questions_h
#endif 

#include <map>

//Class Questions
class Questions
{
private:

	std::map<int, std::string> Questions::question_text;
	std::map<int, std::string> Questions::question_wav;
	std::map<int, std::string> Questions::question_variable;
	std::map<int, std::string> Questions::question_units;
	std::map<int, std::string> Questions::client_name;
	std::map<int, std::string> Questions::variable_change;

public:

	Questions::Questions();

	void Questions::storeQuestionText(int number, std::string value);
	void Questions::storeQuestionWav(int number, std::string value);
	void Questions::storeQuestionVariable(int number, std::string value);
	void Questions::storeQuestionUnits(int number, std::string value);
	void Questions::storeClientName(int number, std::string value);
	void Questions::storeVariableChange(int number, std::string value);

	std::string Questions::getQuestionText(int number);
	std::string Questions::getQuestionWav(int number);
	std::string Questions::getQuestionVariable(int number);
	std::string Questions::getQuestionUnits(int number);
	std::string Questions::getClientName(int number);
	std::string Questions::getVariableChange(int number);


	int Questions::getNumberofElements();
};

