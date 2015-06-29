#pragma once
#ifndef Interrogator_h
#define Interrogator_h
#endif

#include <tuple>

class Interrogator
{
private:

	int input;		//user input for questions (1 to ask a questions)
	long user_input_answer;	//Answer if its a nubmer integer
	long  start_time_buffer;	//holds time of when question was asked
	long time_buffer; //Holds value of how long it took for user to answer question

	std::tuple<long, long> return_values;	//returns answer and time 

	clock_t Interrogator::startClock();	//starts a clock timer
	clock_t Interrogator::stopClock(long start_time);	//(wow wow wow) Stop the clock.
public:

	Interrogator::Interrogator();	//Constructor
	Interrogator::~Interrogator();	//Destructor
	std::tuple<long, long> Interrogator::askQuestion(Questions *questions, int i);	//Asks the questions

};
