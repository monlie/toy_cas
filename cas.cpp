#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <map>
#include <boost/regex.hpp>

using namespace std;
typedef map<string, string> STR_TO_STR;
typedef map<string, int> STR_TO_INT;
typedef map<string, float> STR_TO_F;
typedef map<int, int> INT_TO_INT;
typedef float (*uFunc) (float a);
typedef float (*biFunc) (float a, float b);


STR_TO_INT priority = { { "abs", 2 },
					    { "exp", 2 },
					    { "sin", 2 },
					    { "cos", 2 },
			            { "+", 0 },
					    { "-", 0 },
					    { "*", 1 },
					    { "/", 1 } };

STR_TO_STR funcMap = { { "abs", "uFunc" },
	                   { "exp", "uFunc" },
					   { "sin", "uFunc" },
					   { "cos", "uFunc" },
                       { "+", "biFunc" },
					   { "-", "biFunc" }, 
					   { "*", "biFunc" }, 
					   { "/", "biFunc" } };
boost::regex num("^[\\d\\.]+$");
boost::regex var_name("^\\w+$");


class node
{
private:
	string type;
	string value;
	vector<node> ch;
public:
	node(string n_type, string n_value, vector<node> n_ch = vector<node>())
	{
		type = n_type;
		value = n_value;
		ch = n_ch;
	}

	void* getFunc()
	{
		if (value == "+")
		{
			biFunc bi_pr = [](float a, float b)->float {return a + b;};
			return bi_pr;
		}
		if (value == "-")
		{
			biFunc bi_pr = [](float a, float b)->float {return a - b;};
			return bi_pr;
		}
		if (value == "*")
		{
			biFunc bi_pr = [](float a, float b)->float {return a * b;};
			return bi_pr;
		}
		if (value == "/")
		{
			biFunc bi_pr = [](float a, float b)->float {return a / b;};
			return bi_pr;
		}
		if (value == "exp")
		{
			uFunc u_pr = exp;
			return u_pr;
		}
		if (value == "abs")
		{
			uFunc u_pr = abs;
			return u_pr;
		}
		if (value == "sin")
		{
			uFunc u_pr = abs;
			return u_pr;
		}
		if (value == "cos")
		{
			uFunc u_pr = abs;
			return u_pr;
		}
	}

	float calc()
	{
		if (type == "const") return stof(value);
		if (funcMap[value] == "uFunc") return ((uFunc)getFunc())(ch[0].calc());
		if (funcMap[value] == "biFunc") return ((biFunc)getFunc())(ch[0].calc(), ch[1].calc());
	}

	float calc(STR_TO_F value_dict)
	{
		if (type == "const") return stof(value);
		if (type == "var") return value_dict[value];
		if (funcMap[value] == "uFunc") return ((uFunc)getFunc())(ch[0].calc(value_dict));
		if (funcMap[value] == "biFunc") return ((biFunc)getFunc())(ch[0].calc(value_dict), ch[1].calc(value_dict));
	}
};

node parse(string formula)
{
	string cha;
	cout << formula << endl;
	if (boost::regex_match(formula, num)) return node("const", formula);
	if (boost::regex_match(formula, var_name)) return node("var", formula);
	
	stack<int> pare_stack;
	INT_TO_INT pare_dict;

	for (int i = 0; i < formula.length(); i++)
	{
		if (formula[i] == '(') pare_stack.push(i);
		if (formula[i] == ')')
		{
			pare_dict[pare_stack.top()] = i;
			pare_stack.pop();
		}
	}
	if (pare_dict.count(0) > 0 && pare_dict[0] == formula.length() - 1)
	{
		formula = formula.substr(1, formula.length() - 2);
	}
	vector<pair<int, string>> op_list;
	for (int i = 0; i < formula.length(); i++)
	{
		if (formula[i] == '(') pare_stack.push(i);
		if (formula[i] == ')')
		{
			pare_stack.pop();
		}
		if (pare_stack.empty())
		{
			stringstream stream;
			stream << formula[i];
			cha = stream.str();
			if (priority.count(cha) > 0)
			{
				op_list.push_back(pair<int, string>(i, cha));
			}
		}
		sort(op_list.begin(), op_list.end(),
			[](pair<int, string> a, pair<int, string> b)->bool {return priority[a.second] < priority[b.second];}); 
	}
	cout << op_list[0].second << endl;
	vector<node> ch;
	ch.push_back(parse(formula.substr(0, op_list[0].first)));
	ch.push_back(parse(formula.substr(op_list[0].first + 1, formula.length() - op_list[0].first - 1)));
	//return node("const", "5");
	return node("op", op_list[0].second, ch);
}

int main()
{
	/*node n("const", "5");
	node m("const", "2");
	vector<node> child;
	child.push_back(n);
	child.push_back(m);
	node k("op", "+", child);
	node h("op", "exp", vector<node>(1, k));*/
	STR_TO_F dic = { {"x", 3551} };
	string str;
	cout << "pls enter your formula:" << endl;
	cin >> str;
	cout << parse(str).calc(dic) << endl;
	system("pause");
	return 0;
}