#pragma once
#include <linked_list.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
/**
 * Handles signals
 * @param sig signal
 */
void handle_signal(int sig);
/** 
  * Linked list's component, which points to the start of the linked list. 
*/
__attribute__((unused))static Node *head = NULL;
/**
 * Contact structure, indicates contact's name, last name, email and phone number.
 */
struct Contact {
    char *name;
    char *surname;
    char *email;
    char *phone;
};
/**
 * Dynamically allocates a new contact.
 * @param *name name.
 * @param *surname last name.
 * @param *email email.
 * @param phone phone number.
 */
struct Contact *ContactObj(char *name, char *surname, char *email, char *phone);
/**
 * Adds an allocated address to the linked list.
 * @param *contact allocated contact.
 */
void add_address(struct Contact *contact);
/**
 * Insert address at the given position.
 * @param *contact allocated contact.
 * @param pos position.
 */
void insert_address_pos(struct Contact *contact, __uint16_t pos);
/**
 * Deletes address of a given position.
 * @param pos position
 */
void delete_address_pos(__uint16_t pos);
/**
 * Deletes all addresses
 */
void delete_addresses();
/**
 * Returns an address of a given position.
 * @param pos position
 */
void find_address_pos(__uint16_t pos);
/**
 * Returns an address of a given keyword.
 * @param keyword keyword
 */
void find_address(char *keyword);
/**
 * Saves contacts to file.
 */
void save_contacts();
/**
 * Loads contacts from file.
 */
void load_contacts();
/**
 * Displays all contacts in the linked list.
 */
void display_contacts();
/**
 * Returns 0, if integer is a number.
 * @param *input char string buffer
 */
int is_valid_integer(char *input);
/**
 * Prints contact's data.
 * @param *contact struct Contact * object
 */
void printContact(struct Contact *contact);
/**
 * Terminal freeze with getchar()
 */
void term_freeze();
/**
 * Returns user's integral choice
 * @param *input user's standard input 
 */ 
int user_function(char *input);
/**
 * Prints all functions' names. 
 */
void user_choice();
/**
 * Asks for contact's general information. 
*/ 
void enter_info(char *name, char *surname, char *email, char *phone);
/**
 * Returns user's chosen function.
 * @param *buffer integral buffer
 * @param pos position
 */
int user_position(char *buffer, __uint16_t pos);
