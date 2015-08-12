#include "stdafx.h"
#include <bitset>


#define HEIGHT 3	//Height of multi-dim array for questions_average

unsigned int Connection::quit;

HANDLE Connection::hSimConnect = NULL;

int Connection::started_flag = 0;

float Connection::fsx_answer_buffer = 0;

int z;

//Data definition for requests sent to SImConnect
static enum DATA_REQUEST_ID
{
	REQUEST,
	REQUEST_AIRPORT
};

//Struct for a single item returned by FSX
struct SingleAnswer
{
	int answer_id;
	float answer_value;
};

//Struct for tagged received data
struct Answers
{
	//Memory space for 1000 answers
	SingleAnswer answer[1000];
};

struct Airport
{
	char Icao_near[9];
	double Lat_near;
	double Long_near;
	double Alt_near;
};

//Data definition of client EVENTS with the SimConnect server
static enum DATA_DEFINE_ID
{
	DEFINITION,
};

static enum EVENT_ID
{
	EVENT_SIM_START,
	EVENT_SIM_STOP,
	EVENT_TIMER
};


//Constructor. Defines HANDLE variable
Connection::Connection()
{
	//Future versions use constructor and destructor to connect and disconnect to FSX
}

//Initiate connectino with SimConnect
void Connection::Connect(Questions *questions, std::string file_name)
{
	HRESULT hr;

	//Start a new connection when hSimConnect is still NULL, meaning, no messages from server (SimConnect) have been recieved
	if (Connection::hSimConnect == NULL)
	{
		printf("\nAttempting to connect to Flight Simulator X\n");

		//Call SimConnect function to open a new connection.
		hr = SimConnect_Open(&Connection::hSimConnect, "Situation Awareness Questionare", NULL, 0, NULL, 0);

		if (hr == S_OK)
		{
			//SimConnect_Open returned S_OK
			printf("\nConnection established with Flight Simulator X\n");
			 
			//data definition set up. This sets the FSX simulation variable to a client defined object definition (in this case enum DEFINITION is the client defined data definition)
			for (int i = 1; i <= questions->getNumberofElements(); i++)
			{

				hr = SimConnect_AddToDataDefinition(Connection::hSimConnect, DEFINITION, questions->getQuestionVariable(i).c_str(), questions->getQuestionUnits(i).c_str(), SIMCONNECT_DATATYPE_FLOAT32);
			}

			//Requesting  FSX event.
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");

			//create three arrays to hold question number, question average response time, and question count
			int *question_number_array = new int[questions->getNumberofElements()];
			double *question_average_array = new double[questions->getNumberofElements()];
			double *question_time_sum_array = new double[questions->getNumberofElements()];
			int *question_count_array = new int[questions->getNumberofElements()];

			//Populate question number in the question number array
			for (int count = 0; count < questions->getNumberofElements(); count++)
			{
				question_number_array[count] = count + 1;	//add question numbers
				question_average_array[count] = 0;	//fill array with zero
				question_count_array[count] = 0;	//fill array with zero
				question_time_sum_array[count] = 0; //fill array with zero
				//std::cout << question_count_array[count] << " ";
			}

			while (quit == 0)
			{
				skip_loop: if (Connection::started_flag == 1)
				{
					//ask question and save answer to a answer buffer
					//Ask user to press one to ask a questions
					std::cout << "\nPress 1 to ask a questions or 'Q' to quit: ";
					std::getline(std::cin, input_string);	//read user input

					//When user enters 1 (input == 1)
					if (input_string == "1")
					{
						old_time_average = new_time_average;
						//Counter to count how many questios have been asked
						number_questions_asked += 1;

						//Write time stamp to file and questions number
						std::ofstream ofs;
						if (!ofs.is_open())
						{
							ofs.open(file_name, std::ofstream::app);
						}

						//Pick a random questions from database
						z = rand() % questions->getNumberofElements() + 1;


						//Playing sound has not worked on C++ due to passing of string via questions pointer. Much easier to do this on C#
						//play wav of question in databaseth;
						//PlaySound(TEXT("C:\\Users\\Elite\\Desktop\\test.wav"), NULL, SND_ASYNC | SND_FILENAME);
						//PlaySound((LPCWSTR)questions->getQuestionWav(z).c_str(), NULL, SND_ASYNC | SND_FILENAME);
				
						//display text of questions being asked
						invalid_input_loop: std::cout << questions->getQuestionText(z).c_str() << std::endl;
						//start timer
						start_time_buffer = Connection::startClock();

						//write text of questions being ask onto file
						ofs << Connection::localTime();
						ofs << "Questions number: " << number_questions_asked << ". " << questions->getQuestionText(z).c_str() << std::endl;

						//take in input and save into variable
						std::getline(std::cin, input_string);	//read user input
						if (std::cin.fail())
						{
							std::cout << "Invalid input. Try again.";
							std::cin.clear();
							std::cin.ignore(256, '\n');
							goto invalid_input_loop;
						}
						//stop timer and return it
						time_buffer = Connection::stopClock(start_time_buffer);

						//write user input to file
						ofs << "User input: " << input_string;
						//write time_buffer to file
						ofs << " Answered in " << time_buffer << " seconds." << std::endl;

						//Process the next SimConnect messaged recieved thorugh the spcified callback function MyDispatchProc
						SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);

						//Check if questions is asking for heading. If it is then FSX result must be converted from radiants to degrees
						if (questions->getQuestionText(z).find("heading") != std::string::npos)
						{
							//Convert heading from fsx radians to degrees
							fsx_calculated_answer = Connection::getAnswer() * 57.2957795;
						}
						else
						{
							//No need to convert fsx results. Pass it to the variable.
							fsx_calculated_answer = Connection::getAnswer();
						}

						//Calculate answer different
						result_buffer = abs(std::stof(input_string) - fsx_calculated_answer);

						//sum all the response times
						time_buffer_sum = time_buffer_sum + time_buffer;

						//get the time_buffer average
						new_time_average = time_buffer_sum / number_questions_asked;

						//std::cout << new_time_average << " new. " << old_time_average << " old." << std::endl;

						//store and calculate individual question average and count
						for (int count = 0; count < questions->getNumberofElements(); count++)
						{
							if (question_number_array[count] == z)
							{
								//save current question average
								current_question_average_buffer = question_average_array[count];
								//increase count by 1
								question_count_array[count] += 1;
								//Sum time to respond to this question
								question_time_sum_array[count] += time_buffer;
								//calculate average and store it to average array
								question_average_array[count] = question_time_sum_array[count] / question_count_array[count];

								//Save location of current question in arrays for calculations
								int location_of_current_question_in_arrays = count;

								if ((number_questions_asked > 1) && (question_average_array[location_of_current_question_in_arrays] > (2 * current_question_average_buffer)) && (current_question_average_buffer != 0))
								{
									current_question_increase_multiplier =question_average_array[location_of_current_question_in_arrays] / current_question_average_buffer;
									ofs << "Average response time  for question number " << location_of_current_question_in_arrays << " increased by a factor of " << current_question_increase_multiplier << ". The previews average was " << current_question_average_buffer << " and the new average is " << question_average_array[location_of_current_question_in_arrays] << "." << std::endl;
									incorrect_count_time += 1;
									if (incorrect_count_time >= 2)
									{
										//Average response time increase twice. Make suggestion
										//Make a suggestion
										std::cout << "\n** SUGGESTION 1 - Decrease automation**" << std::endl;
										ofs << "**Suggestion was made.**" << std::endl;
										//increase suggestions count
										suggestion_count += 1;
									}
								}
								else if ((number_questions_asked > 1) && (question_average_array[location_of_current_question_in_arrays] > (current_question_average_buffer)))
								{
									improvement_count += 1;
									if ((improvement_count > 2) && suggestion_count >= 1)
									{
										//Improvement is being shown
										ofs << "Improving answer reaction time. Suggest to increase automation." << std::endl;
										std::cout << "\n** SUGGESTION 2 - Increase automation **" << std::endl;
									}
								}
							}
						}


						//Check to see if the current average and the new average has doubled
						if ((number_questions_asked > 1) && (new_time_average > (2 * old_time_average)) && (old_time_average != 0))
						{
							average_increase_multiplier = (new_time_average / old_time_average);
							ofs << "Average response time  increase by a factor of " <<  average_increase_multiplier << ". The previews average was " << old_time_average << " and the new average is " << new_time_average << "." << std::endl;
							incorrect_count_time += 1;

							if (incorrect_count_time >=2)
							{
								//Average response time increase twice. Make suggestion
								//Make a suggestion
								std::cout << "\n** SUGGESTION 1 - Decrease automation**" << std::endl;
								ofs << "**Suggestion was made.**" << std::endl;
								//increase suggestions count
								suggestion_count += 1;
							}
						}
						else if ((number_questions_asked > 1) && (new_time_average < (old_time_average)))
						{
							improvement_count += 1;
							if ((improvement_count > 2) && suggestion_count >= 1)
							{
								//Improvement is being shown
								ofs << "Improving answer reaction time. Suggest to increase automation." << std::endl;
								std::cout << "\n** SUGGESTION 2 - Increase automation **" << std::endl;
							}
						}


						if (result_buffer > std::stof(questions->getVariableChange(z)))
						{
							//incorrect answer
							ofs << "Incorrect answer number " << incorrect_count_error + 1 << ". FSX result was " << fsx_calculated_answer << ". Difference in answers " << result_buffer << std::endl;
							std::cout << "\nIncorrect Answer." << std::endl;

							//count how many incorrect answer were provided
							incorrect_count_error = incorrect_count_error + 1;

							//if this is the second incorrect answer
							if (incorrect_count_error >= 2)
							{
								//Make a suggestion
								std::cout << "\n** SUGGESTION 1 - Decrease automation**" << std::endl;
								ofs << "**Suggestion was made.**" << std::endl;
								//increase suggestions count
								suggestion_count += 1;
								//reset incorrect_count back to 0
								incorrect_count_error = 0;
							}
						}
						else
						{
							ofs << "Correct answer. FSX result was " << fsx_calculated_answer << ". Difference in answers was " << result_buffer << std::endl;
						}

						ofs << "\n\n";
						ofs.close();
					}
					//when user inputs anything besides a 1 (this MAY cause problems when expecting answer as string)
					else if (input_string == "Q" || input_string == "q")
					{
						quit = 1;
					}
					else
					{
						//Notify it was incorrect entry and clear cin buffer
						std::cout << "\nInvalid input. Try again!\n" << std::endl;
					}

					//Check to see if sim flight is still active
					if (started_flag != 1)
					{	
						//If it is not active exit out of this loop and reset the questioning session
						goto skip_loop;
					}

					SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);
				}
				else
				{
					SimConnect_CallDispatch(Connection::hSimConnect, MyDispatchProc, NULL);
				}
			}

			//FSX has quit,or ran out of questiosn in the database. disconnect.
			Connection::Disconnect();
		}
		else
		{
			printf("\nUnable to connect to Flight Simulator X\n");
		}
	}

	Connection::Pause();
}

//Disconnect from SimConnect Server
void Connection::Disconnect()
{
	HRESULT hr;

	if (Connection::hSimConnect != NULL)
	{
		//Attempt to close connection by calling SimConnect_Close()
		if (SUCCEEDED(hr = SimConnect_Close(Connection::hSimConnect)))
		{
			Connection::hSimConnect = NULL;
			printf("Connection with Flight Simulator X has been terminated \n");
		}
		else
		{
			//Unsuccesfull closing connection.
			printf("Error has occured during termination of connection \n");
		}
	}
}

//handels each event ID, all communications with SimConnect Server
void CALLBACK Connection::MyDispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void *pContext)
{
	//Simconnect result handle
	HRESULT hr;

	//read each event and determine what to do with it
	switch (pData->dwID)
	{
		//An event was recieved. Check to see if it is relevent
		case SIMCONNECT_RECV_ID_EVENT:
		{
			//Event poitner
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

			//Check which even it was and do something with it.
			switch (evt->uEventID)
			{
				case EVENT_SIM_START:
				{
					// Now the sim is running, request information on the user aircraft
					//Calls for data everysecond but only if the data has changed and only the data that changed.
					hr = SimConnect_RequestDataOnSimObject(Connection::hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND,
						SIMCONNECT_DATA_REQUEST_FLAG_CHANGED | SIMCONNECT_DATA_REQUEST_FLAG_TAGGED);
					started_flag = 1;

					break;
				}
				case EVENT_SIM_STOP:
				{
					//Sim stopped running. Restart the questions.
					started_flag = 0;
					z = 1;
					break;
				}

				default:
				break;
			}
		break;
		}

		//An sim ojbect was recieved. Check and see what to do with it.
		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			//Simobject pointer
			SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA *)pData;

			switch (pObjData->dwRequestID)
			{
			//Object requested by client
				case REQUEST:
				{
					Answers *pS = (Answers *)&pObjData->dwData;

					Connection::fsx_answer_buffer = pS->answer[z - 1].answer_value;

					break;
				}

				default:
					break;
			}
			break;
		}

		//Aiport return list
		//Currently the program can request all aiports around the users airplane. 
		//However it does not yet use that data to determin the proximity of each airport to the users plane. It appears that the first returned airport
		//is the closest airport to the user but this is not confirmed.
		//All airport information is saved in the Airport struct.
		case SIMCONNECT_RECV_ID_AIRPORT_LIST:
		{
			SIMCONNECT_RECV_AIRPORT_LIST * airport_list_ptr = (SIMCONNECT_RECV_AIRPORT_LIST *)pData;
			printf("  Num of airports: %d Test: %c %d\n",
				airport_list_ptr->dwArraySize, airport_list_ptr->rgData[0], airport_list_ptr->rgData[1]);

			
			for (int aa = 0; aa < (int)airport_list_ptr->dwArraySize; aa++)
			{
				std::cout << airport_list_ptr->rgData[aa].Icao << " " ;
				std::cout << airport_list_ptr->rgData[aa].Longitude << " ";
				std::cout << airport_list_ptr->rgData[aa].Latitude << " ";
				std::cout << airport_list_ptr->rgData[aa].Altitude << std::endl;
			}

			break;
		}
		//Deals with quitting FSX
		case SIMCONNECT_RECV_ID_QUIT:
			Connection::quit = 1;
			printf("\nFSX has been terminated.\n");
		break;
		
		//Identify that FSX is open
		case SIMCONNECT_RECV_ID_OPEN:
			Connection::quit = 0;
			std::cout << "\nFSX is currently open.\n";
		break;

		//Ignore any even that the client does not care for
		default:
			printf("\nIgnored Event ID. Received: %d\n", pData->dwID);
		break;
	}
}

//After ending the program pause it and only quit when user presses a key.
void Connection::Pause()
{
	std::cin.sync();	//Flush the input buffer
	std::cin.ignore();	//Dont store any input by the user.
}

//Start the clock
float Connection::startClock()
{
	float start_time = clock();
	return start_time;
}

//(Wow wow wow) Stop the clock
float Connection::stopClock(float start_time)
{
	return (((float)clock() - start_time)/CLOCKS_PER_SEC);
}

float Connection::getAnswer()
{
	return Connection::fsx_answer_buffer;
}

std::string Connection::localTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	return asctime(timeinfo);
}