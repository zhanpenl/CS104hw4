#include "ProgramState.h"

using namespace std;

ProgramState::ProgramState(int numLines) {
	m_numLines = numLines;
	m_currentLine = 1;
	varMap = new map<string, int>;
	programStack = new StackInt;
}

ProgramState::~ProgramState() {
	delete varMap;
	delete programStack;
}

int ProgramState::getNumLines() {
	return m_numLines;
}

int ProgramState::getLine() {
	return m_currentLine;
}

void ProgramState::setLine(int line) {
	m_currentLine = line;
}

bool ProgramState::containsVariable(string variableName) {
	if (varMap->find(variableName) != varMap->end())
		return true;
	else 
		return false;
}

int ProgramState::getValue(string variableName) {
	// if doesn't have the variable return 0
	if (!this->containsVariable(variableName)) return 0;
	return (*varMap)[variableName];
}

void ProgramState::setValue(string variableName, int val) {
	(*varMap)[variableName] = val;
}

void ProgramState::printAll(std::ostream &outf) {
	map<string, int>::iterator it;
	for (it = varMap->begin(); it != varMap->end(); ++it)
	{
		outf << it->first << " " << it->second << std::endl;
	}
}

void ProgramState::pushLine(int line) {
	programStack->push(line);
}

int ProgramState::popLine() {
	if ( programStack->empty() ) {
		return -1;
	}

	int temp = programStack->top();
	programStack->pop();
	return temp;
}