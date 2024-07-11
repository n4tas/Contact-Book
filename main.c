#include "address.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(){
    signal(SIGINT, handle_signal);
	signal(SIGCONT, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);
    char name[__UINT16_MAX__];
    char surname[__UINT16_MAX__];
    char email[__UINT16_MAX__];
    char phone[__UINT16_MAX__];
    char input[3];
    char buffer[__UINT16_MAX__];
    char keyword[__UINT16_MAX__];
    int choice;
    int pos;
    system("clear");
    while (1){
        user_choice();
        choice = user_function(input);
        if (choice == -1)
            continue;
        switch (choice){
            case 1:
                enter_info(name, surname, email, phone);
                struct Contact* contact = ContactObj(name, surname, email, phone);
                add_address(contact);
                printf("\nContact added.\nPress Enter to return.");
                term_freeze();
                break;
            case 2:
                enter_info(name, surname, email, phone);
                pos = user_position(buffer, pos);
                if (pos != -1){
                    struct Contact* address = ContactObj(name, surname, email, phone);
                    insert_address_pos(address, pos);
                }
                term_freeze();
                break;
            case 3:
                pos = user_position(buffer, pos);
                if (pos != -1){
                    delete_address_pos(pos);
                    term_freeze();
                }
                break;
            case 4:
                delete_addresses();
                printf("\nAddresses deleted.\nPress Enter to return.");
                term_freeze();
                break;
            case 5:
                pos = user_position(buffer, pos);
                if (pos != -1)
                    find_address_pos(pos);
                term_freeze();
                break;
            case 6:
                printf("Enter keyword: ");
                scanf("%s", keyword);
                printf("keyword: %s\n", keyword);
                printf("\n");
                find_address(keyword);
                term_freeze();
                break;
            case 7:
                save_contacts();
                term_freeze();
                break;
            case 8:
                load_contacts();
                term_freeze();
                break;
            case 9:
                display_contacts();
                term_freeze();
                break;
            case 10:
                delete_addresses();
                printf("\nClosing Address Book...\n\n");
                exit(0);
            default:
                break;  
        }   
    }
}
