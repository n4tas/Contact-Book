#include "book.h"
static node_t *top = NULL;
char filepath[256];
const char *user;
int lock()
{
    int fd = open(PATH, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        syslog(LOG_ERR, "Failed to open the lock file. Error code: %d\n", errno);
        return fd;
    }
    int lock = flock(fd, LOCK_EX | LOCK_NB);
    if (lock == -1)
    {
        syslog(LOG_ERR, "Failed to lock the instance. Error code: %d \n", errno);
        close(fd);
        return errno;
    }
    syslog(LOG_INFO, "Instance locked.");
    return 0;
}
void unlock()
{
    int fd = open(PATH, O_RDWR, 0666);
    if (fd == -1)
    {
        syslog(LOG_ERR, "Failed to open lock file for unlocking. Error code: %d\n", errno);
        close(fd);
        remove(PATH);
        return;
    }
    int unlock = flock(fd, LOCK_UN);
    if (unlock == -1)
    {
        syslog(LOG_ERR, "Failed to unlock. Error code: %d \n", errno);
        close(fd);
        remove(PATH);
        return;
    }
    syslog(LOG_INFO, "Successful Unlock.");
    close(fd);
    remove(PATH);
}
void handle_signal(int sig)
{
    printf("Received termination signal. Exiting... \n");
    wipeContacts(&top);
    Unlock();
    exit(sig);
}
struct Contact *Contact(char *first_name, char *last_name, char *phone_number, char *email)
{
    struct Contact *c = (struct Contact *)malloc(sizeof(struct Contact));
    c->first_name = strdup(first_name);
    c->last_name = strdup(last_name);
    c->phone_number = strdup(phone_number);
    c->email = strdup(email);
    return c;
}
void addContact(struct Contact *contact)
{
    if (&top == NULL)
        return;
    node_t *kontaktas = create_node(contact);
    insertEnd(&top, kontaktas);
}
void removeContact(char *keyword)
{
    if (&top == NULL || keyword == NULL)
        return;
    node_t *temp = top;
    uint16_t pos = 1;
    while (temp != NULL)
    {
        struct Contact *contact = (struct Contact *)temp->data;
        if (strcasecmp(contact->first_name, keyword) == 0)
        {
            removePos(&top, pos);
            free(contact);
            return;
        }
        if (strcasecmp(contact->last_name, keyword) == 0)
        {
            removePos(&top, pos);
            free(contact);
            return;
        }
        if (strcasecmp(contact->phone_number, keyword) == 0)
        {
            removePos(&top, pos);
            free(contact);
            return;
        }
        if (contact->email != NULL || strcasecmp(contact->email, "null") != 0)
        {
            if (strcasecmp(contact->email, keyword) == 0)
            {
                removePos(&top, pos);
                free(contact);
                return;
            }
        }
        temp = temp->next;
        pos++;
    }
}
void insertContact(struct Contact *contact, uint16_t pos)
{
    if (&top == NULL)
        return;
    node_t *kontaktas = create_node(contact);
    insertAfter(&top, pos, kontaktas);
}
void removeContactPOS(uint16_t pos)
{
    if (&top == NULL || pos == 0 || getNodeCount(&top) == 0 || pos < 0)
        return;
    node_t *temp = top;
    for (uint16_t i = 0; i < pos - 1; i++)
        temp = temp->next;
    struct Contact *contact = ((struct Contact *)temp->data);
    removePos(&top, pos);
    free(contact);
}
void findContact(char *keyword)
{
    if (&top == NULL || keyword == NULL)
        return;
    node_t *temp = top;
    while (temp != NULL)
    {
        struct Contact *contact = ((struct Contact *)temp->data);
        if (strstr(contact->first_name, keyword) || strcasecmp(contact->first_name, keyword) == 0)
            printObj(temp);
        if (strstr(contact->last_name, keyword) || strcasecmp(contact->last_name, keyword) == 0)
            printObj(temp);
        if (strstr(contact->phone_number, keyword) || strcasecmp(contact->phone_number, keyword) == 0)
            printObj(temp);
        if (contact->email != NULL || strcmp(contact->email, "null") != 0)
        {
            if (strstr(contact->email, keyword) || strcasecmp(contact->email, keyword) == 0)
                printObj(temp);
        }
        temp = temp->next;
    }
}
void wipeContacts()
{
    if (&top == NULL)
        return;
    node_t *temp = top;
    while (temp != NULL)
    {
        if (!(temp->data == NULL))
        {
            struct Contact *contact = (struct Contact *)temp->data;
            if (contact->email == NULL || strcmp(contact->email, "null") == 0)
            {
                free(contact->first_name);
                free(contact->last_name);
                free(contact->phone_number);
            }
            if (contact->email != NULL && strcmp(contact->email, "null") != 0)
            {
                free(contact->first_name);
                free(contact->last_name);
                free(contact->phone_number);
                free(contact->email);
            }
            free(temp->data);
        }
        temp = temp->next;
    }
    cleanup(&top);
}
void saveContacts()
{
    if (&top == NULL)
        return;
    node_t *temp = top;
    FILE *fp = fopen(filepath, "w");
    while (temp != NULL)
    {
        struct Contact *contact = ((struct Contact *)temp->data);
        if (contact->email == NULL || strcmp(contact->email, "null") == 0)
            fprintf(fp, "%s,%s,%s\n",
                    contact->first_name,
                    contact->last_name,
                    contact->phone_number);
        if (contact->email != NULL && strcmp(contact->email, "null") != 0)
            fprintf(fp, "%s,%s,%s,%s\n",
                    contact->first_name,
                    contact->last_name,
                    contact->phone_number,
                    contact->email);
        temp = temp->next;
        if (temp == NULL)
        {
            fclose(fp);
            return;
        }
    }
} 
void loadContacts()
{
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL)
        return;
    char Store[UINT16_MAX];
    char *email; // null option
    while (fgets(Store, UINT16_MAX, fp))
    {   
        char *first_name = strtok(Store, ",");
        char *last_name = strtok(NULL, ",");
        char *phone_number = strtok(NULL, ",");
        if (first_name && last_name && phone_number){
            first_name = strdup(first_name);
            last_name = strdup(last_name);
            phone_number = strdup(phone_number);
            char *last_token = strtok(NULL, ",");
            if (last_token && last_token[0] != '\n' && last_token && strcmp(last_token, "null") != 0) {
                last_token = strdup(last_token);
                struct Contact *contact = Contact(first_name, last_name, phone_number, last_token);
                addContact(contact);
            }
            else {
                struct Contact *contact = Contact(first_name, last_name, phone_number, "null");
                addContact(contact);
            }
            strtok(NULL, ",");
        }
    }
    fclose(fp);
}
void printList()
{
    if (top == NULL)
        return;
    node_t *temp = top;
    uint16_t i = 0;
    while (temp != NULL)
    {
        struct Contact *contact = ((struct Contact *)temp->data);
        if (contact->email != NULL && strcmp(contact->email, "null") != 0)
            printf("Name: %s, Surname: %s, Phone: %s, Email: %s\n",
                   contact->first_name,
                   contact->last_name,
                   contact->phone_number,
                   contact->email);
        if (contact->email == NULL || strcmp(contact->email, "null") == 0)
            printf("Name: %s, Surname: %s, Phone: %s\n",
                   contact->first_name,
                   contact->last_name,
                   contact->phone_number);
        temp = temp->next;
        ++i;
    }
}
void printObj(node_t *data)
{
    if (&top == NULL || data == NULL)
        return;
    node_t *temp = data;
    struct Contact *contact = ((struct Contact *)temp->data);
    if (contact->email != NULL && strcmp(contact->email, "null") != 0)
        printf("Name: %s, Surname: %s, Phone: %s, Email: %s\n",
               contact->first_name,
               contact->last_name,
               contact->phone_number,
               contact->email);

    if (contact->email == NULL || strcmp(contact->email, "null") == 0)
        printf("Name: %s, Surname: %s, Phone: %s\n",
               contact->first_name,
               contact->last_name,
               contact->phone_number);
    temp = temp->next;
}
int main()
{
    lock();
    signal(SIGINT, handle_signal);
    signal(SIGCONT, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGTERM, handle_signal);
    unsigned p;
    char first_name[30];
    char last_name[30];
    char phone_number[30];
    char email[30];
    user = getlogin();
    if (user == NULL)
        return 1;
    snprintf(filepath, sizeof(filepath), "/home/%s/Public/Books.csv", user);
    while (1){
    system("clear");
    print_choices();
    switch (user_choice){
        case 1:
            system("clear");
            printf("Enter contact's first name: ");
            scanf("%s", first_name);
            printf("Enter contact's last name: ");
            scanf("%s", last_name);
            printf("Enter contact's phone number: ");
            scanf("%s", phone_number);
            printf("Enter contact's email: ");
            scanf("%s", email);
            if (email[0] == '\n' || email == "null")
            {
                strcpy(email, "null");
                struct Contact *contact = Contact(first_name, last_name, phone_number, "null");
                strcpy(email, "");
                addContact(contact);
            }
            else
            {
                struct Contact *contact = Contact(first_name, last_name, phone_number, email);
                strcpy(email, "");
                addContact(contact);
            }
            break;

        case 2:
            char keyword[UINT16_MAX];
            if (getNodeCount(&top) == 0){
                printf("No contacts in memory!\n");
                printf("Press any key to return!");
            }
            if (getNodeCount(&top) > 0){
                printf("Enter keyword: ");
                scanf("%s", keyword);
                removeContact(keyword);
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;

        case 3:
            uint16_t pos = 0;
            printf("Enter contact's first name: ");
            scanf("%s", first_name);
            printf("Enter contact's last name: ");
            scanf("%s", last_name);
            printf("Enter contact's phone number: ");
            scanf("%s", phone_number);
            printf("Enter contact's email: ");
            scanf("%s", email);
            printf("Enter position: ");
            scanf("%hd", &pos);
            if (email[0] == '\n' || email == "null")
                strcpy(email, "null");
            struct Contact *contact = Contact(first_name, last_name, phone_number, email);
            insertContact(contact, pos);
            break;

        case 4:
            if (getNodeCount(&top) == 0){
                printf("No contacts in memory!\n");
                printf("Press any key to return!");
            }
            if (getNodeCount(&top) > 0){
                int16_t pos_negative;
                // pos < 0 : Read user input into a signed integer, then check if it's positive and copy it it into an unsigned int variable
                printf("Enter position: ");
                scanf("%hu", &pos_negative);
                if (pos_negative >= 0){
                    pos = pos_negative;
                    removeContactPOS(pos);
                }
                if (pos_negative < 0){
                    printf("Unsigned (negative) index!");
                    
                }
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;

        case 5:
            if (getNodeCount(&top) == 0){
                printf("No contacts in memory!\n");
                printf("Press any key to return!");
            }
            if (getNodeCount(&top) > 0){
                char keyword[30];
                printf("Enter keyword: ");
                scanf("%s", keyword);
                findContact(keyword);
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;

        case 6:
            wipeContacts();
            printf("Contact(s) wiped!");
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;

        case 7:
            if (getNodeCount(&top) == 0){
                printf("No contacts in memory!\n");
                printf("Press any key to return!");
            }
            if (getNodeCount(&top) > 0){
                saveContacts();
                printf("Contact(s) saved to file!");
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;

        case 8:
            if (access(filepath, F_OK) == 0){
                loadContacts();
                printf("Contact(s) loaded to memory!");
            }
            if (access(filepath, F_OK) != 0){
                printf("Encountered problem with .csv file!\n");
                printf("Press any key to return");
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;  

        case 9:
            system("clear");
            if (getNodeCount(&top) == 0){
                printf("No contacts in memory!\n");
                printf("Press any key to return!");
            } 
            if (getNodeCount(&top) > 0){
                printList();
            }
            while ((p = getchar()) != '\n' && p != EOF);
            getchar();
            break;  

        case 10:
            exit(0);
        }
    }
    unlock();
    return 0;
}
