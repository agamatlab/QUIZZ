﻿#pragma once

void mySetColor(int fg, int bg) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, fg | (bg << 4));
}


void readParticipants(string filepath) {
	ifstream fileIN(filepath);
	if (!fileIN.is_open()) throw exception("LeaderBoard can not be Accessed...");

	Participant p;
	while (fileIN >> p)
		participants.push_back(p);

	fileIN.close();
}


void updateLeaderBoard(Participant& newParticipant, string);


bool manageChoice(unsigned short& choice, unsigned short answerCount) noexcept
{
	// Uİ'da dəyərlərin keyboardla idarəsini təmin edir.

	char kb = _getch();
	if (kb == 224 || kb == -32) kb = _getch();

	switch (kb)
	{
	case 'w':
	case 'W':
	case 'a':
	case 'A':
	case UP:
	case LEFT:
		if (choice == 0) choice = answerCount;
		choice--;
		return true;
	case 's':
	case 'S':
	case 'd':
	case 'D':
	case DOWN:
	case RIGHT:
		choice++;
		choice %= answerCount;
		return true;
	case ENTER:
		return false;
		break;
	default:
		return true;
	}
}


short yes_no(const char* question, string answ0 = "YES", string answ1 = "NO", bool existESC = false) noexcept 
{
	unique_ptr<string[]> answers(new string[]{ answ0,answ1 });
	int answerCount = 2;
	bool notFound(true);
	U_short choice = 0;

	mySetColor(LIGHTGREEN, BLACK);

	while (notFound)
	{

		system("cls");
		cout << question << endl;
		for (size_t i = 0; i < answerCount; i++)
		{
			char prefix = ' ';
			if (i == choice) { mySetColor(LIGHTGREEN, GREY); prefix = char(254); }
			cout << ' ' << prefix << " <<" << answers[i] << ">>" << endl;
			mySetColor(LIGHTGREEN, BLACK);
		}
		if (existESC) cout << "ESC - EXIT\n";

		notFound = manageChoice(choice, answerCount);
		if(existESC && GetAsyncKeyState(VK_ESCAPE)) return -1;
	}

	return choice;
}


void sortLeaderBoard() noexcept {

	sort(participants.begin(), participants.end(),
		[](Participant& p1, Participant& p2)
		{return p1.getScore() > p2.getScore(); });
}


void showTopLeaderBoard(int top = 10) noexcept {

	int i = 0;
	for (auto& participant : participants)
	{
		cout << ++i << ") " << participant.getName() << ' ' << participant.getScore() << endl;
		if (i == top) break;
	}

	cout << "\n\nPress any key to continue...\n\n";

	cin.get();
}


bool isQuizExist(string quizName) noexcept {
	for (auto& fileName : fileNames)
		if (quizName == fileName) return true;
	return false;

}


void showResults(string leaderPath) {
	int answer[3]{};

	cout << leaderPath << endl;
	for (auto& question : questions)
		if (question.selectedAnswer == -1) answer[2]++;
		else if (question.selectedAnswer == question.correctAnswer) answer[0]++;
		else answer[1]++;

	cout << "True Answers: " << answer[0] << endl;
	cout << "False Answers: " << answer[1] << endl;
	cout << "Not Answered: " << answer[2] << endl;

	Sleep(2000);

	bool showLeaderBoard = !yes_no("Do you Wish to add your score in LeaderBoard?");
	currentPlayer.setScore(round(float(answer[0]) / questions.size() * 100));

	if (showLeaderBoard) {
		try	{ readParticipants(leaderPath); }
		catch (const std::exception& ex)
		{
			cout << "You are the only one on LeaderBoard...\n";
			Sleep(2000);
		}
		updateLeaderBoard(currentPlayer, leaderPath);
		sortLeaderBoard();
		showTopLeaderBoard();
	}
}


void readFileNames() {
	ifstream fileList("fileList.txt", ios::in);
	if (!fileList.is_open()) throw exception("Quiz can not be openned...");
	string line;

	while (getline(fileList, line))
		fileNames.push_back(line);

	fileList.close();
}


string getFile() {

	if (!fileNames.size()) throw exception("No Quiz Exsists...");

	int answerCount = fileNames.size();
	bool notFound(true);
	U_short choice = 0;

	mySetColor(LIGHTGREEN, BLACK);

	while (notFound)
	{

		system("cls");
		cout << "Which Quiz Do You want To Play?" << endl;
		int i = 0;
		for (string name : fileNames)
		{
			char prefix = ' ';
			if (i++ == choice) { mySetColor(LIGHTGREEN, GREY); prefix = char(254); }
			cout << ' ' << prefix << " <<" << name << ">>" << endl;
			mySetColor(LIGHTGREEN, BLACK);
		}
		cout << "ESCAPE <-> BACK\n";
		notFound = manageChoice(choice, answerCount);
		if (GetAsyncKeyState(VK_ESCAPE)) return "";
	}

	return fileNames[choice];
}


short manageQuiz(unsigned short& choice)
{
	// Uİ'da dəyərlərin keyboardla idarəsini təmin edir.

	char kb = _getch();
	if (kb == 224 || kb == -32) kb = _getch();

	switch (kb)
	{
	case 'a':
	case 'A':
	case LEFT:
		if (questions.begin() == currentQuestion) { currentQuestion = --questions.end(); currentQuestionCount = questions.size(); }
		else { --currentQuestion; --currentQuestionCount; }
		return false;
	case 'w':
	case 'W':
	case UP:
		if (choice == 0) choice = (*currentQuestion).answers.size();
		choice--;
		return true;
	case 'd':
	case 'D':
	case RIGHT:
		if (--questions.end() == currentQuestion) { currentQuestion = questions.begin(); currentQuestionCount = 1; }
		else { ++currentQuestion; ++currentQuestionCount; }
		return false;
	case 's':
	case 'S':
	case DOWN:
		choice++;
		choice %= (*currentQuestion).answers.size();
		return true;
	case ENTER:
		if (choice == (*currentQuestion).selectedAnswer) 
			(*currentQuestion).selectedAnswer = -1;
		else 
			(*currentQuestion).selectedAnswer = choice;
		return true;
	case 'e':
	case 'E':
		return -1;
	default:
		return true;
	}
}


void readFromFile(string filename) {

	ifstream file(filename);
	if (!file.is_open()) throw exception("Quiz can not be openned...");
	Question q;
	while (file >> q)
		questions.push_back(q);
	file.close();
}


void logParticipant(string date, string name) {
	ofstream file("participantList.txt", ios::app);
	if (!file.is_open()) throw exception("Can not LOG the participant.");
	file << date << " --> " << name << endl;
	file.close();
}


void signUP() {
	Admin a;
	cin >> a;

	for (auto& admin : adminstrators)
		if (admin.getLogin() == a.getLogin()) throw exception("Username Already Exists...");

	ofstream file("admins.txt", ios::app);
	if (!file.is_open()) throw exception("Can't SIGN UP User...\nTry again later.");
	file << a;

	file.close();

	adminstrators.push_back(a);
}


void updateQuiz(string);


void signIN() {
	string name, password;
	cout << "Enter USERNAME: ";
	getline(cin, name);

	bool Found_USER(false);
	Admin a;

	for(auto admin : adminstrators) 
		if (admin.getLogin() == name)
			{ a = admin; Found_USER = true; }

	if (!Found_USER) throw exception("False Username Entered...");;
	
	cout << "Enter Password: ";
	getline(cin, password);
	
	if (a.getPassword() != password) 
		throw exception("False Password Entered...");

	cout << "SIGNED IN SUCCESSFULLY!..\n";
	Sleep(1000);
}


void changeQuiz() {
	while (true) {
		try
		{
			string file;
			string path;

			short choice = yes_no("Do you want to ADD or APPEND?", "ADD", "APPEND", true);
			if (choice == -1) return;
			if (!choice) {
				cout << "Enter quiz Name (NO EXTENSION): ";
				getline(cin, file);

				if (file == "fileList" || file == "leaderboard"
					|| file == "participantList" || file == "admins")
					throw exception("UnSupported QUIZ name...");

			}
			else
				try { file = getFile(); if (file == "") throw exception("No File chosen..."); }
			catch (const std::exception& ex) {
				cout << ex.what() << endl; cout << "\n\nPress any key to continue...\n\n"; cin.get(); continue;
			}

			path = currentPath + '\\' + file;
			create_directory(path);

			file += ".txt";

			if (file != "")updateQuiz(path + '\\' + file);
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
			cout << "\n\nPress any key to continue...\n\n";
			cin.get();
			system("cls");
		}
	}
}


void EnterMenu() {
	short choice = yes_no("Do you want to:", "SIGN IN", "SIGN UP", true);;
	if (!choice) { 
		signIN();
		changeQuiz();
	}
	else if(choice == 1) signUP();

}


void startQuiz() {

	string fileName = getFile();
	if (fileName == "") return;
	
	fileName = currentPath + '\\' + fileName + '\\' + fileName + ".txt";
	readFromFile(fileName);

	if (!questions.size()) throw exception("No Question Exist in QUIZ.");

	{
		vector<Question> tempQuestion(questions.begin(), questions.end());
		shuffle(tempQuestion.begin(), tempQuestion.end(), std::default_random_engine(time(0)));
		copy(tempQuestion.begin(), tempQuestion.end(), questions.begin());
	}

	U_short choice = 0;
	currentQuestion = questions.begin();

	mySetColor(LIGHTGREEN, BLACK);

	while(true)
	try
	{
		string name;
		cout << "Enter Your name: ";
		getline(cin, name);

		currentPlayer.setName(name);
		break;
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
		cout << "\n\nPress any key to continue...\n\n";
		cin.get();
		system("cls");
	}

	currentPlayer.updateTime();

	try 
		{ logParticipant(currentPlayer.getTime(), currentPlayer.getName());}
	catch (const std::exception& ex) 
		{ cout << ex.what() << endl; cout << "\n\nPress any key to continue...\n\n"; cout << "\n\nPress any key to continue...\n\n"; cin.get(); }


	while (true)
	{
		system("cls");
		cout << " " << currentQuestionCount << ") " << (*currentQuestion).question << endl;
		int i = 0;

		for (auto& answer : (*currentQuestion).answers)
		{
			char prefix = ' ';
			if (i == (*currentQuestion).selectedAnswer)prefix = char(254);
			if (i++ == choice) { mySetColor(LIGHTGREEN, GREY); }
			cout << ' ' << prefix << " <<" << answer << ">>" << endl;
			mySetColor(LIGHTGREEN, BLACK);
		}
		short input = manageQuiz(choice);
		if (input == -1) { showResults(fileName.substr(0, fileName.rfind('.')) + "L.txt"); exit(777);   }
		if (!input) choice = 0;
	}


}


void updateLeaderBoard(Participant& newParticipant, string filename) {
	ofstream fileOut(filename, ios::app);
	if (!fileOut.is_open()) throw exception("Leaderboard can not be updated...");
	fileOut << newParticipant;
	fileOut.close();

	participants.push_back(newParticipant);
}


void readAdminstrators() {
	ifstream file("admins.txt");
	if (!file.is_open()) throw exception("No Admin EXIST...");
	Admin temp;

	while (file >> temp)
		adminstrators.push_back(temp);
	
	file.close();
}


void updateQuiz(string path) {
	
	ofstream file(path.substr(0, path.rfind('.')) + "L.txt", ios::app);
	if (!file) throw exception("addQuiz - Specified Quiz Leaderboard can not be created");
	if (!file.is_open()) throw exception("addQuiz - Specified Quiz Leaderboard can not be openned");
	
	file.close();

	file.open(path, ios::app);

	if (!file) throw exception("addQuiz - Specified Quiz can not be created");
	if (!file.is_open()) throw exception("addQuiz - Specified Quiz can not be openned");

	string filename = path.substr(path.rfind('\\') + 1);
	filename = filename.substr(0, filename.rfind('.'));
	if (!isQuizExist(filename)) {

		try
		{
			ofstream fileList("fileList.txt", ios::app);
			fileList << filename << endl;
			fileList.close();

			fileNames.push_back(filename);
		}
		catch (const std::exception& ex)
		{
			string sourceName = "addQuiz - ";
			throw exception((sourceName + ex.what()).c_str());
		}

	}

	while (true) {
		Question q;
		cout << "Enter the question: ";
		getline(cin, q.question);

		string temp;
		cout << "Enter Correct Answer: ";
		getline(cin, temp);
		q.answers.push_back(temp);


		for (size_t i = 0; i < MAX_ANSWER - 1; i++)
		{
			cout << "Enter Another Answer: ";
			getline(cin, temp);
			q.answers.push_back(temp);

			if (i == MAX_ANSWER - 2) break;
			if (yes_no("Do you want to add Another Answer?")) break;
		}

		file << q;
		cout << "Question added Successfully...\n";

		if (yes_no("Do you want to add Another Question?")) break;

	}
	file.close();
}