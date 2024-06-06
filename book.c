#include "book.h"
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
    printf("Received signal. Exiting... \n");
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
        if (first_name && last_name && phone_number)
        {
            first_name = strdup(first_name);
            last_name = strdup(last_name);
            phone_number = strdup(phone_number);
            char *last_token = strtok(NULL, ",");
            if (last_token && last_token[0] != '\n' && last_token && strcmp(last_token, "null") != 0)
            {
                last_token = strdup(last_token);
                struct Contact *contact = Contact(first_name, last_name, phone_number, last_token);
                addContact(contact);
            }
            else
            {
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
        move(i, 0);
        struct Contact *contact = ((struct Contact *)temp->data);
        if (contact->email != NULL && strcmp(contact->email, "null") != 0)
            printw("Name: %s, Surname: %s, Phone: %s, Email: %s\n",
                   contact->first_name,
                   contact->last_name,
                   contact->phone_number,
                   contact->email);
        if (contact->email == NULL || strcmp(contact->email, "null") == 0)
            printw("Name: %s, Surname: %s, Phone: %s\n",
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
        printw("Name: %s, Surname: %s, Phone: %s, Email: %s\n",
               contact->first_name,
               contact->last_name,
               contact->phone_number,
               contact->email);

    if (contact->email == NULL || strcmp(contact->email, "null") == 0)
        printw("Name: %s, Surname: %s, Phone: %s\n",
               contact->first_name,
               contact->last_name,
               contact->phone_number);
    temp = temp->next;
}
void printMenu(WINDOW *menu_win, int highlight)
{
    int x, y, i;
    x = 2;
    y = 1;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i)
    {
        if (highlight == i + 1)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}
void delMenu(WINDOW *menu_win)
{
    move(0, 0);
    clear();
    refresh();
}
int main()
{
    lock();
    signal(SIGINT, handle_signal);
    signal(SIGCONT, handle_signal);
    signal(SIGSEGV, handle_signal);
    signal(SIGTERM, handle_signal);
    user = getlogin();
    if (user == NULL)
        return 1;
    snprintf(filepath, sizeof(filepath), "/home/%s/Public/Books.csv", user);
    char first_name[30];
    char last_name[30];
    char phone_number[30];
    char email[30];
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int startx, starty;
    int highlight = 1;
    int choice = 0;
    int c;
    startx = (COLS - WIDTH) / 2;
    starty = (LINES - HEIGHT) / 2;
    // center
    int row;
    int col;
    WINDOW *menu_win;
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    refresh();
    curs_set(0);
    printMenu(menu_win, highlight);
    while (1)
    {
        c = wgetch(menu_win);
        switch (c)
        {
        case KEY_UP:
            if (highlight == 1)
                highlight = n_choices;
            else
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight == n_choices)
                highlight = 1;
            else
                ++highlight;
            break;
        case 10:
            choice = highlight;
            break;
        default:
            break;
        }
        printMenu(menu_win, highlight);
        if (choice == 1)
        {
            delMenu(menu_win);
            printw("Enter contact's first name: ");
            refresh();
            echo();
            scanw("%s", first_name);
            printw("Enter contact's last name: ");
            refresh();
            scanw("%s", last_name);
            printw("Enter contact's phone number: ");
            refresh();
            scanw("%s", phone_number);
            printw("Enter contact's email: ");
            refresh();
            scanw("%s", email);
            if (email[0] == '\n' || email == "null")
                strcpy(email, "null");
            struct Contact *contact = Contact(first_name, last_name, phone_number, email);
            strcpy(email, "");
            addContact(contact);
            choice = 0;
            highlight = 1;
            move(0, 0);
            clear();
            refresh();
            printMenu(menu_win, highlight);
        }
        if (choice == 2)
        {
            char keyword[UINT16_MAX];
            delMenu(menu_win);
            if (getNodeCount(&top) == 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "No contacts in memory!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (getNodeCount(&top) > 0)
            {
                echo();
                printw("Enter keyword: ");
                refresh();
                scanw("%s", keyword);
                removeContact(keyword);
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
        }
        if (choice == 3)
        {
            uint16_t pos = 0;
            delMenu(menu_win);
            refresh();
            echo();
            printw("Enter contact's first name: ");
            refresh();
            echo();
            scanw("%s", first_name);
            printw("Enter contact's last name: ");
            refresh();
            scanw("%s", last_name);
            printw("Enter contact's phone number: ");
            refresh();
            scanw("%s", phone_number);
            printw("Enter contact's email: ");
            refresh();
            scanw("%s", email);
            printw("Enter position: ");
            refresh();
            scanw("%hu", &pos);
            if (email[0] == '\n' || email == "null")
                strcpy(email, "null");
            struct Contact *contact = Contact(first_name, last_name, phone_number, email);
            insertContact(contact, pos);
            choice = 0;
            highlight = 1;
            move(0, 0);
            clear();
            refresh();
            printMenu(menu_win, highlight);
        }
        if (choice == 4)
        {
            uint16_t pos = 0;
            delMenu(menu_win);
            refresh();
            if (getNodeCount(&top) == 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "No contacts in memory!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (getNodeCount(&top) > 0)
            {
                int16_t pos_negative;
                printw("Enter position: ");
                refresh();
                scanw("%hu", &pos_negative);
                // pos < 0 : Read user input into a signed integer, then check if it's positive and copy it it into an unsigned int variable
                if (pos_negative >= 0)
                {
                    pos = pos_negative;
                    removeContactPOS(pos);
                    choice = 0;
                    highlight = 1;
                    move(0, 0);
                    clear();
                    refresh();
                    printMenu(menu_win, highlight);
                }
                if (pos_negative < 0)
                {
                    delMenu(stdscr);
                    getmaxyx(stdscr, row, col);
                    char *mesg = "Unsigned (negative) index!";
                    mvprintw(row / 2 - 1, (col - strlen(mesg)) / 2, "%s", mesg);
                    refresh();
                    getch();
                    choice = 0;
                    highlight = 1;
                    move(0, 0);
                    clear();
                    refresh();
                    printMenu(menu_win, highlight);
                }
            }
        }
        if (choice == 5)
        {
            char keyword[30];
            delMenu(menu_win);
            refresh();
            if (getNodeCount(&top) == 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "No contacts in memory!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (getNodeCount(&top) > 0)
            {
                printw("Enter keyword: ");
                echo();
                refresh();
                scanw("%s", keyword);
                findContact(keyword);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
        }
        if (choice == 6)
        {
            delMenu(menu_win);
            refresh();
            wipeContacts();
            getmaxyx(stdscr, row, col);
            char *mesg = "Contact(s) wiped!";
            mvprintw(row / 2 - 1, (col - strlen(mesg)) / 2, "%s", mesg);
            refresh();
            getch();
            choice = 0;
            highlight = 1;
            printMenu(menu_win, highlight);
        }
        if (choice == 7)
        {
            delMenu(menu_win);
            refresh();
            if (getNodeCount(&top) == 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "No contacts in memory!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (getNodeCount(&top) > 0)
            {
                saveContacts();
                getmaxyx(stdscr, row, col);
                char *mesg = "Contact(s) saved to file!";
                mvprintw(row / 2 - 1, (col - strlen(mesg)) / 2, "%s", mesg);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                printMenu(menu_win, highlight);
            }
        }
        if (choice == 8)
        {
            delMenu(menu_win);
            refresh();
            if (access(filepath, F_OK) == 0)
            {
                loadContacts();
                getmaxyx(stdscr, row, col);
                char *mesg = "Contact(s) loaded to memory!";
                mvprintw(row / 2 - 1, (col - strlen(mesg)) / 2, "%s", mesg);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (access(filepath, F_OK) != 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "Encountered problem with .csv file!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
        }
        if (choice == 9)
        {
            delMenu(menu_win);
            refresh();
            if (getNodeCount(&top) == 0)
            {
                getmaxyx(stdscr, row, col);
                char *mesg1 = "No contacts in memory!";
                char *mesg2 = "Press ENTER to return";
                mvprintw(row / 2 - 1, (col - strlen(mesg1)) / 2, "%s", mesg1);
                refresh();
                mvprintw(row - 1, (col - strlen(mesg2)) / 2, "%s", mesg2);
                refresh();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
            if (getNodeCount(&top) > 0)
            {
                printList();
                getch();
                choice = 0;
                highlight = 1;
                move(0, 0);
                clear();
                refresh();
                printMenu(menu_win, highlight);
            }
        }
        if (choice == 10)
        {
            delwin(menu_win);
            endwin();
            exit(0);
        }
    }
    endwin();
    unlock();
    return 0;
}
