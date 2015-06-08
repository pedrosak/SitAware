// SitAware.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//Main constructor.
void SitAware()
{

	Database database = Database::Database();
	Questions questions = Questions::Questions();

	char column[] = "*";
	char  table[] = "questions";
	char condition[] = "question_number = 1";

	database.Connect();

	database.Query(column, table, condition);

	database.Disconnect();


	system("Pause");
}

void _tmain(int argc, _TCHAR* argv[])
{
	SitAware();
}

