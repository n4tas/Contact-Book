#include <llist.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <syslog.h>
#include <errno.h>
#include <strings.h>
#define PATH "ContactBook.lock"
/*
Array of choices the user can choose from.
*/
const char *choices[] = {
    "1. Add contact",
    "2. Remove contact",
    "3. Insert contact",
    "4. Remove contact (POS)",
    "5. Find contact",
    "6. Wipe contact(s)",
    "7. Save contact(s)",
    "8. Load contact(s)",
    "9. Print contact(s)",
    "10. Exit"};
/*
Integer which represents the count of choices.
*/
int n_choices = sizeof(choices) / sizeof(char *);
/*
Integer which represents the user's choice.
*/
int user_choice;
/*
Function which creates a .lock file. A lock file 
tells other applications not to use the file unless 
it is free from the application that is using it.
*/
int lock();
/*
Function which unlocks the application, eventually deleting the .lock file.
*/
void unlock();
/*
Function which prompts that there is a signal, wipes all contacts, unlocks the app and exits.
@param
sig: represents the signal that the function is handling. 
*/
void handle_signal(int sig);
/*
Contact structure: each contact consists of four components (email can be null).
@param
first_name: first name of the contact.
@param
last_name: last name of the contact.
@param
phone_number: phone number of the contact.
@param
email: email address of the contact.
*/
struct Contact
{
    char *first_name;
    char *last_name;
    char *phone_number;
    char *email;
};
/*
Function which creates a Contact object.
@param
first_name: first name of the contact.
@param
last_name: last name of the contact.
@param
phone_number: phone number of the contact.
@param
email: email address of the contact.
@return
Pointer to the created Contact object.
*/
struct Contact *Contact_obj(char *first_name, char *last_name, char *phone_number, char *email);
/*
Function which adds a new contact to the linked list.
@param
contact: pointer to the Contact object.
*/
void addContact(struct Contact *contact);
/*
Function which removes contact from the linked list based on a given keyword.
@param
keyword: keyword to search for in the contacts.
*/
void removeContact(char *keyword);
/*
Function which inserts a contact at a specific position in the linked list.
@param
contact: pointer to the Contact object.
@param
pos: position at which the contact is to be inserted.
*/
void insertContact(struct Contact *contact, uint16_t pos);
/*
Function which removes a contact from the linked list at a specific position.
@param
pos: position of the contact to be removed. (One-based indexing. 0 is not a valid pos).
*/
void removeContactPOS(uint16_t pos);
/*
Function which finds a contact based on a keyword.
@param
keyword: keyword to search for in the contacts (can be a partial).
*/
void findContact(char *keyword);
/*
Function which wipes out all the contacts and data from the linked list.
@param
top: pointer to the head of the linked list.
*/
void wipeContacts();
/*
Function which saves the contacts to a file.
*/
void saveContacts();
/*
Function which loads contacts from a file.
*/
void loadContacts();
/*
Function which prints out every contact's data.
*/
void printList();
/*
Function which prints a contact object on given data.
@param
data: contact's data.
*/
void printObj(node_t* data);
/*
Function which prints out choices.
*/
void print_choices(){
    for (int i = 0; i < n_choices; i++)
        printf("%s\n", choices[i]); 
    printf("Press index of the function: "); 
    scanf("%d", &user_choice);
}
