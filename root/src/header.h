/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    header.h

    The header file will be used to declare all of the
    function prototypes, constants and including statemnets
    Having this header will allow us to use functions and
    constants accross multiple files
*/

// ***makes sure that header doesn't get initialized multiple times in compilation
#ifndef HEADER_H
#define HEADER_H

// ***all of our structures
struct pcb
{
    char * name; // must be at least 8 char + null char
    char class; // 'a' for application 's' for system process
    int priority; // must be between 0-9. lower the number the higher the priority
    int state; // 0 is ready, 1 is running, 2 is blocked
    int is_suspended; // 0 for suspended and 1 for not suspended
    char * file_path; // hold path to the .techos file
    int offset; // how much of the process has already been completed
};

struct node
{
    struct node * forwards_pointer; // will point to the next node in list
    struct node * backwards_pointer; // will point to the previous node in list
    struct pcb * data; // the actual information stored in the node
};

struct queue
{
    struct node * head; // pointer to the first node
    struct node * tail; // pointer to the last element in the list
    int size; // keeps track of the number of elements in queue
};

struct user
{
    char * username;
    char * password;
    int access_level;// 0 is regular user, 1 is admin, and 2 is superadmin
};

struct ll_node
{
    struct ll_node * next;
    struct ll_node * previous;
    struct user * data;
};

struct linked_list
{
    struct ll_node * head;
    struct ll_node * tail;
    int size;
};

// ***all of our include statements
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>

// ***all of our function declarations (prototypes)
// backend
void start_tech_os_cli();
void set_system_date_and_time(time_t new_time);
char * get_date();
char * get_time(int type);
void set_time_type(int var);
void set_date(int month, int day, int year);
char * call_function(char * function_name);
int is_date_valid(int month, int day, int year);
void format_input(char * str);
void help_function();
void get_version();
void terminate_tech_os();
void add_to_history(char * str);
char * history_function();
void createPCB(char * name, char class, int priority, char * file);
int deletePCB(char * name);
void showAllPCB();
void showReadyPCB();
void showBlockedPCB();
int suspendPCB(char * name);
int resumePCB(char * name);
int blockPCB(char * name);
int unblockPCB(char * name);
int setPriorityPCB(struct pcb * process, int new_priority);
int showPCB(char * name);
char *trimwhitespace(char *str);
void dispatch();
void login(void);
int valid_password_chars_contained(char * str);


//for frontend_cli
void get_input();
void get_date_input(int * month, int * day, int * year);
void get_time_format_input(int * format);
void get_create_pcb_input();
void get_process_name_input(char * name);
void get_priority_input(int * new_priority);
void get_add_user_input(char *username, char *password, int *a_lvl);
void get_username(char * user_input);
int get_password(char * user_input);

// for queue
void initialize_queue(struct queue * q);
void enqueue(struct queue * q, struct pcb *data);
struct pcb * dequeue(struct queue * q);
void print_queue(struct queue * q);
void enqueue_pq(struct queue * q, struct pcb *data);

// for process handler
struct pcb* allocatePCB();
int freePCB(struct pcb* my_pcb);
struct pcb* setupPCB(char * name, char class, int priority, char * file);
struct pcb* findPCB(char * name);
void insertPCB(struct pcb * my_pcb);
int removePCB(struct pcb * my_pcb);
void printProcess(struct pcb * process);

extern struct queue* ready;
extern struct queue* readySuspended;
extern struct queue* blocked;
extern struct queue* blockedSuspended;
extern struct linked_list * users;

// for file manager
extern char * current_path;
void view_directory();
void change_directory();
void create_folder();
void remove_folder();
void create_file();
void remove_file();

// for login and users
void hash(char * str, char * key, char * result);
void load_users();
void linked_list_to_file();
int create_user(char * username, char * password, int a_lvl);
int remove_user(char * username_to_delete, int access_level);
int change_password(char * username_to_change, int access_level, char * current_username);
int addAdmin(char * username_to_change, int access_level);
int removeAdmin(char * username_to_change, int access_level);
int my_strlen(char * str);

// for linked list
void initialize_linked_list(struct linked_list * ll);
int add_ll(struct linked_list * ll, struct user * added_user);
int does_user_exist(struct linked_list * ll, char * username);
int login_check(struct linked_list * ll, char * username, char * password);
void print_ll(struct linked_list * ll);
struct ll_node * get_user(char * username);
int remove_node(char * username, int access_level);

// ***all of our global constant declarations
#define DATE_STRING_LENGTH 30
#define MAX_USER_INPUT 99
#define MAX_FILE_LENGTH 200
#define HISTORY_SIZE 5
#define PROCESS_NAME_SIZE 10
#define HASH_KEY "JVJVKKx4GuKz8wVZnj1Ek0lvkpb846KkfCIYld4RqBBHeUFks3lkgSXETqVsuikVcHwKlTvuPEDBLAXC0X1sNu4HbkwJr7hNJfk4DnbcdKenVkOHKNuVOG3evgMrqtYPJowCo1B96442c0q11wXtVVi5WPLSze1WE6Tg83v8V2zovx9XjmXvcKdYn0VnkrLQDOwy3RBSWRIdLOZ7GKR1wcime5qDsl5b5DKR9s1UHJ7QtSPtcQ6jEePMbbLH5h7nPow\0"
#define KEY_LENGTH 256
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MIN_PASSWORD_LENGTH 8



#endif

#if (defined(_WIN32) || defined(__WIN32__))
#define mkdir(A, B) mkdir(A)
#endif