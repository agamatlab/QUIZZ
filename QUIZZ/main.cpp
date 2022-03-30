#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<fstream>
#include<algorithm>
#include<windows.h>
#include<memory>
#include<iomanip>
#include<random>
#include<queue>
#include<filesystem>
#include<conio.h>
using namespace std;
using namespace std::filesystem;


class Question;

list<Question> questions;
list<Question>::iterator currentQuestion;
int currentQuestionCount = 1;
string currentPath = current_path().string();
 
#include "objects.h"
Participant currentPlayer;
vector<Admin> adminstrators;
vector<string> fileNames;
vector<Participant> participants;
#include "functions.h"


int main() {
	try 
		{ readFileNames(); }
	catch (const std::exception& ex) 
		{ cout << ex.what() << endl; cout << "\n\nPress any key to continue...\n\n"; cin.get(); system("cls"); }
	
	try 
		{ readAdminstrators(); }
	catch (const std::exception& ex) 
		{ cout << ex.what() << endl; cout << "\n\nPress any key to continue...\n\n"; cin.get(); }
	
	gameStart();

	return EXIT_SUCCESS;
}