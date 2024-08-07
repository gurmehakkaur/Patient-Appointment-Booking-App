#define _CRT_SECURE_NO_WARNINGS

// System library
#include <stdio.h>
#include <string.h>

// User-defined library
#include "core.h"
#include "clinic.h"


// Clear the standard input buffer 
void clearInputBuffer(void) 
{ 
    
    // Discard all remaining char's from the standard input buffer: 
    while (getchar() != '\n' )
    { 
        ; // do nothing! 
    } 
} 
 
// Wait for user to input the "enter" key to continue 
void suspend(void) 
{ 
    printf("<ENTER> to continue..."); 
    clearInputBuffer(); 
    putchar('\n'); 
}

// Ensures that entered input is an integer
int inputInt(void)
{
    int flag = 1;
    int Input; 
    char Char; 

	do 
    {
		scanf(" %d%c", &Input, &Char); 

		if (Char != '\n')
		{
            clearInputBuffer(); 
			printf("Error! Input a whole number: ");
		}
		else
		{  
			flag = 0; 
		}
	} while (flag); 

    return Input;
}

// Ensures that enteres integer is positive
int inputIntPositive(void)
{
    int flag = 1; 
    int Input; 
    char Char; 

    do 
    {
        scanf(" %d%c", &Input, &Char); 

        if (Char != '\n') 
		{
            clearInputBuffer();
			printf("Error! Input a whole number: ");
		}
        else if (Input <= 0) 
        {
            printf("ERROR! Value must be > 0: ");
        }
        else
        {
            flag = 0; 
        }
    } while (flag); 

    return Input;

}

// Ensures that entered number lies in given range
int inputIntRange(int lowerLimit, int upperLimit)
{
    int flag = 1;
    int userInput; 
    char newLineChar; 

    do 
    {
        scanf(" %d%c", &userInput, &newLineChar); 

        if (newLineChar != '\n') 
		{
            clearInputBuffer(); 
			printf("Error! Input a whole number: ");
		}
        else if (userInput > upperLimit || userInput < lowerLimit)
        {
            
            printf("ERROR! Value must be between %d and %d inclusive: ", lowerLimit, upperLimit);
           
        }
        else 
        {
            flag = 0; 
        }
    } while (flag); 

    return userInput;
}

// Ensures that entered char lies in given string
char inputCharOption(const char CharStr[])
{
    int flag;
    char inputChar;


    do {
        flag = 0;

        scanf(" %c", &inputChar);

        char* i = strchr(CharStr, inputChar);
        if (i != NULL)
        {
            flag++;
        }

        else
        {
            printf("ERROR: Character must be one of [%s]: ", CharStr);
        }
    } while (flag == 0);


    clearInputBuffer();

    return inputChar;
}

// Ensures entered string's length lies between the passed limit
void inputCString(char *stringValue, int minCharLimit, int maxCharLimit)
{
    int flag = 1;
    int stringLength = 0;
    int i = 0; 
    char stringInput[20] = { "\0" }; 
    
    do
    {
        for (i = 0; i < NAME_LEN; i++)
        {
            stringValue[i] = '\0'; 
        }
        
        scanf("%[^\n]", stringInput); 
        clearInputBuffer();
        
        stringLength = strlen(stringInput);

        if (stringInput[0] == '\0' || stringInput[0] == '\n')
        {
            flag = 1; 
        }
        else if ((stringLength > maxCharLimit || stringLength < minCharLimit) && !(stringInput[0] == '\0' || stringInput[0] == '\n'))// Input String is out of range
        {
       
            if (maxCharLimit == minCharLimit) 
            {
                printf("ERROR: String length must be exactly %d chars: ", maxCharLimit);
            }
            else if (stringLength > maxCharLimit) 
            {
                printf("ERROR: String length must be no more than %d chars: ", maxCharLimit);
            }
            else
            {
                printf("ERROR: String length must be between %d and %d chars: ", minCharLimit, maxCharLimit);
            }
        }
        else
        {
            
            flag = 0; 
        }

    } while (flag);
    

    for (i = 0; i < stringLength && stringInput[i] != '\0'; i++)
    {
        stringValue[i] = stringInput[i];
    }
    

    return;
}

// Return  given string of number in Phone format
void displayFormattedPhone(const char *string)
{
    int flag = 0; 
    int stringLength = 0;
    int i; 

    if (string == NULL) 
    {
        flag = 1; 
    }
    else
    {
        for (i = 0; string[i] != '\0'; i++) 
        {
            stringLength++; 

            if (string[i] < '0' || string[i] > '9') 
            {
                flag = 1; 
            }
        }
    }
    
    if (stringLength == 10 && flag == 0)
    {
        printf("(");
        for (i = 0; i < 3; i++)
        {
            printf("%c", string[i]);
        }
        printf(")");

        for (i = 3; i < 6; i++)
        {
            printf("%c", string[i]);
        }
        printf("-");

        for (i = 6; i < 10; i++)
        {
            printf("%c", string[i]);
        }
    }
    else 
    {
        printf("(___)___-____");
    }

    return;
    
}