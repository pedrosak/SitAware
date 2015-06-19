#pragma once
#ifndef Interrogator_h
#define Interrogator_h
#endif

#include <tuple>

class Interrogator
{
private:

	int input;		//user input for questions (1 to ask a questions)
	double answer_double;	//Answer if its a nubmer integer
	float  start_time_buffer;	//holds time of when question was asked
	float time_buffer; //Holds value of how long it took for user to answer question

	std::tuple<double, float> return_values;	//returns answer and time 

public:

	Interrogator::Interrogator();	//Constructor
	std::tuple<double, float> Interrogator::askQuestion(Questions *questions, int i);	//Asks the questions
	clock_t Interrogator::startClock();	//starts a clock timer
	clock_t Interrogator::stopClock(float start_time);	//(wow wow wow) Stop the clock.

};
