# Contact-Book
This project is a command-line based contact management system written in C. It allows users to add, remove, insert, find, wipe, save, load, and print contacts. The user interface is built using the ncurses library.
## Features
-  Add Contact: Add a new contact to the list.
-  Remove Contact: Remove a contact by keyword (first name, last name, phone number, or email).
-  Insert Contact: Insert a contact at a specific position.
-  Remove Contact (POS): Remove a contact at a specific position.
-  Find Contact: Find a contact by keyword.
-  Wipe Contacts: Delete all contacts.
-  Save Contacts: Save contacts to a file.
-  Load Contacts: Load contacts from a file.
-  Print Contacts: Print all contacts.
-  Exit: Exit the program.
## Dependencies
- <a href="https://github.com/N4tis/Linked-List">`Linked List Library`</a> - Provides essential data structures and functions for managing contacts, including operations to create, insert, and remove nodes in the list.
- <a href="https://www.linux.co.cr/ldp/lfs/appendixa/ncurses.html">`Ncurses`</a> - Allows to create the text-based user interface, enabling functionalities such as rendering menus, handling user input, and managing the overall interaction with the application.
## How Users Can Get Started with the Project
- Clone the repository:
`git clone https://github.com/N4tis/Contact-Book.git`

- Navigate to the project directory:
`cd Contact-Book`

- Compile the library:
`make`

- Run the compiled program:
`./a.out`

## Code Overview
### Structures
- `Contact`: Represents a contact with fields for first name, last name, phone number, and email.
- `node_t`: Node structure used in a linked list to manage contacts.
### Functions
-  `lock()`: Locks the application to prevent multiple instances.
-  `unlock()`: Unlocks the application.
-  `handle_signal(int sig)`: Handles termination signals for graceful shutdown.
-  `Contact(char *first_name, char *last_name, char *phone_number, char *email)`: Creates a new contact.
-  `addContact(struct Contact *contact)`: Adds a contact to the end of the list.
-  `removeContact(char *keyword)`: Removes a contact by keyword.
-  `insertContact(struct Contact *contact, uint16_t pos)`: Inserts a contact at a specified position.
-  `removeContactPOS(uint16_t pos)`: Removes a contact at a specified position.
-  `findContact(char *keyword)`: Finds and prints contacts matching a keyword.
-  `wipeContacts()`: Deletes all contacts.
-  `saveContacts()`: Saves contacts to a file.
-  `loadContacts()`: Loads contacts from a file.
-  `printList()`: Prints all contacts.
-  `printObj(node_t *data)`: Prints a single contact.
-  `print_Menu(WINDOW *menu_win, int highlight)`: Prints the menu with the current highlight.
-  `del_Menu(WINDOW *menu_win)`: Clears the menu.
### Menu Navigation
- Use the arrow keys to navigate the menu.
- Press Enter to select an option.
- Follow prompts to add, remove, insert, or find contacts as needed.
