// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Main constructor.
void SitAware()
{

	Database database = Database::Database();

	database.Connect();
	std::string column[] = { "question_number", "question_text" };

	database.Query(column, "question_bank", "");

	system("Pause");
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

