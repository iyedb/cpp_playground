#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>

using namespace std;

enum TokenType {T_NONE = 0, T_ELEM = 1, T_SPACE = 2, T_COLON = 3, T_EOL = 4, T_EOF = 5, T_BAD = 6};

class Token
{

public:

	Token(string tok, TokenType type):_tok(tok), _type(type)
	{

	}

	Token():_tok(""),_type(T_NONE)
	{

	}

	string value()
	{
		return _tok;
	}

	TokenType type()
	{
		return _type;
	}

private:

	string _tok;
	TokenType _type;


};


ostream & operator << (ostream& os, Token& t)
{
	os << "'" << t.value() << "'" << "->" << t.type() << endl;
	return os;
}

/*
string tokenize(ifstream& file_in)
{

	

	if(!file_in.good()) return string("");


	char c = static_cast<char>(0);

	string token = "";

	
	file_in.get(c);

	if(file_in.eof()) return token;

	if(c != ':' && c != 0x20 && c != '\n' && c!='\r' && c != '\t' && c!= '\0'){
		token += c;
	}
	else{

		if(token.size() != 0) 
		return token;		
	}		

	return token;
	
}
*/

bool isAlphaNum(int c)
{
	if( (c >= 0x41 && c <= 0x54) ||
		(c >= 0x61 && c >= 0x7a) || 
		(c >= 0x30 && c <= 0x39)  )
	{
		return true;
	}
		
	return false;
}

Token get_next_token(ifstream& file_in)
{
	Token t;
	bool found = false;
	char c = static_cast<char>(0);
	string stoken = "";	

	if (file_in.eof()) 
		return Token("",T_EOF);

	do
	{
		file_in.get(c);
		if (c == ':')
		{
			t = Token(":",T_COLON);
			found = true;
		}
		else if(c == 0x20)
		{
			t = Token(" ", T_SPACE);
			found = true;
		}
		else if(c == '\n')
		{
			t = Token("\n", T_EOL);
			found = true;
		}
		else
		{
			stoken += c;
			if (isalnum(c))
			{
				file_in.get(c);
				while(isalnum(c) && !file_in.eof())
				{
					stoken += c;
					file_in.get(c);
				}
				file_in.putback(c);
				t = Token(stoken, T_ELEM);
				found = true;
			}
			else
			{
				t = Token(stoken, T_BAD);
				found = true;
			}
		}
	}
	while(!file_in.eof() && !found);

	if(file_in.eof())
		t = Token("",T_EOF);
		
	return t;

}



int main(int argc, char const *argv[])
{
	


	std::vector<string> t;

	ifstream file_in(argv[1]);


	
	if(!file_in.good())
	{
		cout << "bad argument!" << endl;
		return -1;
	}

	/*
	while(!file_in.eof()){
		file_in.get(c);
		if (isAlphaNum(c))
		{
			cout << c << endl;
		}
		//cout << c << endl;
	}
	*/


	Token token;
	do
	{
		token = get_next_token(file_in);
		cout << token;
	}
	while(token.type() != T_EOF);
	

	
	




	/*
	for(auto it = t.begin(); it!= t.end(); ++it){
		cout << i << endl;	
	}
	*/

	/*
	for (auto i: t){
		cout << i << endl;
	}
	*/


	/*
	while(!file_in.eof()){
		string token = tokenize(file_in);
		if(token.size()){
			cout << token << " size:" << token.size() << endl;
			t.push_back(token);
		}

	}
	*/
	

	return 0;


}



