
#ifndef CORE_H
#define CORE_H


//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Takes integer input from user and guarantees that the Returns is a valid integer number 
int inputInt(void);

// Guarantees that the return integer is greater than 0
int inputIntPositive(void);

// Guarantees that the return integer is within the range(inclusive)
int inputIntRange(int lowerLimit, int upperLimit);

// Guarantee a single character value is entered within the list of valid characters
char inputCharOption(const char charArray[]);

// Guarantee’s a C string value contains the number of characters within the range specified by the 2nd and 3rd arguments 
void inputCString(char* string, int minCharLimit, int maxCharLimit);

// Return a number in (###)###-#### format is number has 10 characters and all integers
void displayFormattedPhone(const char *string);

#endif // !CORE_H
