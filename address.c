#include "address.h"

void handle_signal(int sig)
{   
    fprintf(stdout, "\nReceived signal %d.\n", sig);
    delete_addresses();
    exit(sig);
}
struct Contact *ContactObj(char *name, char *surname, char *email, char *phone)
{
    struct Contact *contact = (struct Contact*)malloc(sizeof(struct Contact));
    contact->name = strdup(name);
    contact->surname = strdup(surname);
    contact->email = strdup(email);
    contact->phone = strdup(phone);
    return contact;
}
void add_address(struct Contact *contact)
{   
    if (contact == NULL)
        return;
    Node *address = create_node(contact);
    add_to_list(&head, address);
}
void insert_address_pos(struct Contact *contact, __uint16_t pos)
{   
    if (node_count(head) < pos){
        free(contact->name);
        free(contact->surname);
        free(contact->email);
        free(contact->phone);
        free(contact);
        printf("Error encountered.\nPress Enter to return.");
        return;
    }
    Node *address = create_node(contact);
    insert_to_list(&head, address, pos);
    printf("Contact inserted.\nPress Enter to return.");
}
void delete_address_pos(__uint16_t pos)
{   
    Node *temp = head;
    int i = 0;
    if (node_count(head) < pos-1 || node_count(head) == 0){
        printf("Position out of bounds.\nPress Enter to return.");
        temp = NULL;
        return;
    }
    while (temp != NULL && i != pos){
        temp = temp->next;
        i++;
    }
    struct Contact *address = (struct Contact *)temp->data;
    free(address->name);
    free(address->surname);
    free(address->email);
    free(address->phone);
    free(address);
    delete_node(&head, pos);
    temp = NULL;
    printf("Address deleted.\nPress Enter to return.");
}
void delete_addresses()
{   
    Node *temp = head;
    while (temp != NULL){
        struct Contact *contact = (struct Contact *)temp->data;
        free(contact->name);
        free(contact->surname);
        free(contact->email);
        free(contact->phone);
        temp = temp->next;
    }
    delete_list(&head);
}
void find_address_pos(__uint16_t pos)
{   
    if (pos > node_count(head) || node_count(head) == 0){
        printf("Position out of bounds.\nPress Enter to return.");
        return;
    }
    Node *node = find_node(&head, pos);
    if (node == NULL)
        return;
    struct Contact *address = node->data;
    printf("Contact: %s, %s, %s, %s\n", address->name, address->surname, address->email, address->phone);
}
void find_address(char *keyword)
{
    Node *temp = head;
    __uint16_t index = 0;
    if (strlen(keyword) >= __UINT16_MAX__){
        temp = NULL;
        return;
    }
    while (temp != NULL){
        struct Contact* address = temp->data;
        if (strcmp(address->name, keyword) == 0){
            printContact(address);
            index++;
            temp = temp->next;
            continue;
        }
        if (strcmp(address->surname, keyword) == 0){
            printContact(address);
            index++;
            temp = temp->next;
            continue;
        }
        if (strcmp(address->email, keyword) == 0){
            printContact(address);
            index++;
            temp = temp->next;
            continue;
        }
        if (strcmp(address->phone, keyword) == 0){
            printContact(address);
            index++;
            temp = temp->next;
            continue;
        }
        temp = temp->next;
    }
    if (index == 0)
        printf("No addresses found with this keyword (\"%s\").\nPress Enter to return.", keyword);
    temp = NULL;
}
void save_contacts()
{   
    FILE *file;
    Node *temp = head;
    char address_path[256];
    char *user_name = getlogin();
    if (head == NULL){
        printf("No contacts in memory.\nPress Enter to return.");
        return;
    }
    if (user_name == NULL)
        return;
    snprintf(address_path, sizeof(address_path), "/home/%s/addresses.csv", user_name);
    file = fopen(address_path, "a");
    while (temp != NULL){
        struct Contact* address = temp->data;
        fprintf(file, "%s,%s,%s,%s\n", address->name, address->surname, address->email, address->phone);
        temp = temp->next;
    }
    printf("Contacts saved.\nPress Enter to return.");
    temp = NULL;
    fclose(file);
}
void load_contacts()
{
    FILE *file;
    char address_path[256];
    char *user = getlogin();
    char buffer[__UINT16_MAX__];
    if (user == NULL){
        memset(buffer, 0, sizeof(buffer));
        return;
    }
    snprintf(address_path, sizeof(address_path), "/home/%s/addresses.csv", user);
    file = fopen(address_path, "r");
    if (file == NULL){
        printf("Couldn't open file.\nPress Enter to return.");
        memset(buffer, 0, sizeof(buffer));
        return;
    }
    if (feof(file)){
        printf("File is empty.\nPress any key to return.");
        memset(buffer, 0, sizeof(buffer));
        return;
    }
    while (fgets(buffer, __UINT16_MAX__, file)){
        char *name = strtok(buffer, ",");
        char *surname = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *phone = strtok(NULL, ",");
        phone[strcspn(phone, "\n")] = 0;
        struct Contact *address = ContactObj(name, surname, email, phone);
        add_address(address);
    }
    strtok(NULL, ",");
    memset(buffer, 0, sizeof(buffer));
    fclose(file);
    printf("Contacts loaded.\nPress Enter to return.");
}
void display_contacts()
{   
    Node *temp = head;
    __uint16_t i = 1;
    if (head == NULL){
        printf("No contacts.\nPress Enter to return.");
        temp = NULL;
        return;
    }
    while (temp != NULL){
        struct Contact* address = temp->data;
        printf("Contact %d: %s, %s, %s, %s", i, address->name, address->surname, address->email, address->phone);
        if (temp->next)
            printf("\n");
        ++i;
        temp = temp->next;
    }
    temp = NULL;
}
int is_valid_integer(char *input) 
{
    input[strcspn(input, "\n")] = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isdigit(input[i]) && input[i] != '-' && input[i] != '+') {
            return 0;
        }
    }
    return 1;
}
void printContact(struct Contact *contact)
{
    if (contact == NULL)
        return;
    Node *temp = head;
    struct Contact *address = ((struct Contact *)temp->data);
    printf("Contact: %s, %s, %s, %s\n", address->name, address->surname, address->email, address->phone);
    temp = temp->next;
}
void term_freeze(){
    signed p;
    while ((p = getchar()) != '\n' && p != EOF);
    getchar();
}
int user_function(char *input)
{   
    __uint8_t choice;
    printf("Enter function: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 2)
        input[2] = '\0';
    if (is_valid_integer(input) != 0){ 
        return -1;
    } else {
        choice = atoi(input);
        return choice;
    }
}
void user_choice()
{   
    printf("1. Add address\n");
    printf("2. Insert address\n");
    printf("3. Delete  address (by position)\n");
    printf("4. Delete all addresses\n");
    printf("5. Find address (by position)\n");
    printf("6. Find address (by keyword)\n");
    printf("7. Save contacts\n");
    printf("8. Load contacts\n");
    printf("9. Display contacts\n");
    printf("10. Exit\n");
    printf("\n");
}
void enter_info(char *name, char *surname, char *email, char *phone)
{
    printf("Enter contact's first name: ");
    scanf("%s", name);
    printf("Enter contact's last name: ");
    scanf("%s", surname);
    printf("Enter contact's email: ");
    scanf("%s", email);
    printf("Enter contact's phone number: ");
    scanf("%s", phone);
}
int user_position(char *buffer, __uint16_t pos)
{   
    printf("Enter position (Zero-based indexing): ");
    scanf("%s", buffer);
    if (is_valid_integer(buffer) != 0){ 
        printf("Invalid position.\nPress Enter to return.");
        return -1;
    } 
    pos = atoi(buffer);
    return pos;
}
