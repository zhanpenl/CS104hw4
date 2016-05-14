// Facile.cpp
#include "ProgramState.h"
#include "Statement.h"
#include "LetStatement.h"
#include "PrintStatement.h"
#include "PrintallStatement.h"
#include "AddStatement.h"
#include "SubStatement.h"
#include "MultStatement.h"
#include "DivStatement.h"
#include "GotoStatement.h"
#include "IfStatement.h"
#include "GosubStatement.h"
#include "ReturnStatement.h"
#include "EndStatement.h"
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <cstdlib>

#include <iostream> // added 
#include <cstring>
#include <cerrno>


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


int main(int argc, const char *argv[])
{
    // cout << "Enter Facile program file name: ";
    // string filename;
    // getline(cin, filename);
    // ifstream infile(filename.c_str());
    // if (!infile)
    // {
    //         cout << "Cannot open " << filename << "!" << endl;
    //         return 1;
    // }
    // interpretProgram(infile, cout);
    if(argc < 3)
    {
        std::cerr << "Program usage: ./Facile inputFile outputFile" << std::endl;
        return 1;
    }

    std::ifstream inFile(argv[1]);
    std::ofstream outFile(argv[2]);
    if (!inFile)
    {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }
    if(!outFile)
    {
        std::cerr << "Could not open file " << argv[2] << std::endl;
        return 1;
    }

    interpretProgram(inFile, outFile);
    inFile.close();
    outFile.close();

    return 0;
}



void parseProgram(istream &inf, vector<Statement *> & program)
{
	program.push_back(NULL); // 0th line, null statement
	
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
	int val, gotoLine;
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


	if (type == "MULT") {
		parseMathOperation(ss, var, val, var_rhs, isRhsDigit);
		if (isRhsDigit)
			statement = new MultStatement(var, val);
		else
			statement = new MultStatement(var, var_rhs);
	}

	if (type == "DIV") {
		parseMathOperation(ss, var, val, var_rhs, isRhsDigit);
		if (isRhsDigit)
			statement = new DivStatement(var, val);
		else
			statement = new DivStatement(var, var_rhs);
	}

	if (type == "GOTO") {
		ss >> gotoLine;
		// cout << "Parse GOTO: gotoLine = " << gotoLine << endl;
		statement = new GotoStatement(gotoLine);
	}

	if (type == "IF") {
		string oprSymbol, then;
		ss >> var;
		ss >> oprSymbol;
		ss >> val;
		ss >> then;	// don't check the string "THEN"
		ss >> gotoLine;

		statement = new IfStatement(var, oprSymbol, val, gotoLine);
	}

	if (type == "GOSUB") {
		ss >> gotoLine;
		statement = new GosubStatement(gotoLine);
	}

	if (type == "RETURN") {
		statement = new ReturnStatement();
	}


	if (type == "END" || type[0] == '.') {
		statement = new EndStatement();
	}

	// static int count = 1;
	// cout << count << ": " << type << endl;
	// count++;

	// if none of the above if statement is entered, null statement would return
	// causing seg-fault when deleting
	return statement;
}


void interpretProgram(istream& inf, ostream& outf)
{
	vector<Statement *> program;
	parseProgram( inf, program );

	// Continuing part
	ProgramState * state = new ProgramState(program.size() - 1); // there is a null statement at head

	cout << "Lines of program: " << state->getNumLines() << endl << endl;

	while (state->getLine() > 0 && state->getLine() <= state->getNumLines()) {
		program[state->getLine()]->execute(state, outf);
		cout << "After execution, Line number: " << state->getLine() << endl;
	}

	for (uint i = 0; i < program.size(); i++) {
		// cout << "the " << i << "th time:" << endl;
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