#include "stdafx.h"

//Constructor
Interrogator::Interrogator()
{

}

//Ask one question as the user presses key 1. Goes down the list of questions database
std::tuple<double, float> Interrogator::askQuestion(Questions *questions)
{
	////Start while loop to ask questions.
	//while (flag == 0)
	//{
		//Ask user to press one to ask a questions
		std::cout << "\nPress 1 to ask a questions: ";
		std::cin >> input;	//reads user input

		//Check input to see if its balid
		//if ((input == 1) && (i < (database.getRowNumber())))
		if ((input == 1) && (i < 4))
		{
			//Query the database to get the questions 
			std::cout << questions->getQuestionText(i).c_str() << std::endl;

			//start a timer to measure how long it took for the user to answer
			start_time_buffer = Interrogator::startClock();

			//clear stream and wait for input
			std::cin.clear();
			std::cin.ignore(100, '\n');

			//take in input and save it to variable answer_string
			std::cin >> answer_double;
			
			//stop timer and return it
			time_buffer = Interrogator::stopClock(start_time_buffer);

			i++;
		}
		else if ((std::cin.fail()) || (input != 1))
		{
			std::cout << "\nInvalid input. Try again!\n" << std::endl;
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}
		else
		{
			std::cout << "\nNo more questions in the database!\n" << std::endl;
			//SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);
			flag = 1;
		}
	//}
		return_values = std::make_tuple(answer_double, time_buffer);
	return return_values;
}

//Start the clock
clock_t Interrogator::startClock()
{
	clock_t start_time = clock();
	return start_time;
}

//(Wow wow wow) Stop the clock
clock_t Interrogator::stopClock(float start_time)
{
	clock_t stop_time = clock();
	return ((float)stop_time - start_time)/CLOCKS_PER_SEC;
}
