#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

vector<string> read_file(string input_file,int x) {
	vector<string> lst;
	ifstream s;
	string temp;
	s.open(input_file);
	try {
		if (!s.is_open())
		{
			throw "Can't open " + input_file;
			return lst;
		}
	}
	catch (const char* invalid_argument) {
		cout << invalid_argument << endl;
	}
	for(int i=0;i<x;i++)
	{
		getline(s, temp);
		stringstream check(temp);
		string intermediate;
		while (getline(check, intermediate, ' '))
		{
			//Split (,[,{ and the number
			if (intermediate.substr(0, 1) == "("|| intermediate.substr(0, 1) == "["|| intermediate.substr(0, 1) == "{")
			{
				lst.push_back(intermediate.substr(0, 1));
				lst.push_back(intermediate.substr(1, intermediate.length()-1));
				continue;
			}
			//Split ),],] and the number
			if (intermediate.substr(intermediate.length() - 1, 1) == ")" || intermediate.substr(intermediate.length() - 1, 1) == "]" || intermediate.substr(intermediate.length() - 1, 1) == "}")
			{
				lst.push_back(intermediate.substr(0, intermediate.length() - 1));
				lst.push_back(intermediate.substr(intermediate.length() - 1,1));
				continue;
			}
			lst.push_back(intermediate);
		}
		//Signal the end of an exp
		lst.push_back("\n");
	}
	s.close();
	return lst;
}

bool isOperator(string element)
{
	if (element == "+" || element == "-" || element == "*" || element == "/" || element == "^")
	{
		return true;
	}
	return false;
}

bool isLeftParenthesis(string element)
{
	if (element == "(" || element == "{" || element == "[")
	{
		return true;
	}
	return false;
}

bool isRightParenthesis(string element)
{
	if (element == ")" || element == "}" || element == "]")
	{
		return true;
	}
	return false;
}

bool hasSameOrHigherPrecedence(string infixOperator, string stackOperator)
{
	if (stackOperator == "^" || infixOperator == "+" || infixOperator == "-")
	{
		return true;
	}
	if ((stackOperator == "*" || stackOperator == "/") && infixOperator != "^")
	{
		return true;
	}
	return false;
}

bool isOperand(string element)
{
	if (isLeftParenthesis(element) || isRightParenthesis(element) || isOperator(element))
	{
		return false;
	}
	return true;
}

bool isValidOperand(string operand)
{
	if (operand[0] == '.')
	{
		return false;
	}
	bool isFloat = false;
	for (int i = 0; i < operand.length(); i++)
	{
		if (operand[i] != '0' && operand[i] != '1' && operand[i] != '2' && operand[i] != '3' && operand[i] != '4' && operand[i] != '5' && operand[i] != '6' && operand[i] != '7' && operand[i] != '8' && operand[i] != '9' && operand[i] != '.')
		{
			return false;
		}
		if (operand[i] == '.')
		{
			if ((operand.length() - i) > 3 || (operand.length() - i) < 2)
			{
				return false;
			}
			if (isFloat)
			{
				return false;
			}
			else
			{
				isFloat = true;
			}
		}
	}
	return true;
}

bool isValidExpression(vector<string> infix)
{
	int leftCount = 0, rightCount = 0;
	if (isOperator(infix.back()) || isLeftParenthesis(infix.back()))
	{
		return false;
	}
	for (int i = 0; i < infix.size() - 1; i++)
	{
		if (isOperand(infix.at(i)))
		{
			if (!isValidOperand(infix.at(i)))
			{
				return false;
			}
			if (isOperand(infix.at(i + 1)) || isLeftParenthesis(infix.at(i + 1)))
			{
				return false;
			}
		}
		if (isOperator(infix.at(i)) && (isOperator(infix.at(i + 1)) || isRightParenthesis(infix.at(i + 1))))
		{
			return false;
		}
		if (isLeftParenthesis(infix.at(i)))
		{
			if (isOperator(infix.at(i + 1)) || isRightParenthesis(infix.at(i + 1)))
			{
				return false;
			}
			leftCount++;
		}
		if (isRightParenthesis(infix.at(i)))
		{
			if (isOperand(infix.at(i + 1)) || isLeftParenthesis(infix.at(i + 1)))
			{
				return false;
			}
			rightCount++;
		}
		if (rightCount > leftCount)
		{
			return false;
		}
	}
	if (!isValidOperand(infix.back()))
	{
		return false;
	}
	if (isRightParenthesis(infix.back()))
	{
		rightCount++;
	}
	if (rightCount != leftCount)
	{
		return false;
	}
	return true;
}

void infixToPostfix(vector<string> infix, vector<string> &postfix)
{
	vector<string> operatorStack;
	operatorStack.push_back("(");
	infix.push_back(")");
	for (int i = 0; i < infix.size(); i++)
	{
		if (isOperand(infix.at(i)))
		{
			postfix.push_back(infix.at(i));
		}
		else if (isOperator(infix.at(i)))
		{
			while (isOperator(operatorStack.back()) && hasSameOrHigherPrecedence(infix.at(i), operatorStack.back()))
			{
				postfix.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.push_back(infix.at(i));
		}
		else if (isLeftParenthesis(infix.at(i)))
		{
			operatorStack.push_back(infix.at(i));
		}
		else if (isRightParenthesis(infix.at(i)))
		{
			while (!isLeftParenthesis(operatorStack.back()))
			{
				postfix.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.pop_back();
		}
	}
}

float calculate(float firstOperand, float secondOperand, string op)
{
	if (op == "+")
	{
		return firstOperand + secondOperand;
	}
	if (op == "-")
	{
		return firstOperand - secondOperand;
	}
	if (op == "*")
	{
		return firstOperand * secondOperand;
	}
	if (op == "/")
	{
		return firstOperand / secondOperand;
	}
	if (op == "^")
	{
		return pow(firstOperand, secondOperand);
	}
}

float evaluateInfix(vector<string> infix)
{
	vector<string> operatorStack;
	vector<float>valueStack;
	string op;
	float firstOperand, secondOperand;
	// scan the infix expression
	for (int i = 0; i < infix.size(); i++)
	{
		// push operand to value stack if ecountered
		if (isOperand(infix.at(i)))
		{
			valueStack.push_back(stof(infix.at(i)));
		}
		// if encounter an operator
		else if (isOperator(infix.at(i)))
		{
			/* while operator stack is not empty, value stack has at least two elements 
			and the current operator of expression has equal or lower precedence
			than the top element of operator stack */
			while (!operatorStack.empty() && hasSameOrHigherPrecedence(infix.at(i), operatorStack.back()) && valueStack.size() >= 2)
			{
				/* process the top two elements in value stack and the top
				element of operator stack and push the result to value stack */
				op = operatorStack.back();
				operatorStack.pop_back();
				secondOperand = valueStack.back();
				valueStack.pop_back();
				firstOperand = valueStack.back();
				valueStack.pop_back();
				valueStack.push_back(calculate(firstOperand, secondOperand, op));
			}
			operatorStack.push_back(infix.at(i));
		}
		// push left parenthesis to operator stack if encountered
		else if (isLeftParenthesis(infix.at(i)))
		{
			operatorStack.push_back(infix.at(i));
		}
		else if (isRightParenthesis(infix.at(i)))
		{
			/* while the top element of operator stack is not a left
			parenthesis and value stack has at least two elements */
			while (!isLeftParenthesis(operatorStack.back()) && valueStack.size() >= 2)
			{
				op = operatorStack.back();
				operatorStack.pop_back();
				secondOperand = valueStack.back();
				valueStack.pop_back();
				firstOperand = valueStack.back();
				valueStack.pop_back();
				valueStack.push_back(calculate(firstOperand, secondOperand, op));
			}
			operatorStack.pop_back();
		}
	}
	// process the remaining operators in operator stack
	while (!operatorStack.empty() && valueStack.size() >= 2)
	{
		op = operatorStack.back();
		operatorStack.pop_back();
		secondOperand = valueStack.back();
		valueStack.pop_back();
		firstOperand = valueStack.back();
		valueStack.pop_back();
		valueStack.push_back(calculate(firstOperand, secondOperand, op));
	}
	// the last remaining element in the value stack is the final value of expression
	return valueStack.back();
}

void output_cal(string output_file, vector<string>& lst,int x)
{
	vector<string>temp;
	ofstream fout;
	fout.open(output_file);
	try {
		if (!fout.is_open())
		{
			throw "Can't open " + output_file;
			return;
		}
	}
	catch (const char* invalid_argument) {
		cout << invalid_argument << endl;
	}
	int j = 0;
	for (int i = 0; i < x; i++)
	{
		while (lst[j] != "\n")
		{
			temp.push_back(lst[j]);
			j++;
		}
		if (isValidExpression(temp))
		{
			fout << evaluateInfix(temp) << endl;
		}
		else
		{
			fout << "E" << endl;
		}
		j++;
		temp.clear();
	}
	fout.close();
}

void output_con(string output_file, vector<string>& lst, int x)
{
	ofstream fout;
	vector<string>in;
	vector<string>post;
	fout.open(output_file);
	try {
		if (!fout.is_open())
		{
			throw "Can't open " + output_file;
			return;
		}
	}
	catch (const char* invalid_argument) {
		cout << invalid_argument << endl;
	}
	int j = 0;
	for (int i = 0; i < x; i++)
	{
		while (lst[j] != "\n")
		{
			in.push_back(lst[j]);
			j++;
		}
		if (isValidExpression(in))
		{
			infixToPostfix(in, post);
			for (int k = 0; k < post.size();k++)
			{
				fout << post[k] << " ";
			}
			fout << endl;
		}
		else
		{
			fout << "E" << endl;
		}
		in.clear();
		post.clear();
		j++;
	}
	fout.close();
}

void output_main(string input_file, int x,string func,string output_file)
{
	vector<string> lst = read_file(input_file, x);
	if (func == "-c") output_cal(output_file, lst, x);
	else if (func == "-t") output_con(output_file, lst, x);
	else cout << "Error func" << endl;	
	cout << "Result saved in " << output_file << endl;
}

int main(int argc, char* argv[]) {
	string input_file,output_file; 
	int x;
	string func;
	if (argc > 3) {
		input_file = argv[1]; 
		x = atoi(argv[2]);
		func = argv[3];
		output_file = argv[4];
		output_main(input_file, x, func, output_file);
	}
	else {
		cout << "Enter input_file:"; cin >> input_file;
		cout << "Enter x:"; cin >> x;
		cout << "Enter func:"; cin >> func;
		cout << "Enter output_file:"; cin >> output_file;
	}
	output_main(input_file, x,func,output_file);
	if (!system(NULL)) system("pause"); return 0;
}