#pragma once

#pragma warning(disable : 4996)

typedef unsigned short U_short;

enum Color { BLACK = 0, GREEN = 2, AQUA = 3, RED = 4, DEFAULT = 7, GREY = 8, LIGHTGREEN = 10, LIGHTPURPLE = 13 };
enum Keys { LEFT = 75, UP = 72, RIGHT = 77, DOWN = 80, ENTER = 13, SPACE = 32 };

#define MAX_ANSWER 5



class User {
protected:
	string _time;

public:
	void updateTime() {
		time_t result = time(nullptr);
		_time = ctime(&result);
		_time.pop_back();
	}

	string getTime() const { return _time; }

	User() {
		updateTime();
	}

};


class Admin : public User {
	string _login;
	string _password;

public:
	
	string getLogin() const { return _login; }
	string getPassword() const { return _password; }

	Admin() = default;

	Admin(string pass, string login) {
		setPass(pass);
		setLogIN(login);
	}

	void setPass(string password) { if (password.size() < 8) throw exception("Password must be longer than 7 characters..."); _password = password; }
	void setLogIN(string login) { if (login.size() < 3) throw exception("Login must be longer than 2 characters..."); _login = login; }

	friend ofstream& operator<<(ofstream& fout, Admin& a) {
		fout << a._login << endl;
		fout << a._password << endl;
		fout << a._time << endl;
		return fout;
	}

	friend ifstream& operator>>(ifstream& fin, Admin& a) {
		getline(fin,a._login);
		getline(fin,a._password);
		getline(fin,a._time);

		return fin;
	}
	
	friend istream& operator>>(istream& in, Admin& a) {
		string line;
		cout << "Ener USERNAME: ";
		getline(in, line);
		a.setLogIN(line);

		cout << "Ener PASSWORD: ";
		getline(in, line);
		a.setPass(line);

		a.updateTime();
		return in;
	}
};






class Participant : public User {
	string _name = "";
	float _score = 0;
public:

	string getName() const { return _name; }
	float getScore() const { return _score; }

	void setName(string name) {
		if (name.size() < 3) 
			throw exception("Name must be Longer than 2 characters.");
		_name = name;
	}

	void setScore(float score) { 
		if (score > 100 || score < 0) 
			throw exception("Score must be lower than 100 and higher than 0"); 
		_score = score; 
	}


	Participant() = default;
	Participant(string name, float score) {
		setName(name);
		setScore(score);
	};

	friend ifstream& operator>>(ifstream& fin, Participant& p) {
		getline(fin, p._name);
		fin >> p._score;
		fin.get();

		return fin;
	}
	friend ofstream& operator<<(ofstream& fout, Participant& p) {
		fout << p.getName() << endl;
		fout << p.getScore() << endl;
		return fout;
	}

};


class Question {
public:
	string question = "";
	vector<string> answers;
	short selectedAnswer = -1;
	U_short correctAnswer = 0;

	Question() = default;
	Question(string question, vector<string> answers) : question(question), answers(answers) {}

	friend ifstream& operator>>(ifstream& fin, Question& q) {
		if (q.answers.size() != 0)q.answers.clear();
		q.correctAnswer = 0;

		getline(fin, q.question);
		string line, answerTemp;
		getline(fin, answerTemp);
		q.answers.push_back(answerTemp);

		while (getline(fin, line)) {
			if (line == "=") break;
			q.answers.push_back(line);
		}
		
		shuffle(q.answers.begin(), q.answers.end(), std::default_random_engine(time(0)));

		for (auto& answer : q.answers)
			if (answerTemp != answer) q.correctAnswer++;
			else break;

		return fin;
	}


	friend ostream& operator<<(ofstream& fout, Question& q) {
		fout << q.question << endl;
		for (auto& answer : q.answers)
			fout << answer << endl;
		fout << '=' << endl;

		return fout;
	}
};

