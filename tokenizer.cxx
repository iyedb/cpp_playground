#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <ctype.h>

using namespace std;

enum TokenType {T_NONE = 0, T_ELEM = 1, T_SPACE = 2, T_COLON = 3, T_EOL = 4, T_EOF = 5, T_BAD = 6};

string TokenTypetoString(TokenType type)
{
	switch(type)
	{
		case T_NONE:
			return string("T_NONE");
			break;
		case T_ELEM:
			return string("T_ELEM");
			break;
		case T_SPACE:
			return string("T_SPACE");
			break;
		case T_COLON:
			return string("T_COLON");
			break;
		case T_EOL:
			return string("T_EOL");
			break;
		case T_EOF:
			return string("T_EOF");
			break;
		default:
			return string("T_BAD");
	}
}

class Token
{

public:

	Token(string tok, TokenType type):_tok(tok), _type(type)
	{

	}
	

	Token():_tok(""),_type(T_NONE)
	{

	}

	Token(const Token& other)
	{
		_tok  = other.value();
		_type = other.type();
	}

	Token& operator=(const Token& other)
	{
		_tok = other.value();
		_type = other.type();
	}

	string value() const
	{
		return _tok;
	}

	TokenType type() const
	{
		return _type;
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


ostream & operator << (ostream& os, const Token& t)
{
	os << "'" << t.value() << "'" << "->" << TokenTypetoString(t.type()) << endl;
	return os;
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



class Node
{
public:
	Node(Token tok);
	~Node();
	void add_child(Node*);
	void add_parent(Node*);
	void add_next_sibling(Node*);
	void add_prev_sibling(Node*);
	const Token& get_data() const;
	const Token& get_data();
	
	/* data */
private:

	Token data;



	Node *child;
	Node *parent;
	Node *next_sibling;
	Node *prev_sibling;
};


Node::Node(Token tok)
{

	data = tok;
	child = NULL;
	parent = NULL;
	next_sibling = NULL;
	prev_sibling = NULL;
}

void Node::add_child(Node* n)
{
	child = n;
}

void Node::add_parent(Node* n)
{
	parent = n;
}

void Node::add_next_sibling(Node* n)
{
	next_sibling = n;
}

void Node::add_prev_sibling(Node* n)
{
	prev_sibling = n;
}


const Token& Node::get_data()
{
	return data;
}


const Token& Node::get_data() const
{
	return data;
}

Token get_next_token(std::list<Token>& token_list)
{
	Token tok = token_list.front();
	token_list.pop_front();
	return tok;
}




Node* build_tree(std::list<Token>& token_list)
{

	Node *node = NULL;
	Token tok;

	if(token_list.empty())
	{
		return NULL;
	}

	do
	{
		tok = get_next_token(token_list);	
	}
	while( tok.type() != T_ELEM && tok.type() != T_COLON && !token_list.empty() );
	
	if (tok.type() == T_ELEM)
	{
		node = new Node(tok);
		tok = get_next_token(token_list);

		if(tok.type() == T_COLON)
		{
			Node* child = build_tree(token_list);
			if (child != NULL)
			{
				child->add_parent(node);
			}
			node->add_child(child);	
		}
		else
		{
			token_list.push_front(tok);
			Node* next_sibling = build_tree(token_list);
			if (next_sibling != NULL)
			{
				next_sibling->add_prev_sibling(node);
			}

			node->add_next_sibling(next_sibling);
		}	
		
	}
	return node;
}

class TmlDoc
{
public:
	TmlDoc(string file_name);
	~TmlDoc();

private:
	Token get_next_token();
	Node* build_tree();
	Token get_next_token(ifstream& file_in);

	/* data */
	std::list<Node> token_list;
	Node* root;
};


int main(int argc, char const *argv[])
{
	
	std::vector<string> t;

	ifstream file_in(argv[1]);
	
	if(!file_in.good())
	{
		cout << "bad argument!" << endl;
		return -1;
	}

	Token token;
	std::list<Token> token_list;
	do
	{
		token = get_next_token(file_in);
		if(token.type() != T_BAD)
			token_list.push_back(token);
		else
			break;
	}
	while(token.type() != T_EOF);
	
	token_list.pop_back();
	
	Node* tree = NULL;

	tree = build_tree(token_list);

	return 0;


}



