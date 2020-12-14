#include <string>
#include <stack>
#include <iostream>
#include <vector>


enum Type { number, operation, openbracket, closebracket };

class Term {
protected:
	Type type;
public:
	Type GetType() {
		return type;
	}
	virtual void Print() = 0;
};

class Number : public Term {
public:
	Number(double numb) {
		type = Type::number;
		value = numb;
	}
	double value;
	void Print() { std::cout << value; }
};

class Operation : public Term {
public:
	char oper;
	Operation(char op) {
		type = Type::operation;
		oper = op;
	}
	int Priority() {
		if (oper == '+' || oper == '-') return 0;
		else return 1;
	}
	
	void Print() { std::cout << oper;}
};

class OpenBracket : public Term {
public:
	OpenBracket() {	type = Type::openbracket; }
	void Print() { std::cout << '('; }
};

class CloseBracket : public Term {
public:
	CloseBracket() { type = Type::closebracket; }
	void Print() { std::cout << ')'; }
};

class Arithmetic_expressions {
protected:
	std::string s;
	std::vector<Term*> terms;
	std::vector<Term*> pos_not;
	double result;

	bool lexicalAnalysis() {
		std::string str = s;
		bool correct = true;
		while (str.length() > 0) {
			if (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/') {
				terms.push_back(new Operation(str[0]));
				str.erase(0, 1);
			}
			else if (str[0] == '(') {
				terms.push_back(new OpenBracket);
				str.erase(0, 1);
			}
			else if (str[0] == ')') {
				terms.push_back(new CloseBracket);
				str.erase(0, 1);
			}
			else if (str[0] >= '0' && str[0] <= '9') {
				size_t numLength;
				terms.push_back(new Number(std::stod(str, &numLength)));
				str.erase(0, numLength);
			}
			else {
				correct = false;
				break;
			}
		}
		return correct;
	}


	bool syntaxAnalysis() {
		size_t num_brackets = 0;
		for (size_t i = 0; i < terms.size() - 1; i++) {
			Type term = terms[i + 1]->GetType();
			switch (terms[i]->GetType()) {
			case Type::openbracket:
			{
				if (term == closebracket || term == operation)
					return false;
				else num_brackets++;
				break;
			}

			case Type::closebracket:
			{
				if (term == openbracket || term == number)
					return false;
				else if (--num_brackets < 0)
					return false;
				break;
			}

			case Type::number:
			{
				if (term == openbracket || term == number)
					return false;
				break;
			}

			case Type::operation:
			{
				if (term == operation || term == closebracket)
					return false;
				break;
			}
			}
		}

		if (terms[terms.size() - 1]->GetType() == openbracket || \
			terms[terms.size() - 1]->GetType() == operation)
			return false;
		if (terms[terms.size() - 1]->GetType() == closebracket) --num_brackets;
		if (num_brackets) return false;

		return true;
	}

	void toPolish() {
		std::stack<Term*> termStack;
		for (int i = 0; i < terms.size(); i++) {
			switch (terms[i]->GetType()) {
			case openbracket:
				termStack.push(terms[i]);
				break;
			case closebracket:
				while (termStack.top()->GetType() != openbracket) {
					pos_not.push_back(termStack.top());
					termStack.pop();
				}
				termStack.pop();
				break;
			case number:
				pos_not.push_back(terms[i]);
				break;
			case operation:
				if (termStack.empty()) termStack.push(terms[i]);
				else if (termStack.top()->GetType() != operation) termStack.push(terms[i]);
				else {
					while (!termStack.empty() && termStack.top()->GetType() == operation && ((Operation*)(termStack.top()))->Priority() >= ((Operation*)terms[i])->Priority()) {
						pos_not.push_back(termStack.top());
						termStack.pop();
					}
					termStack.push(terms[i]);
				}
				break;
			}
		}
		while (!termStack.empty()) {
			pos_not.push_back(termStack.top());
			termStack.pop();
		}
	}
	bool calculation() {
		std::stack<double> num_Stack;
		for (int i = 0; i < pos_not.size(); i++) {
			switch (pos_not[i]->GetType()) {
			case number:
				num_Stack.push(((Number*)(pos_not[i]))->value);
				break;
			case Type::operation:
				double oper2 = num_Stack.top();
				num_Stack.pop();
				double oper1 = num_Stack.top();
				num_Stack.pop();
				switch (((Operation*)(pos_not[i]))->oper) {
				case '+':
					num_Stack.push(oper1 + oper2);
					break;
				case '-':
					num_Stack.push(oper1 - oper2);
					break;
				case '*':
					num_Stack.push(oper1 * oper2);
					break;
				case '/':
					if (oper2 == 0.0) throw "Division by zero";
					num_Stack.push(oper1 / oper2);
					break;
				}
				break;
			}
		}
		result = num_Stack.top();
		return true;
	}
public:
	Arithmetic_expressions(std::string str) {
		s = str;
	}
	~Arithmetic_expressions() {
		for (int i = 0; i < terms.size(); i++)
			delete terms[i];
	}
	double solve() {
		if (!lexicalAnalysis()) throw "Error in lexical analysis";
		if (!syntaxAnalysis()) throw "Error in syntax analysis";
		toPolish();
		if (!calculation())
			throw "Error";
		return result;
	}
};