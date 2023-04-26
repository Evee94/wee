// ReSharper disable CommentTypo
#include <iostream>
#include <iomanip>
#include <cctype>   // character manupulation function
#include <ctime>    // have time format and time(tm) datatype 
#include <string>   // for STRING data type, string::npos means until the end of file
#include <fstream>  // included <istream> and <ofstream> to read, create and write 
#include <algorithm> // for using transform()
#include <cstring>
#include <regex>     // To validate the date of birth
#include <stdexcept>
#include <vector>
#include <sstream>
using namespace std;

//for disabling error message for ctime().
#pragma warning( disable : 4996 )


//global variable
string covid19files[4] = { "Cat_A.txt","Cat_B.txt","Cat_Casual_Contact.txt","Class_MySJ.txt" };
// ReSharper disable once CppVariableCanBeMadeConstexpr
const int MAX_NO_IC_LENGTH = 12;
const string COUNTRY = "Malaysia";


//struct parts

struct Date
{
	int day;
	int month;
	int year;
};

struct Address
{
	int unit = 0;
	string street_name = "";
	string taman = "";
	string city = "";
	string state = "";
	int postcode = 0;
	string country = COUNTRY;
};
typedef struct
{
	string name;
	string useric;//Store user ic
	//Store user password
	Date dob; //Store user dob
	string contactnumber; //Store user contact number
	//string age; //Store user age
	Address address; //Store user address
	string username; //Store username
	string test_results;
	string password;  //Store user test results
}USER; //Yu_xiang part


string structAddressToString(const Address& s) {
	ostringstream STRING;
	STRING << s.unit << ","
		<< s.street_name << ","
		<< s.taman << ","
		<< s.city << ","
		<< s.state << ","
		<< s.postcode << ","
		<< s.country;
	return STRING.str();
}
string structDateToString(const Date& s) {
	ostringstream STRING;
	STRING << s.day << "/" << s.month << "/" << s.year;
	return STRING.str();
}

// function prototype
// BELOW YUXIANG
//For user
//void login_page(); //Read user details and verify user details
//void read_user_data(USER& user); //Read username and password
void check_login(USER& user); //Verify the username and password
void user_menu(USER user); //Let user the view details and update their test result

//For new user
bool check_user_name_exists(string username); //Check new user whether the registration name is repeated
void write_user_data(USER user); //Write user details into the UserData.txt file that insert by new user
void user_registration(); //Let user to create an account

//Update details
void update_user_ic(USER& user); //Function to update user ic
//void update_age(USER& user); //Function to update user age
void update_user_dob(USER& user); //Function to update user gender
void update_contactnumber(USER& user); //Function to update user contact number
void update_address(USER& user); //Function to update user address
void update_test_results(USER& user); //Use to update test result if user needed


//JingSheng
bool validateIC(string ic);
bool validateDate(string date_str, Date date);
bool validateContactNum(string contact_num);
//void saveUserToFile(const UserInfo& user, const string& filename1);

//Hieng
void UpdateMenu();
void updateTestResult();
void checkClassification();
void reportSuspected();


//Alwin
bool checkAlphabetAndCommas(string str);
string toAllUppercase(string str);
string replaceCommasWithSpace(string str);
void searchAndDeleteName(string filenames[], int numFiles, string name);
void add_user_cat(void);
void delete_user_cat(void);
void disp_admin_page(void);


int main(void)
{
	int num;
	USER user;

	cout << "=========================" << endl;
	cout << "       Login Page        " << endl;
	cout << "=========================" << endl;
	cout << "1. Existing User \n";
	cout << "2. New User \n\n";

	cout << "Choose an option by inputting a digit: ";
	cin >> num;
	cout << endl << endl;
	if (num == 1)
	{
		//login_page();
		check_login(user);
	}
	else
	{
		user_registration();
	}
	return 0;
}

//Change(Complete)
void check_login(USER& user) //Function to check if the entered username and password match
{
	//Open txt file for reading
	ifstream in_file("users.txt");
	if (!in_file)
	{
		cout << "Error opening input file. \n";
	}
	else
	{
		vector<USER> users_temp;
		USER user;
		while (in_file >> user.name >> user.useric >> user.dob.day >> user.dob.month >> user.dob.year >> user.contactnumber
			>> user.address.unit >> user.address.street_name >> user.address.taman >> user.address.city >> user.address.state >> user.address.postcode >> user.address.country
			>> user.username >> user.password)
		{
			users_temp.push_back(user);
		}in_file.close();

		string username, password;
		cout << "Enter username: ";

		cin >> username;
		cout << "Enter password: ";
		cin >> password;

		if (username == "admin" && password == "admin")
		{
			system("cls");
			disp_admin_page();
		};

		bool login_successful = false;
		USER current_user;
		for (const auto& u : users_temp)
		{
			if (username == u.username && password == u.password)
			{
				login_successful = true;
				current_user = u;
				break;

			}
		}
		if (login_successful)
		{
			cout << "Login success. \n";
			user_menu(current_user);
		}
		else {
			system("cls");
			cout << "Invalid username or password.\nPlease try again." << endl << endl;
			check_login(user);
		}
	}
}

//Function to register a new user
void user_registration()
{
	USER user;
	// Get user name
	string name;
	cout << "Name: ";
	getline(cin, name);
	if (name.empty())
	{
		throw invalid_argument("Invalid name");
	}
	// Convert name to uppercase
	transform(name.begin(), name.end(), name.begin(), ::toupper);
	user.name = name;

	// Get user IC
	string ic;
	do {
		cout << "No IC: ";
		getline(cin, ic);
	} while (!validateIC(ic));
	user.useric = ic;

	// Get user date of birth
	char c;
	string date_str;
	Date dob;

	cout << "Enter date of birth: ";
	cin >> dob.day >> dob.month >> dob.year;
	do
	{
		do
		{
			cout << "Date of birth (DD/MM/YYYY): ";
			getline(cin, date_str);
		} while (!validateDate(date_str, dob));
		user.dob = dob;
		cout << "Is this date of birth correct? (y/n): ";   // To confirm the input is correct or not
		cin >> c;
		cin.ignore();
	} while (c != 'y' && c != 'Y');

	//string dob;
	//cout << "Enter date of birth: ";
	//cin >> dob.day >> dob.month >> dob.year;

	string contactnumber;
	do
	{
		cout << "Phone Number: ";
		getline(cin, contactnumber);
	} while (!validateContactNum(contactnumber));
	user.contactnumber = contactnumber;

	// Get user address
	char confirm;
	Address address;
	do
	{
		cout << "Unit: ";
		cin >> address.unit;
		cin.ignore();
		cout << "Street Name: ";
		getline(cin, address.street_name);
		cout << "Taman: ";
		getline(cin, address.taman);
		cout << "Postcode: ";
		cin >> address.postcode;
		cin.ignore();
		cout << "City: ";
		getline(cin, address.city);
		cout << "State: ";
		getline(cin, address.state);
		address.country = "Malaysia";
		user.address = address;

		cout << endl;

		// Confirm address given by user
		cout << address.unit << ", " << address.street_name << "\n" << address.taman << ", " << address.postcode << ", "
			<< address.city << ", " << address.state << ", " << address.country << "\n";
		do
		{
			cout << "Is this your address? (y/n): ";
			cin >> confirm;
			cin.ignore();
		} while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N');
	} while (confirm != 'y' && confirm != 'Y');

	// Get user username
	string username;
	cout << "Username: ";
	getline(cin, username);
	if (username.empty()) {
		throw invalid_argument("Invalid username");
	}
	user.username = username;

	// Get user password
	string password;
	do {
		cout << "Password: ";
		getline(cin, password);
	} while (password.length() < 8);
	user.password = password;

	//USER user = { name, ic, dob.day, dob.month, dob.year, contactnumber, {address}, username, password };

	write_user_data(user);
	user_menu(user);

	cout << "User registered successfully. \n";
	return;
}

//Function to check if the entered username already exists in the file
bool check_user_name_exists(string username)
{
	ifstream in_file("users.txt");
	if (!in_file)
	{
		cout << "Error opening input file. \n";
		return false;
	}
	else
	{
		string file_username;
		string file_password;
		while (in_file >> file_username >> file_password)
		{
			if (username == file_username)
			{
				cout << "Username already exists. \n";
				in_file.close();
				return true;
			}
		}

		in_file.close();
		return false;
	}
}

//Function to write user data to file
void write_user_data(USER user)
{
	ofstream out_file("users.txt", ios::app);

	if (!out_file)
	{
		cout << "Error opening output file. \n";
	}
	else
	{
		out_file << "\n" << user.username << " " << user.password << " " << user.dob.day << "/" << user.dob.month << "/" << user.dob.year << " "
			<< user.contactnumber << " "
			<< user.address.unit << "," << user.address.street_name << "," << user.address.taman << "," << user.address.city << "," << user.address.state << "," << user.address.postcode << "," << user.address.country << " " << user.test_results; "\n";
		out_file.close();
	}
	return;
}

//Fucntion to update user ic
void update_user_ic(USER& user)
{
	string new_ic;
	cout << "Enter new ic number: ";
	cin >> new_ic;

	user.useric = new_ic;
	write_user_data(user);

	cout << "ic updated succesfully. \n";
}

//Function to update user dob
void update_user_dob(USER& user)
{
	Date new_dob;
	cout << "Enter new day of birth (Format: DD MM YYYY): ";
	cin >> new_dob.day >> new_dob.month >> new_dob.year;

	user.dob = new_dob;
	write_user_data(user);

	cout << "Day of birth updated succesfully. \n";
}

//Function to update user age
//void update_age(USER& user)
//{
//	string new_age;
//	cout << "Enter new age: ";
//	cin >> new_age;
//
//	user.age = new_age;
//	write_user_data(user);
//
//	cout << "Age updated succesfully. \n";
//}

//Function to update user contact number
void update_contactnumber(USER& user)
{
	string new_contactnum;
	cout << "Enter new contact number: ";
	cin >> new_contactnum;

	user.contactnumber = new_contactnum;
	write_user_data(user);

	cout << "Contact number updated succesfully. \n";
	return;
}

//Function to update user address
void update_address(USER& user)
{
	Address new_address;
	cout << "Enter new address: ";
	cin >> new_address.unit >> new_address.street_name >> new_address.taman >> new_address.city >> new_address.state >> new_address.postcode;

	user.address = new_address;
	write_user_data(user);

	cout << "Address updated succesfully. \n";
	return;
}

//Function to update user test results
void update_test_results(USER& user)
{
	string new_test_results;
	cout << "Enter new test results: ";
	cin >> new_test_results;

	user.test_results = new_test_results;
	write_user_data(user);

	cout << "Test results updated succesfully. \n";
	return;
}

//User menu
void user_menu(USER user)
{
	int choice;
	do
	{
		cout << "==============================\n";
		cout << "          USER MENU           \n";
		cout << "==============================\n";
		cout << "1. View user details \n";
		cout << "2. Update user_ic \n";
		cout << "3. Update day of birth \n";
		//cout << "4. Update age \n";
		cout << "5. Update contactnum \n";
		cout << "6. Update address \n";
		cout << "7. Update Menu \n";
		cout << "8. Exit\n";
		cout << "==============================\n";
		cout << "ENTER CHOICE: ";
		cin >> choice;
		cout << "\n";
		cout << "==============================\n";

		switch (choice)
		{
		case 1:
			cout << "User's Name: " << user.name << endl;
			cout << "User ic: " << user.useric << endl;
			cout << "Day of birth: " << structDateToString(user.dob) << endl;
			//cout << "Age: " << user.age << endl;
			cout << "Contact number: " << user.contactnumber << endl;
			cout << "Address: " << structAddressToString(user.address) << endl;
			cout << "Username: " << user.username << endl;
			//cout << "Test results: " << user.test_results << endl;
			cout << "==============================\n";
			break;
		case 2:
			update_user_ic(user);
			break;
		case 3:
			update_user_dob(user);
			break;
		case 4:
			//update_age(user);
			break;
		case 5:
			update_contactnumber(user);
			break;
		case 6:
			update_address(user);
			break;
		case 7:
			UpdateMenu();
			break;
		case 8:
			cout << "Exiting user menu. \n";
			break;
		}
	} while (choice != 8);

}


//JING SHENG
bool validateIC(string ic)
{
	if (ic.length() != MAX_NO_IC_LENGTH)
	{
		return false;
	}

	for (char c : ic)
		if (!isdigit(c))
		{
			return false;
		}

	if (ic == "000000000000")
	{
		return false;
	}

	return true;
}
/*bool validateDate(string date_str, Date date)
{
	regex date_regex("^\\d{2}/\\d{2}/\\d{4}$");
	if (!regex_match(date_str, date_regex))
	{
		return false;
	}

	int day = stoi(date_str.substr(0, 2));
	int month = stoi(date_str.substr(3, 2));
	int year = stoi(date_str.substr(6, 4));


	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100)
	{
		return false;
	}



	date = { day, month, year};
	return true;
}
*/
bool validateDate(string date_str, Date date)
{
	regex date_regex("^\\d{2}/\\d{2}/\\d{4}$");
	if (!regex_match(date_str, date_regex))
	{
		return false;
	}

	int day = stoi(date_str.substr(0, 2));
	int month = stoi(date_str.substr(3, 2));
	int year = stoi(date_str.substr(6, 4));


	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100)
	{
		return false;
	}



	date = { day, month, year };
	return true;
}

bool validateContactNum(string contact_num)
{
	if (contact_num.length() < 10 || contact_num.length() > 11)
	{
		return false;
	}

	for (char c : contact_num)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}

	if (contact_num == "0000000000")
	{
		return false;
	}

	return true;
}

/*void saveUserToFile(const UserInfo& user, const string& filename1)
{
	ofstream file1(filename1, ios::app);

	if (!file1)
	{
		throw runtime_error("Error: Failed to open file for writing");
	}

	// Write user info to file1
	file1 << user.name << "\n"
		<< user.ic << "\n"
		<< user.date_of_birth.day << "/"
		<< user.date_of_birth.month << "/"
		<< user.date_of_birth.year << "\n"
		<< user.contact_num << "\n"
		<< user.address.unit << ","
		<< user.address.street_name << ","
		<< user.address.taman << ","
		<< user.address.postcode << ","
		<< user.address.city << ","
		<< user.address.state << ","
		<< user.address.country << "\n"
		<< user.username << "\n"
		<< user.password << "\n";

	file1.close();
	return;
}*/

//Hieng
void UpdateMenu()
{
	int menu_num;
	char quit{};
	cout << "==========================================================================" << endl;
	cout << setw(28) << "*" << "UPDATE MENU" << "*" << endl;
	cout << "==========================================================================" << endl;
	cout << "[1]. UPDATE TEST RESULT" << setw(11) << "" << "[2]. CHECK FOR CLASSIFICATION UNDER MY SEJAHTERA" << endl;
	cout << "[3]. REPORT USER" << endl;
	do
	{
		
		cin >> menu_num;
		switch (menu_num)
		{
		case 1:
			updateTestResult(); // Call the function to update the test result
			break;
		case 2:
			checkClassification(); // Call the function to check classification
			break;
		case 3:
			reportSuspected(); // Call the function to report suspected user
			break;
		default:
			cout << "Invalid input. Please try again." << endl; // if the user enter the wrong input
			continue;
		}
		// this will prom the user to retry or exit the program
		cout << "Press 'q' to quit or any other key to return to the main menu." << endl;
		cin >> quit;
	} while (quit != 'q');
	return;
}


void updateTestResult() // PROM THE USER TO UPDATE THEIR TEST RESULT
{
	int menu_num = 0;
	char categories;
	char choice;

	cout << "WHAT IS YOUR TEST RESULT?" << endl;
	cout << "[A]. POSITIVE " << setw(11) << "" << "[B]. CLOSE CONTACT" << endl;
	cout << "[C]. CASUAL CONTACT" << endl;
	cin >> categories;
	cout << "==========================================================================" << endl;
	cout << setw(24) << "" << "STANDARD OPERATING PROCEDURES" << endl;
	cout << "__________________________________________________________________________" << endl;

	if (categories == 'A' || categories == 'a')
	{
		cout << "POSITIVE COVID-19" << endl;
		cout << "MUST BE QUARANTINE FOR 7 DAYS" << endl;
		cout << "CAN BE RELEASED EARLIER IF TESTED NEGATIVE ON DAY 4" << endl;
	}
	else if (categories == 'B' || categories == 'b')
	{
		cout << "CLOSE CONTACT WITH INDIVIDUALS FROM CATEGORY A" << endl;
		cout << "Are you staying with the close contact individual? (Enter y for yes or n for no)" << endl;
		cin >> choice;

		switch (choice)
		{
		case 'y':
			cout << "NEED TO BE QUANRANTINED IF STAYING WITH TEST POSITIVE COVID-19 FAMILY MEMBERS OR ROOMMATES" << endl;
			cout << "NEED TO DO SELF-TEST ON DAY 1 AND DAY 3" << endl;
			cout << "IF TESTED POSITIVE CATEGORY BECOME CATEGORY A" << endl;
			break;

		case 'n':
			cout << "DOES NOT NEED TO BE QUARANTINE" << endl;
			cout << "NO TEST REQUIRED" << endl;
			break;
		}
	}
	else if (categories == 'c' || categories == 'C')
	{
		cout << "NO QUARANTINES NEEDED" << endl;
		cout << "NO TEST NEEDED" << endl;
		cout << "UNDER MONITORING LIST" << endl;
	}
	return;
}
void checkClassification()// SHOW THE USER THE CLASSIFICATION UNDER MYSEJAHTERA
{
	int num;

	cout << "CLASSIFICATION IN MYSEJAHTERA" << endl;
	cout << "[1]. LOW RISK" << setw(11) << "" << "[2]. CLOSE CONTACT " << endl;
	cout << "[3]. SUSPECTED CASE" << setw(5) << "" << "[4]. PERSON UNDER SURVEILANCE(PUS)" << endl;
	cout << "[5]. CONFIRM CASE" << endl;
	cin >> num;
	switch (num)
	{
	case 1:
		cout << "[1]. LOW RISK - INDIVIDUAL WITH LOW RISK OF COVID-19 INFECTION." << endl;
		break;

	case 2:
		cout << "[2]. CLOSE CONTACT - INDIVIDUALS WHO HAVE BEEN EXPOSED TO OTHER INDIVIDUALS INFECTED WITH COVID-19." << endl;
		cout << "" << endl;

		cout << "BELOW SHOWS THOSE WHO MAY BE CLASSIFIED AS CLOSE CONTACT INDIVIDUALS: " << endl;
		cout << "" << endl;
		cout << "1.) HEALTH CARE ASSOCIATED EXPOSURE,INCLUDING PROVIDING DIRECT CARE FOR COVID 19 PATIENTS WITHOUT USING APPROPRIATE PPE," << endl;
		cout << "WORKING WITH HEALTHCARE WORKERS INFECTED WITH COVID-19, VISITING PATIENTS OR STAYING IN THE SAME CLOSE ENVIRONMENT OF A COVID-19 PATIENT." << endl;
		cout << "" << endl;
		cout << "2.)WORKING TOGETHEDR IN CLOSE PROXIMITY OR SDHARING THE SAME CLASSROOM ENVIRONMENT WITH A COVID-19 PATIENT." << endl;
		cout << "" << endl;
		cout << "3.)TRAVELING TOGETHER WITH COVID-19 PATIENT IN ANY KIND OF CONVEYANCE." << endl;
		cout << "" << endl;
		cout << "4.)LIVING IN THE SAME HOUSEHOLD AS A COVID-19 PATIENT." << endl;
		cout << "" << endl;
		break;

	case 3:
		cout << "[3]. SUSPECTED CASE - INDIVIDUALS WHO ARE AT RISK OF BEING INFECTED WITH COVID-19." << endl;
		cout << "FOR EXAMPLE : INDIVIDUALS WITH A HISTORY OF TRAVELLING ABROAD OR HAVE ATTENDED MASS GATHERINGS WITHIN 14 DAYS BEFORE ONSETS OF SYMTOMPS";
		break;
	case 4:

		cout << "[4]. PERSON UNDER SURVEILANCE(PUS) - INDIVIDUALS WHO ARE AT RISK OF BEING INFECTED WITH COVID-19. " << endl;
		cout << "FOR EXAMPLE : THOSE WHO HAVE TRAVEL HISTORY IN THE PAST 14 DAYS";
		break;
	case 5:
		cout << "[5]. CONFIRM CASE - INDIVIDUALS TESTED POSITIVE FOR COVID-19";
		break;
	default:
		cout << "ERROR, PLEASE RETRY." << endl;
	}
	return;
}
void reportSuspected() { // ALLOW THE USER TO REPORT SUSPECTED COVID CASE
	ofstream out_file("suspected.txt");
	char name[50];
	char area[100];
	cout << "==========================================================================" << endl;
	cout << setw(28) << "" << "REPORTING SYSTEM" << endl;
	cout << "==========================================================================" << endl;

	cout << "DO YOU KNOW ANY SUSPECTED COVID_19 PERSON? " << endl;
	cout << "PLEASE PROVIDE US WITH the NAME REGARDING THAT INDIVIDUAL." << endl;
	cout << "NAME: " << endl;
	cin.ignore();
	cin.getline(name, 50);
	out_file << name << endl;
	cout << "AREA: " << endl;

	cin.getline(area, 100);
	out_file << area << endl;

	out_file.close();
	return;
}


//ALWIN
bool checkAlphabetAndCommas(string str) { // return false for all digits and punctuation except ','
	for (int i = 0; i < str.length(); i++) {
		if (isdigit(str[i]) || (ispunct(str[i]) && str[i] != ',')) {
			return false;
		}
	}
	return true;
}
bool checkDigit(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}
string toAllUppercase(string str) { //Uppercase whole letters of a string
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}
string replaceCommasWithSpace(string str) { // change the ',' into ' ' inside a string
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ',') {
			str[i] = ' ';
		}
	}
	return str;
}
void searchAndDeleteName(string filenames[], int numFiles, string name) {

	string line;
	bool found = false;
	char choice;

	for (int i = 0; i < numFiles; i++) {
		ifstream c19_infile(filenames[i]);
		ofstream temp_outfile("temp.txt"); // create temporary file to hold modified contents
		while (getline(c19_infile, line))
		{
			if (line.find(name) != string::npos) { // if search string found in line
				found = true;
				system("cls");
				cout << "Found \033[1;34m" << name << "\033[0m in file \033[40;34m" << filenames[i] << "\033[0m" << endl;
				cout << "Found name: \033[1;32m" << name << "\033[0m \nOrigin line: \033[1;32m" << line << "\033[0m" << endl << endl;
				cout << "Do you want to delete this line? (y/n)" << endl;

				cin >> choice;

				if (choice == 'y' || choice == 'Y') {
					continue;
				}
			}
			temp_outfile << line << endl;
		};
		c19_infile.close();
		temp_outfile.close();
		if (found) {
			int rename_result;
			remove(filenames[i].c_str()); // delete original file
			rename_result = rename("temp.txt", filenames[i].c_str()); // rename temporary file to original file

			if (rename_result == 0)
			{
				cout << filenames[i] << " renamed successful.\033[33;1m(For Error Checking Purpose)\033[0m\n";
				break;
			}
			cout << "\033[1;31m#Error: Rename Unsuccessful.\033[0m Need check the file and its path directory.";
		}
		else {
			remove("temp.txt"); // delete temporary file	
		};

	};

	if (found == true && (choice == 'Y' || choice == 'y'))  // A confirmation of deleted name
		cout << "\n\n \033[32m" << name << "\033[0m is deleted.\n\n";
	//else if (found == true && (!choice == 'Y' || !choice == 'y'))
	//	cout << "\n\n \033[32m" << name << "\033[0m is not deleted.\n\n";
	else cout << "\n\n \033[32m" << name << "\033[0m not deleted couldn't be found in the system.\n\n";


	cout << "This is the end of searching. Do you want stay at Delete User Category Page\n";
	cout << "or go back to main Admin Page?\n\n  (1) Stay\n  (2) Back to Admin Page\n  (any number beside (1) also go back to admin page)\n";
	cout << "Your input: ";

	int end_of_del_page;
	cin >> end_of_del_page;

	if (end_of_del_page == 1)
	{
		system("cls");
		delete_user_cat();
	}
	else
	{
		system("cls");
		disp_admin_page();
	}

	return;
}
void add_user_cat(void)
{// admin assign c19 categories into user status respectively
	string full_name;
	string C19_category;

	cout << "Update User Covid-19 Category\n" << "=============================\n\n";
	cout << "Please type in full name that need to be added into Covid-19 category.\nExample: LOW,MING,DING\nType \"99back\" to go back main admin menu. \n\nYour name : ";
	cin >> full_name;


	full_name = toAllUppercase(replaceCommasWithSpace(full_name));
	if (full_name == "99BACK") {
		system("cls");
		cout << "#Msg: Showing an error message is considered normal after choosing option at main admin menu page.\n\n";
		disp_admin_page();
	};
	if (!checkAlphabetAndCommas(full_name)) { //check whether the name contains digits, spaces or not
		system("cls");
		cout << "\033[1;31m#Error: Invalid name.\033[0m Please retype your full name.\nCurrent page will be reloaded.\n\n\n";
		add_user_cat();
	}
	else {
		cout << "\n\nName input successful!\n\n"
			<< "What Covid-19 category to put into?\n";
		cout << "1) Category A\n2) Category B\n3) Category Casual Contact \n4) MySejahtera Classification\n\n" << "Your input: ";
	};

	cin >> C19_category;

	if (C19_category == "1")
	{
		ofstream add_cat_outfile("Cat_A.txt", ios::app);
		add_cat_outfile << endl << toAllUppercase(full_name);
		add_cat_outfile.close();
		system("cls");
		cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
		add_user_cat();

	}
	else if (C19_category == "2") {
		ofstream add_cat_outfile("Cat_B.txt", ios::app);
		add_cat_outfile << endl << toAllUppercase(full_name);
		add_cat_outfile.close();
		system("cls");
		cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
		add_user_cat();
	}
	else if (C19_category == "3") {
		ofstream add_cat_outfile("Cat_Casual_Contact.txt", ios::app);
		add_cat_outfile << endl << toAllUppercase(full_name);
		add_cat_outfile.close();
		system("cls");
		cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
		add_user_cat();
	}
	else if (C19_category == "4")
	{
		string MySJ_category;
		ofstream add_catA_outfile("Class_MySJ.txt", ios::app);
		do {
			cout << "Which MySejahtera category you want to assign?(choose a number)\n";
			cout << "1)Low Risk\n2)Close Contact\n3)Person Under Surveillance(PUS)\n4)Suspected Case\n5)Confirmed Case\n\n" << "Your input: ";
			cin >> MySJ_category;

			if (MySJ_category == "1")
			{
				add_catA_outfile << endl << toAllUppercase(full_name) << ", Low Risk";
				add_catA_outfile.close();
				system("cls");
				cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
				add_user_cat();
			}
			else if (MySJ_category == "2")
			{
				add_catA_outfile << endl << toAllUppercase(full_name) << ", Close Contact";
				add_catA_outfile.close();
				system("cls");
				cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
				add_user_cat();
			}
			else if (MySJ_category == "3") {
				add_catA_outfile << endl << toAllUppercase(full_name) << ", Person Under Surveillance";
				add_catA_outfile.close();
				system("cls");
				cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
				add_user_cat();
			}
			else if (MySJ_category == "4") {
				add_catA_outfile << endl << toAllUppercase(full_name) << ", Suspected Case";
				add_catA_outfile.close();
				system("cls");
				cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
				add_user_cat();
			}
			else if (MySJ_category == "5") {
				add_catA_outfile << endl << toAllUppercase(full_name) << ", Confirmed Case";
				add_catA_outfile.close();
				system("cls");
				cout << "\033[32mMessage: Insert successful!\n" << "Redirecting to Add User Category Page...\033[0m\n\n";
				add_user_cat();
			}
			else
			{
				system("cls");
				cout << "\033[1;31m#Error: Invalid MySJ category option.\033[0m Please choose your option again.\nCurrent MySJ cateogry page will be reloaded.\n\n";

			};

		} while (MySJ_category < "1" || MySJ_category > "5" || !checkDigit(MySJ_category));

	}
	else {
		do {
			system("cls");
			cout << "\033[1;31m#Error: Category option invalid.\033[0m Have to start over.\n Page wiil be reloaded.\n";
			add_user_cat();
		} while (C19_category < "1" || C19_category > "4" || !checkDigit(C19_category));
	};

	return;
}
void delete_user_cat(void)
{
	string full_name;

	cout << "=============================\n"
		<< "Delete User Covid-19 Category\n"
		<< "=============================\n\n";

	cout << "Please type in full name that need to be deleted from assigned Covid-19 category.\nExample: LOW,MING,DING\nType \"99back\" to go back main admin menu. \n\nYour name: ";
	cin >> full_name;
	full_name = toAllUppercase(replaceCommasWithSpace(full_name));


	if (full_name == "99BACK") {
		system("cls");
		cout << "#Msg: Showing an error message is considered normal after choosing option at main admin menu page.\n\n";
		disp_admin_page();
	};

	if (!checkAlphabetAndCommas(full_name)) { //check whether the name contains digits, spaces or not
		system("cls");
		cout << "\033[1;31m#Error: Invalid name or name contains digits and unrelated symbols.\033[0m Please retype your full name.\nCurrent page will be reloaded.\n\n";
		delete_user_cat();
	};

	cout << "System is searching the name.........\n";
	string temp_C19_filename;
	searchAndDeleteName(covid19files, 4, full_name);
	//deleteName(full_name,temp_C19_filename);

	return;
}
void disp_admin_page(void)
{

	string function;
	cout << "==================================" << endl;
	cout << "||     You are in Admin Page.   ||" << endl;
	cout << "==================================" << endl << endl;

	cout << "List of functions:\n (1) Add User Category\n (2) Delete User Category\n(99) Exit Program" << endl << endl;
	cout << "Please enter the following number to access its function: ";
	cin >> function;
	system("cls");
	if (function == "1")
		add_user_cat();
	else if (function == "2")
		delete_user_cat();
	else if (function == "99")
	{
		// Get current time
		time_t currentTime;
		time(&currentTime);

		// Convert current time to string
		char* timeString = ctime(&currentTime);

		cout << "\n\n\n\n\n\t\t\t\033[1;31m\033[1mExiting.......\033[0m\n\n\n";  // Bold red colour of "exiting"
		cout << "\tYou have exited at " << "\033[1;33m" << timeString << "\033[0m" << "\n\n\n"; // yellow colour of current time
		exit(100);
	}
	else {
		do
		{
			system("cls"); // clear command screen
			cout << "\033[1;31m#Error: Invalid Option.\033[0m \n";
			cout << "Retype with correct number.Admin page will be reloaded." << endl << endl;
			disp_admin_page();

		} while (function < "1" || function > "2" || !checkDigit(function));

	}
	return;
}