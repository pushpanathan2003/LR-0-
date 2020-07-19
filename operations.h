#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<string > production;
vector<char > nonterminal;
vector<char > terminal;
vector<string > augumented;
vector<vector<string> > automata;
vector<int > rules_index;

typedef unordered_map<char, string> map1;
typedef unordered_map<char, map1> map2;
map2 m2;	//lr0 automata table


//move the dot after reading the current rhs symbol
string move_point ( string s ) {
	string temp = "";
	for ( int i=0; i<s.length(); i++ ) {
		string s1(1, s[i]);
		if ( s1 == "." ) {
			i++;
			string s1(1, s[i]);
			temp = temp + s1;
			temp = temp + ".";
		}
		else {
			temp = temp + s1;
		}
	}

	return temp;
}


//function to check all the symbols after dot
vector<int > check_symbol ( vector<string> temp, char symbol, int i ) {
	vector<int > index;

	for ( ; i<temp.size(); i++ ) {
		int j=2;
		while ( temp[i][j] != '.' ) {
			j++;
		}
		j++;
		if ( temp[i][j] == symbol ) {
			index.push_back ( i );
		}
	}

	return index;
}

//function to check is there any non-terminal after dot in the node
void check_non_terminal ( vector<string > &node, int size ) {

	for ( int i=0; i<size; i++ ) {
		int j=0;
	        int count=0;
		while ( node[i][j] != '.' )
			j++;
		j++;
			
		for ( int k=1; k<augumented.size(); k++ ) {
			if ( node[i][j] == augumented[k][0] ) {
				bool flag = true;
				
				//checking whether the rule is already presenet there in the node
				for ( int l=0; l<size; l++ ) {
					if ( augumented[k] == node[l] ) {
						flag = false;
						break;
					}
				}
				//adding the rule only if it is not present in the node previously
				if ( flag ) {
					node.push_back ( augumented[k] );
					size++;
				}
			}
		}
	}

}		

int check_for_existence ( vector<string > node ) {
	for ( int i=1; i<automata.size(); i++ ) {
	       if ( node[0] == automata[i][0] )
		       return i;
	}

	return 0;
}

void build_lr0_automata() {

	//creating a vector of non-terminals
	for ( int i=0; i<production.size(); i++ ) {
		bool flag = true;
		for ( int j=0; j<i; j++ ) {
			if ( production[i][0] == production[j][0] ) {
				flag = false;
				break;
			}
		}
		if ( flag ) 
			nonterminal.push_back ( production[i][0] );
	}
	
	//Creating augumented grammar
	string s(1, production[0][0]);
	augumented.push_back ( s + "'=." + s );
	for ( int i=0; i<production.size(); i++ ) {
		string s(1, production[i][0]);
		s = s + "=.";
		if ( production[i][2] == 'e' )
			augumented.push_back ( s );

		else {
			for ( int j=2; j<production[i].size(); j++ ) {
				string s1(1, production[i][j]);
				s = s + s1;
			}
			augumented.push_back ( s );
		}
	}
	
	//adding the augumented grammar as the node 0
	automata.push_back ( augumented );
	
	vector<vector<char > > table_symbols;

	int pointer = 0;

	while ( pointer != automata.size() ) {
	
		vector<string > temp = automata[pointer];
		vector<char > temp_table_symbols;
	
		for ( int i=0; i<temp.size(); i++ ) {
			if ( temp[i][0] != '~' ) {
				vector<string > temp1;
				vector<int > index;
				char symbol;
				
				int t=0;
				while ( temp[i][t] != '.' ) {
					t++;
				}
				t++;
					
				symbol = temp[i][t];

				if ( t == temp[i].length() ) {
					if ( pointer != 1 ) {
						rules_index.push_back ( pointer );
					}
					continue;
				}
				
				if ( t == temp[i].size() and temp.size()==1 ) {
					break;
				}

				
				//checking all the productions which has the same symbol after dot
				index = check_symbol ( temp, symbol, i );
				
				if ( index.size() > 0 ) {
					for ( int j=0; j<index.size(); j++ ) {
						string next = move_point ( temp[index[j]] );
						temp1.push_back ( next );
						temp[index[j]] = "~";
					}
				}
	
				
				//Check is there any non-terminal after dot
				check_non_terminal ( temp1, temp1.size() );

				//cout << "symbol : " << symbol << endl;
				temp_table_symbols.push_back ( symbol );
				
				//Check whether the formed node is already existing
				int node_index = check_for_existence ( temp1 );
				char p = pointer + '0';
				
				if ( node_index != 0 ) {
					m2[p][symbol] = to_string( node_index );
				}
				
				else {
					m2[p][symbol] = to_string( automata.size() );
					
					automata.push_back ( temp1 );
				}
				
			}
		}

		table_symbols.push_back ( temp_table_symbols );

		pointer++;
	}

	m2['1']['$'] =  "accept";	//adding the accept condition
	table_symbols[1].push_back ( '$' );	

	//collecting all the terminal symbols from the production rules
	for ( int i=0; i<table_symbols.size(); i++ ) {
		for ( int j=0; j<table_symbols[i].size(); j++ ) {
			int k=0;
			bool flag = true;;
			//checking whether the current symbol is already present in the terminal vector
			while ( k != terminal.size() ) {
				if ( terminal[k] == table_symbols[i][j] ) {
					flag = false;
					break;
				}
				k++;
			}
			if ( flag ) {
				bool flag1 = true;
				//checking whether the current symbol is an non-terminal
				for ( int k=0; k<nonterminal.size(); k++ ) {
					if ( nonterminal[k] == table_symbols[i][j] ) {
						flag1 = false;
						break;
					}
				}
				//if it is not an non-terminal, then adding it to the terminal vector
				if ( flag1 ) {
					terminal.push_back ( table_symbols[i][j] );
				}
			}
		}
	}	
	
	terminal.push_back ( '$' );

	//naming the table values as rules for end nodes
	for ( int i=0; i<rules_index.size(); i++ ) {
		string range = to_string ( nonterminal.size() + terminal.size() );

		char t = rules_index[i] + '0';
		string temp = "r" + to_string(rules_index[i]);

		for ( int j=0; j<nonterminal.size(); j++ ) {
			if ( m2[t][nonterminal[j]] >= "0" and m2[t][nonterminal[j]] <= range ) {
				cout << "\n-----------------------------------------" << endl; 
				cout << "The given grammar is not a LR(0) grammar" << endl;
				cout << "-----------------------------------------\n" << endl;
				exit(EXIT_FAILURE);
			}
			else
				m2[t][nonterminal[j]] = temp;
		}
		for ( int j=0; j<terminal.size(); j++ ) {
			if ( m2[t][terminal[j]] >= "0" and m2[t][terminal[j]] <= range ) {
				cout << "\n-----------------------------------------" << endl;
				cout << "The given grammar is not a LR(0) grammar" << endl;
				cout << "-----------------------------------------\n" << endl;
				exit(EXIT_FAILURE);
			}
			else
				m2[t][terminal[j]] = temp;
		}
	}

	for ( int i=0; i<nonterminal.size(); i++ ) {
		for ( int j=0; j<table_symbols.size(); j++ ) {
			char c = j + '0';
			bool flag = true;

			for ( int k=0; k<table_symbols[j].size(); k++ ){
				if ( nonterminal[i] == table_symbols[j][k] ) {
					flag = false;
					break;
				}
			}
			if ( flag ) {
				m2[c][nonterminal[i]] = "-";
			}
		}
	}

	for ( int i=0; i<terminal.size(); i++ ) {
		for ( int j=0; j<table_symbols.size(); j++ ) {
			bool flag1 = true;

			for ( int l=0; l<rules_index.size(); l++ ) {
				if ( j == rules_index[l] ) {
					flag1 = false;
					break;
				}
			}

			if ( flag1 ) {	
				char c = j + '0';
				bool flag = true;
	
				for ( int k=0; k<table_symbols[j].size(); k++ ) {
					if ( terminal[i] == table_symbols[j][k] ) {
						flag = false;
						break;
					}
				}
				if ( flag ) {
					m2[c][terminal[i]] = "-";
				}
			}
		}
	}	



	
}
