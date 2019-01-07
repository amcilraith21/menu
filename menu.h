/*
Author: Aiden McIlraith
Title: menu
Purpose: Implement an arrow key based menu
Last Modified: 9/5/2018
|--------------------------------------------------------------------------
| Description
|--------------------------------------------------------------------------
|
| This class is intended to be used for the creation of selectable menus in
| a UI. Its data is stored in a custom class that functions similarly to
| a vector. It can be initialized with initializer lists and is a template.
| The UI is highly customizable and can scroll. The class also can do
| concatonate with other menus and lists and functions similarly to a vector
| in many ways. 
|
*/
#ifndef MENU_H
#define MENU_H
#include <conio.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <initializer_list>

using std::string; 
using std::initializer_list;
using std::ostream;
using std::cout;
using std::endl;
using std::setw;

template<typename data_type>
class menu {
private:
	
	template<typename vector_data_type>
	class customVect {
	private:
		/*
		DATA
		
		vectData:
		* pointer to type T
		* used to point to an array in the heap which holds the data
		
		usedSize:
		* int which holds the length of used space in the array
		
		allocatedSize:
		* int which holds the allocated size of the array

		*/
		vector_data_type* vectData;
		unsigned int usedSize;
		unsigned int allocatedSize;


		/*
		PRIVATE FUNCTIONS
		
		doubleSize:
		* doubles the allocated size of the array
		* only called from push_back
		* takes no arguements and returns void

		*/
		void doubleSize() {
			vector_data_type* temp = new vector_data_type[allocatedSize * 2];
			for (int i = 0; i < allocatedSize; i++)
				temp[i] = vectData[i];
			delete[] vectData;
			vectData = temp;
			allocatedSize = allocatedSize * 2;
		}
	public:
		/*
		CONSTRUCTORS AND DESTRUCTORS

		~customVect:
		* deletes the array in the heap
		* takes no arguements

		customVect:
		* creates a new customVect
		* sets the default values
		* takes no arguements

		*/
		~customVect() {
			delete[] vectData;
		}
		customVect() {
			vectData = new vector_data_type[5];
			usedSize = 0;
			allocatedSize = 5;
		}
		/*
		PUBLIC FUNCTIONS

		size:
		* returns the usedSize of the customVect as an int
		* takes no arguements

		at:
		* returns by reference the data at the given location specified
			in the arguement of the function
		* takes one int as an arguement

		operator[]:
		* returns at at the given location by reference

		push_back:
		* adds the input to the back of vectData
		* adjusts allocation when needed
		* returns void
		* takes one T as an input

		clear:
		* deletes the data array and makes a new one of the same size
		* sets the usedSize to 0
		* takes no arguements and returns nothing

		erase:
		* deletes a range of values from the data
		* takes two int arguements
		* inclusive to the first location, exclusive of the last

		*/

		int size() const { return usedSize; }
		vector_data_type& at(int location) const { return *(vectData + location);	}
		vector_data_type& operator[](int location) { return at(location); }
		void push_back(const vector_data_type& input) {
			if (usedSize + 1 >= allocatedSize)
				doubleSize();
			vectData[usedSize] = input;
			usedSize++;
		}
		void clear() { 
			delete[] vectData;
			vectData = nullptr;
			vectData = new vector_data_type[allocatedSize];
			usedSize = 0;
		}
		void erase(int start, int finish) {
			vector_data_type* temp = new vector_data_type[allocatedSize];
			int writeLocation = 0;
			for (int readLocation = 0; readLocation < allocatedSize; readLocation++) {
				if ((readLocation >= finish) || (readLocation < start)) {
					temp[writeLocation] = vectData[readLocation];
					writeLocation++;
				}
			}
			delete[] vectData;
			vectData = temp;
			usedSize -= (finish - start);
		}
	};

	/*
	shortString
	* Used for a simplified string of 16 or less characters
	* can get size and length (same thing)
	* has = assignment operator
	*/
	class shortString {
	private:
		char data[10] = { 0,0,0,0,0,0,0,0,0,0 };
	public:
		shortString() {}
		shortString(string input) {
			for (int i = 0; i < input.length(); i++) {
				if (i >= 10)
					break;
				data[i] = input[i];
			}
		}
		shortString(const char* input) {
			for (int i = 0; i < 10; i++) {
				if (input[i] == char(0))
					break;
				data[i] = input[i];
			}

		}
		shortString(const shortString& input) {
			for (int i = 0; i < 10; i++) {
				this->data[i] = input.data[i];
			}
		}

		int length() { return size(); }

		int size() {
			for (int i = 0; i < 10; i++)
				if (data[i] == 0)
					return i;
			return 10;
		}

		void operator=(const shortString& input) {
			for (int i = 0; i < 10; i++)
				this->data[i] = input.data[i];
		}
		void operator=(const char* input) {
			shortString temp(input);
			operator=(temp);
		}
		void operator=(string input) {
			shortString temp(input);
			operator=(temp);
		}
		friend ostream& operator<<(ostream& os, const shortString& input)
		{
			for (int i = 0; i < 10; i++)
				if(input.data[i] != 0)
					os << input.data[i];
			return os;
		}
		
	};

	

	/*
	DATA SECTION

	data:
	* stores the values which are to be used in the menu
	* stored in a customVect data type which is defined above
	* stores data of the type of which the menu is
	* data is parallel to colors

	colors:
	* stores the color data
	* holds both cursor and non-cursor data
	* the first three digits are the default color
	* the next three digits are the cursor color
	* it is parallel to data

	scrollLength:
	* type int
	* stores how many lines of text are visible before having to scroll

	title:
	* defines the header of the menu
	* stored as a string

	titleColor:
	* stored as an unsigned char
	* holds the color of the title
	* effectively acts as a one bit int

	hasNumbering:
	* stored as a bool
	* determines whether the numbering appears when a menu is used

	begin,before,after,beforeCursor,afterCursor:
	* all are stored as strings
	* begin appears before the numbering
	* before appears between the numbering and the value when the cursor is not on the location
	* beforeCursor appears between the numbering and the value when the cursor is on the location
	* after appears after the value when the cursor is not on a location
	* afterCursor appears after the value when the cursor is on a location



	*/

	customVect<data_type> data;
	customVect<unsigned int> colors;

	unsigned char scrollLength = 10;

	string title = "";
	unsigned char titleColor;
	bool hasNumbering = true;
	shortString begin = "";
	shortString before = ") ";
	shortString after = "";
	shortString beforeCursor = ") ";
	shortString afterCursor = "";

	/*
	PRIVATE FUNCTIONS

	display
	* Returns void
	* This outputs the menu in a numbered list with the cursor showing at the inputted location
	* This also output the menu with the associated colors intact
	* takes two ints as the arguement. One is the current location of the cursor and the other is
		the location of the top of the scrolling part.

	moveCursor
	* Returns true if enter is pressed, otherwise it returns false
	* This function takes the variable that holds the cursor's location by reference
	* The function reads the keyboard input and changes the cursor's location

	setDisplayColor
	* Returns void
	* Takes an int which is converted in to a two-digit HEX value
	* Sets the screens output settings with a background and a foreground according to the input
		value.
	* The input value defaults to 15 which is white text on a black background

	makeChoice
	* returns the location of the user's choice as an int
	* This combines moveCursor and display to make a functioning UI

	cursorColor:
	* takes one int
	* returns the cursor color value at the given location

	defaultColor:
	* takes one int
	* returns the default color value at the given location

	*/
	void display(int cursorLocation, int scrollLocation)
	{
		system("cls");
		if (title.size()) {
			setDisplayColor(titleColor);
			cout << title << endl;
		}
		setDisplayColor();
		if (scrollLocation > 0)
			cout << "^" << endl;
		else 
			cout <<"-"<< endl;
		for (int i = scrollLocation; i < scrollLength + scrollLocation; i++) {
			if (i >= size())
				break;
			if (i == cursorLocation)
				setDisplayColor(cursorColor(i));
			else
				setDisplayColor(defaultColor(i));
			cout << begin;
			if (hasNumbering) {
				cout << setw(log(size())) << i + 1;
			}
			cout << ((i == cursorLocation) ? beforeCursor : before) << data.at(i) << ((i == cursorLocation) ? afterCursor : after) << endl;
			setDisplayColor();
			
			
		}
		if (scrollLength < size() - scrollLocation)
			cout << "v" << endl;
		else
			cout << "-" << endl;
	}

	bool moveCursor(int& cursorLocation) {

		switch (_getch()) {
		case 224: //ARROW KEY
			switch (_getch()) {
			case 72: //UP ARROW
				if (cursorLocation > 0)
					cursorLocation--;
				break;
			case 80: //DOWN ARROW
				if (cursorLocation < size() - 1)
					cursorLocation++;
				break;
			}
			break;
		case 13: //ENTER KEY
			return true;
			break;
		}

		return false;
	} 

	void setDisplayColor(int value = 15)const { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value); } 

	int makeChoice(){
		try {
			if (data.size() == 0) {
				throw(std::out_of_range("No data to pick from"));
			}
		}
		catch (std::out_of_range msg) {
			std::cerr << msg.what() << endl;
			return -1;
		}
		int scrollLocation = 0;
		int cursorLocation = 0;
		while (true) {
			display(cursorLocation, scrollLocation);
			if (moveCursor(cursorLocation))
				break;
			if (cursorLocation >= scrollLocation + scrollLength)
				scrollLocation++;
			if (cursorLocation < scrollLocation)
				scrollLocation--;
		}
		return cursorLocation;
	}

	int cursorColor(int location)  {
		return (colors.at(location) % 1000);
	}

	int defaultColor(int location) {
		return int(colors.at(location) / 1000);
	}

public:
	

	/*
	CONSTRUCTORS AND DESTRUCTORS
	* The destructor does nothing
	* the menu can also be constructed with an initializer list (e.g. {1,2,3})
	* all of the constructors call colorScheme to reset the colors
	* the copy constructor makes a new identical menu similarly to the operator= function
	*/
	~menu() {}
	menu() { colorScheme(); }
	menu(initializer_list<data_type> input) {
		for (int i = 0; i < input.size(); i++) {
			data.push_back(*(input.begin() + i));
		}	
		colorScheme();
	}
	menu(const menu<data_type>& input) {
		for (int i = 0; i < input.size(); i++)
		{
			this->data.push_back(input.data.at(i));
			this->colors.push_back(input.colors.at(i));
		}

		this->title = input.title;
		this->titleColor = input.titleColor;
		this->hasNumbering = input.hasNumbering;
		this->begin = input.begin;
		this->before = input.before;
		this->after = input.after;
		this->beforeCursor = input.beforeCursor;
		this->afterCursor = input.afterCursor;
	}

	/*
	COLOR SETTERS
	uniqueColor:
	* This function edits defaultColors and cursorColors at the given location. This determines the
		colors at any given point.
	* This returns void
	* All of it's inputs are int
	* It must take an input of a location in the data vector
	* The text and background inputs default to white on black, but may be specified. They control the 
		color of the text when the cursor is not on them.
	* The cursorText and cursorBackground inputs default to the background and text inputs respectively.
		They control what the color is when the cursor is on the given location.
	
	colorScheme:
	* These functions set a color for all of the pieces in a menu based on the given color inputs.
	* The functions return void. They all call the uniqueColor function eventually.
	* All of the inputs are ints.
	* The functions require no values by default. It defaults to white on black for default and
		the reverse coloring for the cursor location
	* The functions may take a text and background input. 
	* The functions may take a text and background and a cursorText and a cursorBackground input.

	setTitleColor:
	* This sets the color of the title.
	* The inputs default to white text on a black background

	COLOR LIST
	0 = Black       8 = Gray
	1 = Blue        9 = Light Blue
	2 = Green      10 = Light Green
	3 = Aqua       11 = Light Aqua
	4 = Red        12 = Light Red
	5 = Purple     13 = Light Purple
	6 = Yellow     14 = Light Yellow
	7 = White      15 = Bright White
	*/
	void uniqueColor(int location, int text, int background, int cursorText, int cursorBackground){
		colors.at(location) = int(text + (background * 16)) * 1000;
		colors.at(location) += int(cursorText + (cursorBackground * 16));
	}
	void uniqueColor(int location, int text, int background) { uniqueColor(location, text, background, background, text); }
	void uniqueColor(int location) { uniqueColor(location, 15, 0); }
	
	void colorScheme(int text, int background, int cursorText, int cursorBackground){
		colors.clear();
		for (int i = 0; i < size(); i++) {
			colors.push_back(0);
		}
		for (int i = 0; i < size(); i++)
			uniqueColor(i, text, background, cursorText, cursorBackground);
		setTitleColor(text, background);
	}
	void colorScheme(int text, int background) { colorScheme(text, background, background, text); }
	void colorScheme() { colorScheme(15, 0); }

	void setTitleColor(int text = 15,int background = 0) {
		titleColor = text + background * 16;
	}

	/*
	MISC SETTERS

	numberingOn:
	* sets hasNumbers to true

	numberingOff:
	* sets hasNumbers to false

	toggleNumbering:
	* takes one bool
	* sets hasNumbers to the input

	setBrackets:
	* takes up to four inputs. All are strings.
	* the order of inputs is before the default, after the default, before the cursor, after the cursor
	* the first two are independantly optional, but the last two must either both exist or neither exist 
		(e.g. there may be one, two, or four inputs)

	setBegin:
	* sets the string to display before the counters
	* defaults to ""


	*/
	void numberingOn() { toggleNumbering(true); }
	void numberingOff() { toggleNumbering(false); }
	void toggleNumbering(bool input) { hasNumbering = input; }

	void setBrackets(string before, string after, string beforeCursor, string afterCursor) {
		this->before = before;
		this->after = after;
		this->afterCursor = afterCursor;
		this->beforeCursor = beforeCursor;
	}
	void setBrackets(string before = ")", string after = "") { setBrackets(before, after, before, after); }
	void setBegin(string begin = "") { this->begin = begin; }

	/*
	OPERATORS
	operator=:
	* returns void
	* sets the data to equal a menu or initalizer_list.
	* colors from a menu object are copied, but data from a list sets the color
		to the default colors in the objects
	
	operator+:
	* returns a menu object that is the concatonated version of the menu, data_type, or initializer_list with the input
		appeneded to the end of this object.
	* colors are taken from menus, but an initializer_list or data_type sets the appended data to have the default color
	* this function does not work in reverse
	
	operator+=:
	* returns void
	* appends this object with the inputted initializer_list or menu. The colors will be consistant with an added
		menu, but lists default to the default colors.
	
	operator>>:
	* returns the selected value from the data as a data_type by reference
	* edits the input to match the selected value from the vector through the UI
	
	operator>>=:
	* returns the value of the location as an int
	* edits the input to match the location of the selection from the menu through the UI

	operator[]:
	* returns data_type by reference
	* calls the at(int) function with the inputted location
	* this returns by reference, so the value can be edited
	* functions exactly as the at function

	operator<<:
	* outputs just the title with colors included
	* returns an ostream&

	operator~:
	* returns makeChoice()
	* takes no arguments

	*/

	void operator=(const menu<data_type>& input){
		this->data.clear();
		this->colors.clear();
		
		for (int i = 0; i < input.size(); i++)
		{
			this->data.push_back(input.data.at(i));
			this->colors.push_back(input.colors.at(i));
		}

		this->title = input.title;
		this->titleColor = input.titleColor;
		this->hasNumbering = input.hasNumbering;
		this->begin = input.begin;
		this->before = input.before;
		this->after = input.after;
		this->beforeCursor = input.beforeCursor;
		this->afterCursor = input.afterCursor;
	
	}
	void operator=(const initializer_list<data_type> input) {
		data.clear();
		for (int i = 0; i < input.size(); i++)
		{
			data.push_back(*(input.begin() + i));
		}
		colorScheme();
	}
	
	template<typename dataType>
	friend ostream& operator<<(ostream& os, const menu<dataType>& input);

	menu operator+(menu<data_type> input)const{
		menu<data_type> tempPlus;
		tempPlus = *this;

		for (int i = 0; i < input.size(); i++) {
			tempPlus.data.push_back(input.data.at(i));
			tempPlus.colors.push_back(input.colors.at(i));
		}
		return tempPlus;
	}
	menu operator+(const data_type& input)const {
		menu temp;
		temp = *this;
		temp.push_back(input);
		return temp;
	}
	menu operator+(initializer_list<data_type> input)const {
		menu<data_type> temp(input);
		return *this + temp;
	}

	void operator+=(menu<data_type> input) { *this = (*this + input); }
	void operator+=(initializer_list<data_type> input) { *this = (*this + input); }

	data_type& operator>>(data_type& output){
		int i = operator~();
		output = data.at(i);
		return output;
	}


	int operator>>=(int& output){
		output = operator~();
		return output;
	}

	int operator~() {
		return makeChoice();
	}

	data_type& operator[](int location) { return at(location); }

	/*
	DATA MANIPULATION
	size:
	* returns the length of the data customVect as an int.

	push_back:
	* pushes back the input to the end of the data.
	* Takes one input of type data_type

	erase:
	* removes a range of values from the menu
	* all inputs are ints
	* must have a start position, but the endPosition defaults to one after startPosition
	* erases as [startPosition, endPosition) similar to the vector.erase command

	at:
	* returns a value from the menu
	* return type is data_type and is returned by reference
	* input determines the location of the data. The count starts at 0.
	* the value is passed in by reference, so it can be modified (e.g. this->at(2) = 4).

	setTitle:
	* modifies the value of the title based on the input
	* defaults to an input of nothing

	getTitle:
	* title getter
	* takes no inputs and is of type string

	clear:
	* totally resets the menu
	* clears all data and resets colors
	* does not edit formatting changes outside of colors

	setScrollLength:
	* sets how many rows of the menu are visible at once before it begins
		to scroll.
	* takes one input of int which defaults to 10
	* returns void

	*/
	int size()const { return data.size(); }

	void push_back(data_type input) {
		data.push_back(input);
		colors.push_back(0);
		uniqueColor(size()-1);
	}

	void erase(int startPosition, int endPosition) {
		data.erase( startPosition, endPosition);
		colors.erase(startPosition, endPosition);
	}
	void erase(int startPosition) { erase(startPosition, startPosition + 1); }

	void clear() { 
		menu<data_type> temp;
		*this = temp;
	}

	data_type& at(int location) {
		return data.at(location);
	}
	void setTitle(string input = "") {
		title = input;
	}
	string getTitle() const{ return title; }
	void setScrollLength(int length = 10) { scrollLength = length; }
};

template<typename data_type>
ostream& operator<<(ostream & os, const menu<data_type>& input)
{
	input.setDisplayColor(input.titleColor);
	os << input.title;
	input.setDisplayColor();
	return os;
}

#endif