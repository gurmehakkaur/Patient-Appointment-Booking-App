#define _CRT_SECURE_NO_WARNINGS

// System Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include "core.h"

#include "clinic.h"

void sort(struct Appointment appoints[], int max) {
    int i, j;
    struct Appointment temp;

    for (i = 0; i < max; i++) {
        appoints[i].time.min = (appoints[i].date.year * 12 * 30 * 24 * 60) + (appoints[i].date.month * 30 * 24 * 60) + (appoints[i].date.day * 24 * 60) + (appoints[i].time.hour * 60) + appoints[i].time.min;
    }

    for (i = max - 1; i > 0; i--)
    {
        for (j = 0; j < i; j++)
        {
            if (appoints[j].time.min > appoints[j + 1].time.min)
            {
                temp = appoints[j];
                appoints[j] = appoints[j + 1];
                appoints[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < max; i++) {
        appoints[i].time.min = appoints[i].time.min - (appoints[i].date.year * 12 * 30 * 24 * 60) - (appoints[i].date.month * 30 * 24 * 60) - (appoints[i].date.day * 24 * 60) - (appoints[i].time.hour * 60);
    }

}

//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}


// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}


// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}


// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}


//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// main menu
void menuMain(struct ClinicData* data)
{
    int selection;

    do {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;

    do {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ", patient->patientNumber, patient->name);
        
        displayFormattedPhone(patient->phone.number);
        
        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}



// Menu: Appointment Management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                           data->patients, data->maxPatient);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                              data->patients, data->maxPatient);  // ToDo: You will need to create this function!
            suspend();
            break;
        }
    } while (selection);
}




// Display's all patient data in the FMT_FORM | FMT_TABLE format

void displayAllPatients(const struct Patient patient[], int max, int fmt) 
{
    int i; 

    if (patient == NULL) 
    {
        printf("*** No records found ***\n");
    }
    else
    {
        if (fmt == FMT_TABLE) 
        {
            displayPatientTableHeader();
        }

        
        for (i = 0; i < max; i++) 
        {
            if (patient[i].patientNumber != 0)
            {
                displayPatientData(&patient[i], fmt);
            }
        }
        printf("\n");
    }
}

void searchPatientData(const struct Patient patient[], int max) 
{
    int searchOption;

    do
    {
        printf("Search Options\n");
        printf("==========================\n");
        printf("1) By patient number\n");
        printf("2) By phone number\n");
        printf("..........................\n");
        printf("0) Previous menu\n");
        printf("..........................\n");
        printf("Selection: ");
        scanf(" %d", &searchOption); 
        printf("\n");

        if (searchOption == 1)
        {
            searchPatientByPatientNumber(patient, max);
        }
        else if (searchOption == 2)
        {
            searchPatientByPhoneNumber(patient, max);
        }
    } while (searchOption != 0);
}

// Add a new patient record to the patient array

void addPatient(struct Patient patient[], int max) 
{
    int i; 

   
    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber == 0) 
        {
            patient[i].patientNumber = nextPatientNumber(patient, max); 
            inputPatient(&patient[i]); 

            printf("*** New patient record added ***\n\n"); 
            
            return; 
        }
        
    }
    
    
    printf("ERROR: Patient listing is FULL!\n\n");

    return;

}

// Edit a patient record from the patient array

void editPatient(struct Patient patient[], int max) 
{
    int patientNum; 
    int patientIndex; 

   
    printf("Enter the patient number: ");
    scanf(" %d", &patientNum);
    printf("\n");

    patientIndex = findPatientIndexByPatientNum(patientNum, patient, max);

    if (patientIndex == -1) 
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else 
    {
        menuPatientEdit(&patient[patientIndex]);
    }

    return;

}

// Remove a patient record from the patient array

void removePatient(struct Patient patient[], int max) 
{
    int patientNum; 
    int patientIndex; 
    char userConformation[4]; 


    printf("Enter the patient number: ");
    scanf(" %d", &patientNum); 
    printf("\n");

   
    patientIndex = findPatientIndexByPatientNum(patientNum, patient, max);

    if (patientIndex == -1) 
    {
        printf("ERROR: Patient record not found!\n\n");
	    getchar();
    }
    else 
    {
        displayPatientData(&patient[patientIndex], FMT_FORM); 

        printf("\nAre you sure you want to remove this patient record? (y/n): ");
        getchar();

        do
        {
            fgets(userConformation, sizeof(userConformation), stdin); 
            
            if (strlen(userConformation) != 2) 
            {
                printf("ERROR: Character must be one of [yn]: ");
                getchar();
            }
        } while (strlen(userConformation) != 2); 
      
        if (userConformation[0] == 'n')
        {
            printf("Operation aborted.\n\n"); 
        }
        else
        {
            patient[patientIndex].patientNumber = 0; 
            printf("Patient record has been removed!\n\n");
        }
    }
    

    return;

}



// View ALL scheduled appointments

void viewAllAppointments(struct ClinicData* data) 
{
    
    int i, j;

    
    sort(data->appointments, data->maxAppointments);

    // Display the patients geader
    displayScheduleTableHeader(NULL, 1);

    // Display the data
    for (i = 0; i < data->maxAppointments; i++) 
    {
        for (j = 0; j < data->maxPatient; j++) 
        {
            if (data->appointments[i].patientNumber == data->patients[j].patientNumber) 
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 1);
            }
        }
    }
    printf("\n");
    
}

// View appointment schedule for the user input date

void viewAppointmentSchedule(struct ClinicData* data) 
{
  
    int lastDay;
    int i, j; 
    struct Date date;
    
    
    printf("Year        : ");
    date.year = inputIntPositive();
    


    printf("Month (1-12): ");
    date.month = inputIntRange(1, 12);
   


    if (date.month == 4 || date.month == 6 || date.month == 9 ||  date.month == 11) 
    {
        lastDay = 30;
    }
    else if (date.month == 2) 
    {
        if (date.year % 4 == 0) 
        {
            lastDay = 29;
        }
        else
        {
            lastDay = 28;
        }
    }
    else
    {
        lastDay = 31;
    }

   
    printf("Day (1-%d)  : ", lastDay);
    date.day = inputIntRange(1, lastDay);
    printf("\n");
    

 
    sort(data->appointments, data->maxAppointments);
    
    
    displayScheduleTableHeader(&date, 0);

  
    for (i = 0; i < data->maxAppointments; i++) 
    {
        for (j = 0; j < data->maxPatient; j++) 
        {
           
            if (data->appointments[i].patientNumber == data->patients[j].patientNumber && data->appointments[i].date.year == date.year && data->appointments[i].date.month == date.month && data->appointments[i].date.day == date.day) 
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 0);
            }
        }
    }
    printf("\n");

}


void addAppointment(struct Appointment *app, int maxAppointments, struct Patient *pt, int maxPatients)
{
    struct Date date;
    struct Time time;

   
    int lastDay;
    int patientNumber, index; 
    int slotFlag = 1; 

   
    printf("Patient Number: ");
    patientNumber = inputIntPositive();
    
  
    index = findPatientIndexByPatientNum(patientNumber, pt, maxPatients);

   
    if (index >= 0) 
    {
        while (slotFlag) 
        {
          
            printf("Year        : ");
            date.year = inputIntPositive();

      
            printf("Month (1-12): ");
            date.month = inputIntRange(1, 12);


            if (date.month == 4 || date.month == 6 || date.month == 9 ||  date.month == 11) 
            {
                lastDay = 30;
            }
            else if (date.month == 2) 
            {
                if (date.year % 4 == 0) 
                {
                    lastDay = 29;
                }
                else
                {
                    lastDay = 28;
                }
            }
            else
            {
                lastDay = 31;
            }

            printf("Day (1-%d)  : ", lastDay);
            date.day = inputIntRange(1, lastDay);
            
           
            printf("Hour (0-23)  : ");
            time.hour = inputIntRange(0, 23);

     
            printf("Minute (0-59): ");
            time.min = inputIntRange(0, 59);

     
            if (timeSlotAvailable(date, time, app, maxAppointments))
            {
                printf("\nERROR: Appointment timeslot is not available!\n\n");
            }
            else 
            {
            
                do
                {
                  
                    printf("ERROR: Time must be between %02d:00 and %02d:00 in %02d minute intervals.\n\n", START_HOUR, END_HOUR, MINUTE_INTERVAL);

                   
                    printf("Hour (0-23)  : ");
                    time.hour = inputIntRange(0,23);

                   
                    printf("Minute (0-59): ");
                    time.min = inputIntRange(0,59);

                } while ((time.hour < START_HOUR || time.hour > END_HOUR) || (time.hour == END_HOUR && time.min > 0) || (time.min % MINUTE_INTERVAL != 0));

               
                slotFlag = 0;
            }
        }

       
        index = nextSlotAvailable(app, maxAppointments); 


        app[index].date = date;
        app[index].time = time;
        app[index].patientNumber = patientNumber;

     
        printf("\n*** Appointment scheduled! ***\n\n");
    }
    else 
    {
        printf("\nERROR: Patient record not found!\n\n");
    }
    return;
}



void removeAppointment(struct Appointment *app, int maxAppointments, struct Patient *pt, int maxPatients) 
{
 
    int lastDay, patientNumber;
    struct Date date;
    int index, appointmentIndex; 
    
    
    printf("Patient Number: ");
    patientNumber = inputIntPositive();
    index = findPatientIndexByPatientNum(patientNumber, pt, maxPatients);

    if (index >= 0)
    {
        printf("Year        : ");
        date.year = inputIntPositive();

      
        printf("Month (1-12): ");
        date.month = inputIntRange(1, 12);

       
        if (date.month == 4 || date.month == 6 || date.month == 9 ||  date.month == 11) 
        {
            lastDay = 30;
        }
        else if (date.month == 2) 
        {
            if (date.year % 4 == 0)
            {
                lastDay = 29;
            }
            else
            {
                lastDay = 28;
            }
        }
        else
        {
            lastDay = 31;
        }

       
        printf("Day (1-%d)  : ", lastDay);
        date.day = inputIntRange(1, lastDay);
        printf("\n"); 

      
        appointmentIndex = validAppointment(patientNumber, date, app, maxAppointments);

       
        if(appointmentIndex >= 0)
        {

           
            displayPatientData(&pt[index],FMT_FORM);

          
            printf("Are you sure you want to remove this appointment (y,n): ");
            
           
            if(inputCharOption("yn") == 'y') 
            {               
                app[appointmentIndex].patientNumber = 0;
                printf("\nAppointment record has been removed!\n");
            }
            else 
            {
                printf("ERROR: No appointment for this date!\n\n");
            }
        }
        clearInputBuffer();
        
    }
    else 
    {
        printf("ERROR: Patient record not found!\n\n");
    }

    return;

}


// Search and display patient record by patient number (form)

void searchPatientByPatientNumber(const struct Patient patient[], int max) 
{
   
    int patientNumber; 
    int patientIndex;

   
    printf("Search by patient number: ");
    scanf(" %d", &patientNumber);
    printf("\n");

    
    patientIndex = findPatientIndexByPatientNum(patientNumber, patient, max);

    if (patientIndex == -1) 
    {
        printf("*** No records found ***\n");
    }
    else 
    {
        displayPatientData(&patient[patientIndex], FMT_FORM);
        
    }
    printf("\n");


    clearInputBuffer();
    suspend();

    return;
}

// Search and display patient records by phone number (tabular)

void searchPatientByPhoneNumber(const struct Patient patient[], int max) 
{
    
    char phoneNumber[PHONE_LEN + 1];
    int phoneNumFlag = 0;
    int i;
  
    clearInputBuffer();
    printf("Search by phone number: ");
    scanf("%[^\n]", phoneNumber); 
    printf("\n");

    
    for (i = 0; i < max; i++)
    {
        if (strcmp(patient[i].phone.number, phoneNumber) == 0) 
        {
            if (phoneNumFlag == 0) 
            {
                displayPatientTableHeader();  
            }
            displayPatientData(&patient[i], FMT_TABLE); 
            phoneNumFlag++;
        }
        
    }

    if (phoneNumFlag == 0) 
    {
        displayPatientTableHeader(); 
        printf("\n*** No records found ***\n");
    }
    
  
    printf("\n");
    clearInputBuffer();
    suspend();


    return;
}

// Get the next highest patient number

int nextPatientNumber(const struct Patient patient[], int max) 
{
   
    int largestPatientNum; 
    int i = 0;

    largestPatientNum = patient[i].patientNumber; 
    
    for (i = 1; i < max; i++)
    {
        if (patient[i].patientNumber > largestPatientNum) 
        {
           
            largestPatientNum = patient[i].patientNumber; 
        }
        
    }

    return largestPatientNum + 1; 
}

// Find the patient array index by patient number (returns -1 if not found)

int findPatientIndexByPatientNum(int patientNumber, const struct Patient patient[], int max) 
{
    int i; 

    
    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber == patientNumber) 
        {
            return i;
        }
        
    }
    
    return -1; 
}


int timeSlotAvailable(struct Date date, struct Time time, struct Appointment* app, int maxAppointments) 
{

    int i; 
    int SlotAvailable = 0; 
    for (i = 0; i < maxAppointments; i++) 
    {
     
        if (date.year == app[i].date.year && date.month == app[i].date.month && date.day == app[i].date.day && time.hour == app[i].time.hour && time.min == app[i].time.min) 
        {
       
            SlotAvailable = 1;
        }
    }

    return SlotAvailable;
}


int nextSlotAvailable(struct Appointment* app, int maxAppointments) 
{
    int index = 0; 
    int availableFlag = 0;

  
    for (index = 0; index < maxAppointments && availableFlag == 0; index++)
    {
        if (app[index].patientNumber < 1) 
        {
            availableFlag = 1;
        }
    }

    return index;
}

// Checks to see if an appointment is valid
int validAppointment(int patientNumber, struct Date date, struct Appointment *app, int maxAppointments) 
{
  
    int i = 0; 
    int validFlag = 0; 


    for (i = 0; i < maxAppointments && validFlag == 0; i++)
    {
        if ((app[i].patientNumber == patientNumber) && (app[i].date.day == date.day) && (app[i].date.month == date.month) && (app[i].date.year == date.year)) {
            validFlag = 1; 
        }
    }
    
    return i - 1;
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record

void inputPatient(struct Patient* patient) 
{
    
    printf("Patient Data Input\n");
    printf("------------------\n");
    printf("Number: %05d\n", patient->patientNumber);

    printf("Name  : "); 
    inputCString(patient->name, 1, NAME_LEN);
    putchar('\n');
    
    
    inputPhoneData(&patient->phone);
}

// Get user input for phone contact information

void inputPhoneData(struct Phone* phone) 
{
   
    int phoneDesc; 
    char phoneNum[PHONE_LEN + 2]; 


    printf("Phone Information\n");
    printf("-----------------\n");
    printf("How will the patient like to be contacted?\n");
    printf("1. Cell\n");
    printf("2. Home\n");
    printf("3. Work\n");
    printf("4. TBD\n");

  
    printf("Selection: ");
    scanf(" %d", &phoneDesc);
    printf("\n");

    
    switch (phoneDesc)
    {
    case 1:
        
        strncpy(phone->description, "CELL", PHONE_DESC_LEN);

        
        printf("Contact: %s\n", phone->description);

        
        clearInputBuffer();
        printf("Number : ");

        
        do
        {
            
            scanf("%[^\n]", phoneNum);

            
            if (strlen(phoneNum) != 10)
            {
                printf("Invalid 10-digit number! Number: ");
                getchar();
            }
            
        } while (strlen(phoneNum) != 10);

        
        phoneNum[PHONE_LEN] = '\0';
        strncpy(phone->number, phoneNum, PHONE_LEN);
        phone->number[PHONE_LEN] = '\0';
        printf("\n");
        clearInputBuffer();
        break;

    case 2:
        
        strncpy(phone->description, "HOME", PHONE_DESC_LEN);

        
        printf("Contact: %s\n", phone->description);

        
        clearInputBuffer();
        printf("Number : ");

        
        do
        {
            
            scanf("%[^\n]", phoneNum);

            
            if (strlen(phoneNum) != 10)
            {
                printf("Invalid 10-digit number! Number: ");
                getchar();
            }
            
        } while (strlen(phoneNum) != 10);

       
        phoneNum[PHONE_LEN] = '\0';
        strncpy(phone->number, phoneNum, PHONE_LEN);
        phone->number[PHONE_LEN] = '\0';
        printf("\n");
        clearInputBuffer();
        break;

    case 3:
       
        strncpy(phone->description, "WORK", PHONE_DESC_LEN);

       
        printf("Contact: %s\n", phone->description);

        
        clearInputBuffer();
        printf("Number : ");

       
        do
        {
            scanf("%[^\n]", phoneNum);
\
            if (strlen(phoneNum) != 10)
            {
                printf("Invalid 10-digit number! Number: ");
                getchar();
            }
            
        } while (strlen(phoneNum) != 10);

      
        phoneNum[PHONE_LEN] = '\0';
        strncpy(phone->number, phoneNum, PHONE_LEN);
        phone->number[PHONE_LEN] = '\0';
        printf("\n");
        clearInputBuffer();
        break;

    case 4:
       
        strncpy(phone->description, "TBD", PHONE_DESC_LEN);
        *phone->number = 0; 

        break;
    
    default:
        
        printf("Error:\n");
        break;
    }
    return;

}


//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)

int importPatients(const char* datafile, struct Patient patients[], int max) 
{
   
    FILE *file;

   
    int i; 
    int count = 0;

    file = fopen(datafile,"r");

   
    for (i = 0; (i < max) && (feof(file) == 0); i++) // Loop to get rows
    {
      
        fscanf(file, "%d|%[^|]|%[^|]|%[^\n]", &patients[i].patientNumber, patients[i].name, patients[i].phone.description, patients[i].phone.number);       

       
        if (!feof(file)) 
        {
            count++;
        }
        
    }

  
    fclose(file);


    return count;
}

// Import appointment data from file into an Appointment array (returns # of records read)

int importAppointments(const char* datafile, struct Appointment appoints[], int max) 
{
  
    FILE *file;

   
    int i; 
    int count = 0; 


   
    file = fopen(datafile,"r");

   
    for (i = 0; (i < max) && (feof(file) == 0); i++) 
    {
        
        fscanf(file, "%d,%d,%d,%d,%d,%d", &appoints[i].patientNumber, &appoints[i].date.year, &appoints[i].date.month, &appoints[i].date.day, &appoints[i].time.hour, &appoints[i].time.min);
        
        
        if (!feof(file)) {
            count++;
        }
        
    }

    fclose(file);
    return count;
}

