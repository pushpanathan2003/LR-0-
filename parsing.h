#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<char > stack;
int top=0;
int counter = 0;

//function to validate the given input contain only terminals of the production rules
bool input_validation ( string input ) {
	for ( int i=0; i<input.length(); i++ ) {
		bool flag = false;
		for ( int j=0; j<terminal.size(); j++ ) {
			if ( input[i] == terminal[j] ) {
				flag = true;
			}
		}
		if ( !flag )
			return false;
	}

	return true;
}

//function to find all the parse tree rules
void parse ( string input ) {
	string symbol = "";
	vector<string > rules;	//vector to store all the parsing rules

	input = input + "$";
	stack.push_back ( '0' );
	
	int i=0;
	while ( true ) {
		string temp = m2[stack[top]][input[i]];

		if ( temp == "-" ) {
			cout << "\n------------------------------" << endl;
			cout << "We were on a break" << endl;
			cout << "------------------------------\n" << endl;
			break;
		}

		if ( temp == "accept" ) {
			cout << "\n-------------------------------" << endl;
			cout << "I'm Chandler" << endl;
			cout << "-------------------------------\n" << endl;

			cout << "Parse Tree : " << endl;
			for ( int i=0; i<rules.size(); i++ ) {
				cout << rules[i] << endl;
			}
			break;
		}
			
		if ( temp[0] != 'r' ) {
			stack.push_back ( temp[0] );
			top++;
		
			string s(1, input[i]);
			symbol = symbol + s;
			i++;
		}
		else {
			int t = temp[1] - '0';
			string r = automata[t][0];

			rules.push_back ( automata[t][0] );
			
			string lhs = "";
			int i = 0;
			//copying the lhs of rule
			while ( r[i] != '=' ) {
				string s(1, r[i]);
				lhs = lhs + s;
				i++;
			}
			i++;
			
			//copying the rhs of rule
			string rhs = "";	
			while ( r[i] != '.' ) {
				string s(1, r[i] );
				rhs = rhs + s;
				i++;
			}
			
			int j = symbol.length()-1;
			bool flag = true;
			for ( int i=rhs.length()-1; i>=0; i-- ) {
				if ( rhs[i] != symbol[j] ) {
					flag = false;
				}
				j--;
			}
				
			if ( flag ) {
				string s1 = "";
				for ( int i=0; i<symbol.length()-rhs.length(); i++ ) {
					string s(1, symbol[i]);
					s1 = s1 + s;
				}
				symbol = s1 + lhs;
				
				for ( int i=0; i<rhs.length(); i++ ) {
					stack.erase ( stack.begin() + top );
					top--;
				}
				string s2 = m2[stack[top]][lhs[0]];
				stack.push_back ( s2[0]  );
				top++;
			
			}

		}
	}

}
