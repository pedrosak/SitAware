// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Main constructor.
void SitAware()
{
	Connection connection = Connection::Connection();
	Interrogator interrogator = Interrogator::Interrogator();


	connection.Connect();
	interrogator.Alert();

	system("Pause");

	//sIMcONNECT_rEQUESTFACILITIESLIST  to get list of airports
}

void _tmain(int argc, _TCHAR* argv[])
{
	//std::string questions[] = { "This is a questions.\n" };

	//std::string my_string = "Kurt";
	//
	//printf("%s\n", my_string.c_str());

	//std::cout << questions[0].c_str();

	//system("Pause");

	SitAware();
}

