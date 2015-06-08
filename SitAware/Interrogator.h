#ifndef Interrogator_h
#define Interrogator_h
#endif


class Interrogator
{
private:
	char ready_for_questions;

public:
	Interrogator::Interrogator();
	unsigned int Interrogator::Alert();			//Alerts the pilot of incomming questions. (in the future it will ask the pilot if he is ready for a questions)
	void Interrogator::Interrogate(const char columns[], char table[], char condition[]);	//Asks the pilot a question
};