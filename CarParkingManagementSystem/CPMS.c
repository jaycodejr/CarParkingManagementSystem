#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>
#include <time.h>

//Global Constants
#define VEHICLE_LIST_SIZE 100000
#define TRANSACTION_LIST_SIZE 100000
#define FLOOR_LIST_SIZE 5
#define SLOT_LIST_SIZE 50

typedef struct {
	int floor_number;
	int slot_number;
	bool is_taken;
} Slot;

typedef struct {
	char vehicle_number[10];
	char vehicle_type;
} Vehicle;

typedef struct {
	int transaction_id;
	time_t start_time;
	time_t end_time;
	double total_hours;
	double amount_paid;
	Vehicle vehicle;
	Slot slot;
	bool is_paid;
}Transaction;

//Global Variables
Slot slot_list[SLOT_LIST_SIZE];
Vehicle vehicle_list[VEHICLE_LIST_SIZE];
Transaction transaction_list[TRANSACTION_LIST_SIZE];

int transaction_count = 0;

//Function Prototypes
void show_menu();
void generate_slot();
void view_slot_list();
void add_transaction();
void clear_transaction();
int check_slot_list();
Slot get_available_slot();
void update_slot(Slot slot);
Transaction get_pending_transaction(char vehicle_number[10]);

void clear_stdin(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void convert_string_to_case(char *str, bool change_to_upper) {

	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}


int main(void) 
{
	show_menu();

	


	return 0;
}



void show_menu()
{
	int menuId;

	do
	{
		printf("\n\n\tWelcome to,");
		printf("\n\n\tCAR PARKING MANAGEMENT SYSTEM");
		printf("\n\t-----------------------------");
		printf("\n\t1. Generate Slot");
		printf("\n\t2. Show Slots");
		printf("\n\t3. Add Transaction");
		printf("\n\t4. Clear Transaction");
		printf("\n\n\tPlease select your choice: ");

		if(scanf("%d", &menuId) < 1)
		{ 
			printf("\n\t===> Sorry, invalid choice! <===");
			menuId = 0;
		}
		else 
		{
			switch (menuId)
			{
				case 1:
					generate_slot();
					break;
				case 2:
					view_slot_list();
					break;
				case 3:
					add_transaction();
					break;
				case 4:
					clear_transaction();
					break;

				default:
					printf("\n\t*************************");
					printf("\n\tThank you. See you soon!");
					printf("\n\t*************************\n\n");
					exit(-1);
					break;
			}
		}	

		clear_stdin();

	} while (menuId < 1 || menuId > 5);
}


void add_transaction()
{
	if(!check_slot_list())
	{
		printf("\n\t===> Please generate slots before adding transaction <===");
	}
	else 
	{
		Slot get_slot = get_available_slot();

		if(get_slot.floor_number == 0)
		{
			printf("\n\t===> All slots are taken. Please try again next time! <===");			
		}
		else 
		{
			printf("\n\tADD TRANSACTION\n\t****************");
			

			bool is_data_valid = false;

			Transaction new_transaction;
			
			printf("\n\tEnter vehicle number: ");
			if (!scanf("%s", new_transaction.vehicle.vehicle_number))
			{
				printf("\n\t===> Please enter valid vehicle number <===");
			}
			else
			{
				clear_stdin();

				printf("\tEnter vehicle type (C = Car and V = Van): ");

				if (!scanf("%c", &new_transaction.vehicle.vehicle_type))
				{
					printf("\n\t===> Please enter valid vehicle type <===");
				}
				else
				{
					convert_string_to_case(&new_transaction.vehicle.vehicle_number, true);
					convert_string_to_case(&new_transaction.vehicle.vehicle_type, true);

					if (!(new_transaction.vehicle.vehicle_type == 'C' || new_transaction.vehicle.vehicle_type == 'V'))
					{
						printf("\n\t===> Sorry, invalid vehicle type <===");
					}
					else
					{
						get_slot.is_taken = !get_slot.is_taken;
						new_transaction.transaction_id = time(NULL);
						new_transaction.slot = get_slot;
						new_transaction.start_time = time(NULL);
						new_transaction.is_paid = false;
						

						is_data_valid = true;
					}
				}
			}

			if (is_data_valid) 
			{
				transaction_list[transaction_count] = new_transaction;
				update_slot(get_slot);
				transaction_count++;
				printf("\n\t===> Transaction added successfully! <===");
			}
		}
	}

	show_menu();
}

void clear_transaction() 
{
	printf("\n\tCLEAR TRANSACTION\n\t****************");

	Transaction get_transaction;
	char vehicle_number[10];

	clear_stdin();

	printf("\n\tEnter vehicle number to search: ");

	if (!scanf("%s", vehicle_number))
	{
		printf("\n\t===> Please enter valid vehicle number <===");
	}
	else 
	{
		Transaction trans_data = get_pending_transaction(vehicle_number);

		if(trans_data.transaction_id == 0)
		{
			printf("\n\t===> Sorry, no transaction found! <===");
		}
		else
		{
			struct tm* chekin_time = localtime(&trans_data.start_time);
			time_t c_out = time(NULL);
			struct tm* chekout_time = localtime(&c_out);

			

			printf("\n\tTRANSACTION DETAILS\n\t===================\n");
			printf("\n\tVehicle Details\n\t--------------");
			printf("\n\tPlate Number: %s",trans_data.vehicle.vehicle_number);
			printf("\n\tType: %s",(trans_data.vehicle.vehicle_type == 'C' ? "Car": "Van"));
			printf("\n\n\tSlot Details\n\t--------------");
			printf("\n\tFloor Number: %d", trans_data.slot.floor_number);
			printf("\n\tSlot Number: %d", trans_data.slot.slot_number);
			printf("\n\n\tDuration Details\n\t--------------");
			printf("\n\tCheck-In Time: %02d-%02d-%02d %02d:%02d:%02d", chekin_time->tm_year+1900, chekin_time->tm_mon+1, chekin_time->tm_mday,chekin_time->tm_hour,chekin_time->tm_min,chekin_time->tm_sec);
			printf("\n\tCheck-out Time: %02d-%02d-%02d %02d:%02d:%02d", chekout_time->tm_year+1900, chekout_time->tm_mon+1, chekout_time->tm_mday, chekout_time->tm_hour, chekout_time->tm_min, chekout_time->tm_sec);
			printf("\n\n\t====================\n");
		}
	}

	show_menu();
}

void generate_slot() 
{

	if(check_slot_list())
	{
		printf("\n\t===> You have already generated slots! <===");
	}
	else 
	{
		int slot_count = 0;
		for (int i = 0; i < FLOOR_LIST_SIZE; i++)
		{
			for (int j = 0; j < (SLOT_LIST_SIZE/FLOOR_LIST_SIZE); j++)
			{
				Slot sl = { (i + 1),(slot_count + 1) };
				slot_list[slot_count] = sl;
				slot_count++;
			}
		}
		printf("\n\t===> Slots generated successfully! <===");		
	}
	show_menu();
}

void view_slot_list()
{
	if (!check_slot_list())
	{
		printf("\n\t===> No slots have been generated! <===");
	}
	else
	{
		printf("\n\t-------------------------");
		printf("\n\tSLOT\t | \tSTATUS");
		printf("\n\t-------------------------");
		for (int i = 0; i < SLOT_LIST_SIZE; i++)
		{
			printf("\n\t(%d-%d)\t | \t%s", slot_list[i].floor_number, slot_list[i].slot_number, (slot_list[i].is_taken ? "Taken" : "Available"));
		}		
	}
	show_menu();
}

int check_slot_list()
{
	int slots_generated = 0;

	for (int i = 0; i < SLOT_LIST_SIZE; i++)
	{
		if (slot_list[i].floor_number > 0)
		{
			slots_generated = 1;
			break;
		}
	}

	return slots_generated;
}

Slot get_available_slot()
{
	Slot available_slot = { 0 ,0 };

	for (int i = 0; i < SLOT_LIST_SIZE; i++)
	{
		if (!slot_list[i].is_taken)
		{
			available_slot = slot_list[i];
			break;
		}
	}

	return available_slot;
}

void update_slot(Slot slot) 
{
	bool is_updated = false;

	for (int i = 0; i < SLOT_LIST_SIZE; i++)
	{
		if (slot_list[i].floor_number == slot.floor_number && slot_list[i].slot_number == slot.slot_number)
		{
			slot_list[i].is_taken = slot.is_taken;
			is_updated = true;
			break;
		}
	}

	return is_updated;
}

Transaction get_pending_transaction(char vehicle_number[10])
{
	Transaction result = {0,NULL,NULL,0,0,NULL,NULL,false};

	convert_string_to_case(vehicle_number, true);

	for (int i = 0; i < TRANSACTION_LIST_SIZE; i++)
	{
		int is_found = strcmp(transaction_list[i].vehicle.vehicle_number,vehicle_number);

		if (strcmp(transaction_list[i].vehicle.vehicle_number, vehicle_number) == 0)
		{
			result = transaction_list[i];
			break;
		}
	}

	return result;
}