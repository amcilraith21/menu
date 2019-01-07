#include "menu.h"
#include <string>
#include <vector>

/*
TODO:
Cut down on data size
Make custom string class
Debug further
Make the functions more efficient
*/

using namespace std;

int main(){
	menu<string> myMenu = { "this", "that", "the other" };
	myMenu.setTitle("Hello World!");
	myMenu.colorScheme(15,0, 15, 0);
	myMenu.setBrackets("  ", "", "->", "<-");
	s tringanswer;
	answer = myMenu.at(~myMenu);
	myMenu >> answer;
	cout << answer << endl;
	cout << ~myMenu << endl;

	return 0;
}