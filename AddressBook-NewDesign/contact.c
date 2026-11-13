#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria

    // Bubble sort
    // Bubble sort by name
    for(int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for(int j = 0; j < addressBook->contactCount - i - 1; j++)
        {   //check if name is same even though it is capital letter or small letter
            if(strcasecmp(addressBook->contacts[j].name,addressBook->contacts[j + 1].name) > 0)
            {
                // Swap entire structures
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
    printf("-------------------------------------------------------------------------------------\n");
    printf("                                  CONTACT LIST\n");
    printf("-------------------------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------------------------\n");
    printf("No  Name                       Phone Number       Email\n");
    printf("-------------------------------------------------------------------------------------\n");

    // Print contacts
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        printf("%-3d %-26s %-18s %-34s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
   printf("--------------------------------------------------------------------------------------\n");
}


void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int validate_name(char *str)
{
    //if name doesn't start with a letter
    if((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z'))
    {
    for(int i = 0; str[i]; i++)
    {
        //check if each character of string is capital letter, small letter, space and dot
        if( ! (str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z' || str[i] == ' ' || str[i] == '.')) // not valid
        {
            return 0; // invalid
        }
    }
    }
    else
    {
    return 0;//invalid
    }
    return 1; //valid
}

int validate_phone(char *str, AddressBook *addressBook)
{
    
    int duplicate = 0;
    //check for duplicates
    for(int i = 0; i < addressBook->contactCount; i++)
    {

        if(strcmp(addressBook->contacts[i].phone, str) == 0)
        {
            duplicate = 1;
            break;
        }
    }
    if(duplicate == 1)
    {
        printf("Phone number already exists!!\n\n");
        return 0;// duplicate present
    }
    if(strlen(str) == 10)// valid length
    {
        for(int i = 0; str[i]; i++)
        {//check each character of string is a digit
            if(!(str[i] >= '0' && str[i] <= '9')) // not valid
            {
                printf("Phone number must have digits only\n");
                return 0; // invalid
            }
        }
        return 1; // valid
    }
    printf("Phone number should contain 10 digits only\n");
    return 0;
}

int validate_email(char *str, AddressBook *addressBook)
{
    int at = 0, dot = 0, j = 0, k = 0;
    int duplicate = 0;
    for(int i = 0; i<addressBook->contactCount; i++)
    {
        //check for duplicates
        if(strcmp(addressBook->contacts[i].email, str) == 0)
        {
            duplicate = 1;
            break;
        }
    }
    if(duplicate == 1)
    {
        printf("Email already exists!!\n\n");
        return 0;//invalid, duplicate present
    }
    if((str[0] >= 'a' && str[0] <= 'z'))// valid first character
    {
        for(int i = 1; str[i]; i++)
        {// valid characters
            if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '@' || str[i] == '.')
            {
                if(str[i] == '@')
                {
                    at++;
                    j = i;
                }
                if(str[i] == '.')
                {
                    dot++;
                    k = i;
                }
            }
            else
            {
                printf("Email should not contain any special characters/ uppercase letters\n");
                return 0;
            }
        }
        if(at != 1 || dot != 1)// invalid number of @ or .
        return 0;
        if((k - j > 1) && !(strcmp(str+k,".com")))// valid domain
        {
            return 1;
        }
    }
    printf("Email should start with lowercase letters\n");
    return 0; // invalid
}


void createContact(AddressBook *addressBook)
{
	/*logic to create a Contacts */

    char str1[30], str2[11], str3[30];
    int res = 0;
    do
    {
        printf("Enter the name :");
        scanf(" %29[^\n]", str1);

        res = validate_name(str1);
        if(res == 0)//invalid, ask for valid name
        {
            printf("Please enter valid name.\n");
        }
    }while(res == 0); // 0 means invalid

    do
    {
        printf("Enter the phone :");
        scanf(" %10[^\n]", str2);

        res = validate_phone(str2, addressBook);//addressbook is passed in the function to check for duplicates
        if(res == 0)//invalid, ask for valid phone
        {
            printf("Please enter valid phone number.\n");
        }
    }while(res == 0); // 0 means invalid

    do
    {
        printf("Enter the email :");
        scanf(" %29[^\n]", str3);
       
        res = validate_email(str3, addressBook);//addressbook is passed to the function to check for duplicates
        if(res == 0)//invalid,ask another name
        {
            printf("Please enter valid email.\n");
        }
    }while(res == 0); // 0 means invalid
    //name, phone and email are valid, adding it into the addressBook 
    int ind = addressBook->contactCount;
    strcpy(addressBook->contacts[ind].name, str1);
    strcpy(addressBook->contacts[ind].phone, str2);
    strcpy(addressBook->contacts[ind].email, str3);
    printf("      Contact created successfully.\n");

    //increment the count of contacts in addressBook
    addressBook->contactCount++;

}
//Searching
int search_by_name(AddressBook *addressBook)
{
    char str[30];
    //ask for a name to search
    printf("Enter the name: ");
    scanf(" %29[^\n]", str);
    
    int count = 0, k;
    //create an array to store the index
    int index[100];
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        //compare at most n characters, without considering if it is uppercase / smaller csae
        if(strncasecmp(addressBook->contacts[i].name, str, strlen(str)) == 0)
        {
            //if found, store index to array index
           index[count] = i; 
           //increment count
           count++;
        }
    }
    //no contacts found
    if(count == 0)
    {
        printf("      Contact not found!!\n\n");
        return -1;
    }
    //if only, 1 contact is found
    else if (count == 1)
    {
        printf("%s %s %s\n", addressBook->contacts[index[0]].name, addressBook->contacts[index[0]].phone, addressBook->contacts[index[0]].email);
        return index[0];
    }
    //more than 1 contacts are found
    else
    {
        printf("\nMultiple contacts found\n");
        for(int i = 0; i < count; i++)
        {
            //print each contact
            printf("%-3d   %-26s %-18s %-34s\n", i+1, addressBook->contacts[index[i]].name, addressBook->contacts[index[i]].phone, addressBook->contacts[index[i]].email);
        }
        //ask which contact is needed for user
        int choice;
        printf("Enter the choice: ");
        if(scanf("%d", &choice) != 1)
        {
            printf("      Invalid choice. Please try again.\n");
            while(getchar() != '\n')
            continue;
        }
        //if entered choice is less than or equal to the count
        if(choice <= count && choice > 0)
        {
        printf("%-26s %-18s %-34s\n", addressBook->contacts[index[choice - 1]].name, addressBook->contacts[index[choice - 1]].phone, addressBook->contacts[index[choice - 1]].email);
        
        return index[choice - 1];
        }
        //if entered choice is invalid
        else
        printf("     Invalid choice\n");
    }
}

int search_by_phone(AddressBook *addressBook)
{
    char str[11];
    //ask for phone number to search
    printf("Enter the phone: ");
    scanf(" %10[^\n]", str);
    //search for the user entered phone number
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, str) == 0)
        {
            printf("%s %s %s\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            return i;//contact found, return index
        }
    }
    //no contacts found
    printf("Contact not found\n");
    return -1;
}

int search_by_email(AddressBook *addressBook)
{
    char str[30];
    //ask for email to search
    printf("Enter the email: ");
    scanf(" %29[^\n]", str);
    //search for the user entered email
    for(int i = 0; i<addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].email, str) == 0)
        {
            printf("%s %s %s\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            return i;//contact found, return index
        }
    }
    //contact not found
    printf("       Contact not found\n");
    return -1;
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    do
    {
    int choice;
    printf("Search by: \n");
    printf("1. Name\n");
    printf("2. Phone number\n");
    printf("3. Email\n");
    printf("4. Exit\n");

    printf("Enter the choice: ");
    //if entered choice is a digit or not
    if(scanf("%d", &choice) != 1)
    {
        printf("      Invalid choice. Please try again.\n");
        //loop until it gets a new line
        while(getchar() != '\n');
        continue;
    }

    switch(choice)
    {
        case 1:
            search_by_name(addressBook);
            break;
        case 2:
            search_by_phone(addressBook);
            break;
        case 3:
            search_by_email(addressBook);
            break;
        case 4:
            return; 
        default:
            printf("     Invalid choice. Please try again.\n");
            continue;
    }
    }while(1);
}

//Editing
void edit_by_name(int ind, AddressBook *addressBook)
{
    char str[30];
    do
    {
        //ask for new name
        printf("Enter new name: ");
        scanf(" %[^\n]", str);
        //validate name
        if(validate_name(str) == 0)
          printf("     Invalid name. Please try again.\n");//invalid
        else
          break;//valid
    } while(1);
    //copy the name
    strcpy(addressBook->contacts[ind].name, str);
}

void edit_by_phone(int ind, AddressBook *addressBook)
{
    char str[30];
    do
    {
        //ask for new phone number
        printf("Enter new phone number: ");
        scanf(" %[^\n]", str);
        //validate phone
        if(validate_phone(str, addressBook) == 0)
          printf("     Invalid phone number. Please try again.\n");//invalid phone number
        else
          break;//valid
    } while(1);
    //copy the phone number
    strcpy(addressBook->contacts[ind].phone, str);
}

void edit_by_email(int ind, AddressBook *addressBook)
{
    char str[30];
    do
    {
        //new email
        printf("Enter new email: ");
        scanf(" %[^\n]", str);
        if(validate_email(str, addressBook) == 0)
          printf("     Invalid email. Please try again.\n");//invalid
        else
          break;//valid
    } while(1);
    //copy email
    strcpy(addressBook->contacts[ind].email,str);
}

void editContact(AddressBook *addressBook)
{
    do
    {
    //ask for the contact to edit
    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");    
    printf("4. Exit\n");

    int choice, ind = -1, edit_choice;
    printf("Enter the choice: ");
    //check if entered choice is a digit or not
    if(scanf("%d", &choice) != 1)
    {
        printf("      Invalid choice. Please try again.\n");
        while(getchar() != '\n');
        continue;
    }
     
    switch(choice)
    {
        case 1:
                ind = search_by_name(addressBook);
                break;
        case 2: 
                ind = search_by_phone(addressBook);
                break;
        case 3:
                ind = search_by_email(addressBook);
                break;
        case 4:
                return;
        default:
                printf("      Invalid choice. Please try again.\n");
                continue;
    }
    if(ind == -1)//contact not found
    { 
        printf("      Editing unsuccessfull.\n");//no contact found, therefore editing cannot be done
        continue;
    }
    //if contact found, ask for the field to edit
    do
    {
    printf("Field to edit: \n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("4. Exit\n");

    printf("Enter the choice: ");
    //check if entered choice is an integer
    if(scanf("%d", &edit_choice) != 1)
    {
        printf("      Invalid choice. Please try again.\n");
        while(getchar() != '\n');
        continue;
    }

    switch(edit_choice)
    {
        case 1:
                edit_by_name(ind, addressBook);
                break;
        case 2:
                edit_by_phone(ind, addressBook);
                break;
        case 3:
                edit_by_email(ind, addressBook);
                break;
        case 4:
                break;
        default:
                printf("     Invalid choice.\n");
                continue;
    }
    printf("%s %s %s\n", addressBook->contacts[ind].name, addressBook->contacts[ind].phone, addressBook->contacts[ind].email);
}while(edit_choice != 4);
}while(1);
}
//Deleting
void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    do
    {
    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");    
    printf("4. Exit\n");

    int choice, ind = -1;
    //check if entered choice is a valid integer
    printf("Enter the choice: ");
    if(scanf("%d", &choice) != 1)
    {
        printf("      Invalid choice. Please try again.\n");
        while(getchar() != '\n');
        continue;
    }
     
    switch(choice)
    {
        case 1:
                ind = search_by_name(addressBook);
                break;
        case 2: 
                ind = search_by_phone(addressBook);
                break;
        case 3:
                ind = search_by_email(addressBook);
                break;
        case 4:
                return;
        default:
                printf("      Invalid choice. Please try again.\n");
    }
    //no contact found
    if(ind == -1)
    {
        printf("        Cannot delete.\n");
        continue;
    } 
    char ch;
    //ask for confirmation
label:    
printf("Are you sure you want to delete? (Y/N): ");
    scanf(" %c",&ch);
    //if user entered y/Y
    if(ch == 'Y' || ch == 'y')
    {    
    for(int i = ind; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;
    printf("       Contact deleted successfully.\n");
    }
    //user entered n/N, don't want to delete
    else if(ch == 'N' || ch == 'n')
        continue;
    //user entered character other than y/Y/n/N
    else
    {
        printf("       Invalid character.\n");
        goto label;
    }
}while(1);
}
