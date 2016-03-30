#include "MathStatement.h"

MathStatement::MathStatement(std::string variableName, int val)
: m_variableName( variableName ), m_value( val ), m_variableNameRHS( "" )
{}

MathStatement::MathStatement(std::string variableName, std::string variableNameRHS)
: m_variableName( variableName ), m_value( 0 ), m_variableNameRHS( variableNameRHS )
{}
// mind the MULT/DIV cases, where m_value should be set to 1 after constructed