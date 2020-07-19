#include <iostream>
#include <string>
#include <vector>

#include "operations.h"
#include "parsing.h"

using namespace std;

int main() {
	int n;
	string input;

	cout << "Enter the no of production rules : ";
	cin >> n;

	cout << "\nEnter the production rules : " << endl;
	cout << "(eg: S=AB)\n" << endl;
	for ( int i=0; i<n; i++ ) {
		string temp;
		cin >> temp;
		production.push_back ( temp );
	}
	
	build_lr0_automata();	//function to build the lr(0) automata

	cout << "\nEnter the input word to parse : ";
	cin >> input;	

	if ( input_validation( input ) ) {	//function to validate the input word, checks only all the symbols are terminals of production rules
		parse ( input );
	}
	else
		cout << "Invalid input" << endl;

	return 0;
}
