// Facile.cpp
#include "ProgramState.h"
#include "Statement.h"
#include "LetStatement.h"
#include "PrintStatement.h"
#include "PrintallStatement.h"
#include "AddStatement.h"
#include "SubStatement.h"
#include "EndStatement.h"
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <cstdlib>
#include <iostream> // added 


using namespace std;

// parseProgram() takes a filename as a parameter, opens and reads the
// contents of the file, and returns an vector of pointers to Statement.
void parseProgram(istream& inf, vector<Statement *> & program);

// parseLine() takes a line from the input file and returns a Statement
// pointer of the appropriate type.  This will be a handy method to call
// within your parseProgram() method.
Statement * parseLine(string line);

// interpretProgram() reads a program from the given input stream
// and interprets it, writing any output to the given output stream.
// Note:  you are required to implement this function!
void interpretProgram(istream& inf, ostream& outf);

void parseMathOperation(stringstream &ss, string &var, 
	int &val, string &var_rhs, bool &isRhsDigit);


int main()
{
        cout << "Enter Facile program file name: ";
        string filename;
        getline(cin, filename);
        ifstream infile(filename.c_str());
        if (!infile)
        {
                cout << "Cannot open " << filename << "!" << endl;
                return 1;
        }
        interpretProgram(infile, cout);
}



void parseProgram(istream &inf, vector<Statement *> & program)
{
	program.push_back(NULL);
	
	string line;
	while( ! inf.eof() )
	{
		getline(inf, line);
		program.push_back( parseLine( line ) );
	}
}


Statement * parseLine(string line)
{
	Statement * statement;	
	stringstream ss;
	string type;
	string var;
	string var_rhs;
	int val;
	bool isRhsDigit;

	ss << line;
	ss >> type;
	
	if (type == "LET")
	{
		ss >> var;
		ss >> val;
		// Note:  Because the project spec states that we can assume the file
		//	  contains a syntactically legal Facile program, we know that
		//	  any line that begins with "LET" will be followed by a space
		//	  and then a variable and then an integer value.
		statement = new LetStatement(var, val);
	}

	if (type == "PRINT") {
		ss >> var;
		statement = new PrintStatement(var);
	}

	if (type == "PRINTALL") {
		statement = new PrintallStatement();
	}

	if (type == "ADD") {
		parseMathOperation(ss, var, val, var_rhs, isRhsDigit);
		if (isRhsDigit)
			statement = new AddStatement(var, val);
		else
			statement = new AddStatement(var, var_rhs);
	}

	if (type == "SUB") {
		parseMathOperation(ss, var, val, var_rhs, isRhsDigit);
		if (isRhsDigit)
			statement = new SubStatement(var, val);
		else
			statement = new SubStatement(var, var_rhs);
	}

	if (type == "END" || type[0] == '.') {
		statement = new EndStatement();
	}

		
	return statement;
}


void interpretProgram(istream& inf, ostream& outf)
{
	vector<Statement *> program;
	parseProgram( inf, program );

	// Continuing part
	ProgramState * state = new ProgramState(program.size() - 1);

	cout << "Lines of program: " << state->getNumLines() << endl << endl;

	while (state->getLine() > 0 && state->getLine() <= state->getNumLines()) {
		program[state->getLine()]->execute(state, outf);
		//cout << "After execution, Line number: " << state->getLine() << endl;
	}

	for (uint i = 1; i < program.size(); i++) {
		delete program[i];
	}
	delete state;
}

void parseMathOperation(stringstream &ss,
	string &var, int &val, string &var_rhs, bool &isRhsDigit) {

	ss >> var;
	// check digit or variable, alternative way is to read in string and use strtoi
	while (ss.peek() == ' ' || ss.peek() == '\t') ss.get();
	if (isdigit(ss.peek()) || ss.peek() == '-') {
		ss >> val;
		isRhsDigit = true;
	}
	else {
		ss >> var_rhs;
		isRhsDigit = false;
	}
}