#include <llist.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <syslog.h>
#include <errno.h>
#include <strings.h>
#include <ncurses.h>
#include <menu.h>
#define PATH "ContactBook.lock"
#define WIDTH 30
#define HEIGHT 12
/*
Function to lock the app.
*/
int Lock();
/*
Function to unlock the app.
*/
void Unlock();
/*
Function to handle signals.
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
Function to create a Contact object.
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
Function to add a new contact to the linked list.
@param
contact: pointer to the Contact object.
*/
void addContact(struct Contact *contact);
/*
Function to remove contact from the linked list based on a given keyword.
@param
keyword: keyword to search for in the contacts.
*/
void removeContact(char *keyword);
/*
Function to insert a contact at a specific position in the linked list.
@param
contact: pointer to the Contact object.
@param
pos: position at which the contact is to be inserted.
*/
void insertContact(struct Contact *contact, uint16_t pos);
/*
Function to remove a contact from the linked list at a specific position.
@param
pos: position of the contact to be removed. (One-based indexing. 0 is not a valid pos).
*/
void removeContactPOS(uint16_t pos);
/*
Function to find a contact based on a keyword.
@param
keyword: keyword to search for in the contacts (can be a partial).
*/
void findContact(char *keyword);
/*
Function to wipe out all the contacts and data from the linked list.
@param
top: pointer to the head of the linked list.
*/
void wipeContacts();
/*
Function to save the contacts to a file.
*/
void saveContacts();
/*
Function to load contacts from a file.
*/
void loadContacts();
/*
Function to print out every contact's data.
*/
void printList();
/*
Function to print a contact object on given data.
@param
data: contact's data.
*/
void printObj(node_t* data);
/*
User interface function to print out menu options.
*/
void print_menu(WINDOW *menu_win, int highlight);
/*
User interface function which clears the menu.
*/
void del_menu(WINDOW *menu_win);