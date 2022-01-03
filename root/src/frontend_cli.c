/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    frontend_cli.c

    This file will have all the required code to call the
    features in the backend through a Command Line Interface
*/
#include "header.h"
struct queue* ready;
struct queue* readySuspended;
struct queue* blocked;
struct queue* blockedSuspended;
struct linked_list * users;
int main(void)
{
    start_tech_os_cli();
    
    // keep asking for inputs until termination
    while(1)
    {
        get_input();
    }
    return 0;
}

void get_input()
{
    printf("\nEnter the desired task or enter \'help\' for additional help.\nTechOS> ");
    char * user_input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    setbuf(stdin, NULL);
    fgets(user_input, MAX_USER_INPUT, stdin);
    
    char * return_value = call_function(user_input);
    
    if(return_value != NULL)
        printf("%s\n", return_value);
}

void get_date_input(int * month, int * day, int * year)
{
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    int is_int;
    
    do// year
    {
        printf("Input the new year (must be after 1970): \nTechOS> ");
        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(isdigit(input[i]) == 0)
                is_int = 0;
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", year);
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }
        if(*year < 1970)
            printf("Invalid year.\n");
    } while (is_int == 0 || *year < 1970);


    do // month
    {
        printf("Input the new month as an integer (for example 1 is January)\nTechOS> ");
        
        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(isdigit(input[i]) == 0)
                is_int = 0;
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", month);
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }
            
        if(*month > 12 || *month < 1)
            printf("Invalid month.\n");
    } while (*month > 12 || *month < 1); // makes sure month is valid
    
    do// day
    {
        printf("Input the new day \nTechOS> ");

        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(isdigit(input[i]) == 0)
            {
                is_int = 0;
            }         
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", day);
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }

        
        if(is_date_valid(*month, *day, *year) == 0)
        {
            printf("Invalid day.\n");
            continue;
        }
    } while (is_int == 0 || is_date_valid(*month, *day, *year) == 0);
    
    
}

void get_time_format_input(int * format)
{
    int is_int;
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    while(1)
    {
        printf("Enter 0 for standard time and 1 for military time:\nTechOS>");
    
        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(isdigit(input[i]) == 0)
            {
                is_int = 0;
            }
                
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", format);
            if(*format == 0 || *format == 1)
                break;
            printf("Invalid choice.\n");
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }
    
    }   
}

//function to get the input for creating a pcb
void get_create_pcb_input(){
    //variables to hold all the inputs
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * nameInput = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * fileInput = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char classInput;
    int priorityInput = -1;
    struct pcb * my_pcb = (struct pcb*)malloc(sizeof(struct pcb));
    //loops until the user inputs a valid and unique name
    while(1){
        printf("Please enter the name of the process:\nTechOS>");
        fgets(nameInput, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        if(strlen(nameInput)>PROCESS_NAME_SIZE){
            printf("Must be less than %d characters.\n", PROCESS_NAME_SIZE);
            continue;
        }
        char * new_line_index;
        if((new_line_index =strchr(nameInput, '\n')) != NULL)
            *new_line_index = '\0';
        my_pcb = findPCB(nameInput);
        if(my_pcb!=NULL){
            printf("A process of this name already exists.\n");
            continue;
        }
        break;
    }
    //loops until the user inputs an a or s
    while(1){
        printf("Please enter the class of the process (a or s):\nTechOS>");
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        if(strlen(input)>2){
            printf("Length of input is too long.\n");
            continue;
        }
        if((input[0] == 'a') || (input[0] == 's')){
            classInput = (char)input[0];
        }
        else{
            printf("Input must be a or s.\n");
            continue;
        }
        break;
    }
    //loops until the user inputs and integer that is 0-9
    while(1){
        printf("Please enter the priority of the process (0-9, 0 - Highest, 9 - Lowest):\nTechOS>");
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        if(strlen(input)!=2){
            printf("The input is not the correct length.\n");
            continue;
        }
        if(isdigit(input[0]) == 0){
            printf("The input is not an integer.\n");
            continue;
        }
        sscanf(input, "%d", &priorityInput);
        if(priorityInput<0 || priorityInput>9){
            printf("Input must be between 0 and 9.\n");
            continue;
        }
        break;
    }
    while(1){
        printf("Please enter the relative filepath for the data of the process:\nTechOS>");
        fgets(fileInput, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        fileInput = trimwhitespace(fileInput);
        FILE *file;
        file = fopen(fileInput, "r");
        if(file == NULL){
            printf("This file does not exist.\n");
            continue;
        }
        fclose(file);
        break;
    }
    free(my_pcb);
    // gets rid of new line character in nameInput
    char * new_line_index;
    if((new_line_index =strchr(nameInput, '\n')) != NULL)
        *new_line_index = '\0';
    //calls the createPCB function in the backend with the inputs
    createPCB(nameInput, classInput, priorityInput, fileInput);
    free(input);
    free(nameInput);
    free(fileInput);
}

void get_process_name_input(char * name)
{
    printf("Enter the name of the process:\nTechOS>");
    fgets(name, PROCESS_NAME_SIZE, stdin);
    setbuf(stdin, NULL);

    //gets rid of new line character in name 
    char * new_line_index;
    if((new_line_index =strchr(name, '\n')) != NULL)
        *new_line_index = '\0';
}


void get_priority_input(int * new_priority)
{
    int is_int;
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    while(1)
    {
        printf("Enter a number between 0 and 9 for new priority:\nTechOS>");
    
        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(isdigit(input[i]) == 0)
            {
                is_int = 0;
            }
                
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", new_priority);
            if(*new_priority >= 0 && *new_priority <= 9)
                break;
            printf("Invalid choice.\n");
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }
    
    }   
}

// gets username and password from user
void get_add_user_input(char *entered_username, char *entered_password, int *a_lvl)
{
    printf("-----Creating New User-----\n");
    get_username(entered_username);
    // break if user already exists
    if(does_user_exist(users, entered_username) == 1)
        return;

    int is_valid_password;
    do
    {
        is_valid_password = get_password(entered_password);
        if(is_valid_password == 1)
        {
            printf("Invalid password.\n");
            printf("Password must contain at least 1 uppercase, 1 lowercase, 1 digit, and 1 special character.\n");
            printf("Password must be between %d and %d in length.\n", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH-1);
        }

    }while(is_valid_password == 1); // loops until valid password is entered
    


    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    do // loop until y or n is entered
    {
        printf("Would you like to give %s admin privileges (y/n)? \nTechOS> ", entered_username);
        fgets(input, MAX_USER_INPUT, stdin);
        
        if(input[1] != '\0' && input[1] != '\n')
        {
            printf("Invalid choice\n");
        }
        else if(tolower(input[0]) == 'y')
        {
            *a_lvl = 1;
            break;
        }
        else if(tolower(input[0] == 'n'))
        {
            *a_lvl = 0;
            break;
        }
        else
        {
            printf("Invalid choice\n");
        }
    } while (1);

    free(input);
}

void get_username(char * user_input)
{
    // get user input
    printf("Enter username:\nTechOS> ");
    fgets(user_input, MAX_USERNAME_LENGTH, stdin);

    // gets rid of newline character
    char * new_line_index;
    if((new_line_index =strchr(user_input, '\n')) != NULL)
        *new_line_index = '\0';

    // force username input to lowercase
    for(int i = 0; user_input[i] != '\0'; i++)
        user_input[i] = tolower(user_input[i]);

}

// will return 1 if password is invalid
// will return 0 if password is valid
// password must be of minimum length MIN_PASSWORD_LENGTH and at maximum MAX_PASS_WORD_LENGTH
// password must contain at least one or each of the following:
/*
    Uppercase letter
    Lowercase letter
    digit 1-9
    special character !"#$%&'()*+-./:;<=>?@
*/
int get_password(char * user_input)
{
        printf("Enter password: \nTechOS> ");
        fgets(user_input, MAX_PASSWORD_LENGTH, stdin);

        // gets rid of newline character
        char * new_line_index;
        if((new_line_index =strchr(user_input, '\n')) != NULL)
            *new_line_index = '\0';

        // TODO: add in part to check for password length, special characters, capital letters, and numbers
        int user_input_length = my_strlen(user_input);
        if(user_input_length >= MIN_PASSWORD_LENGTH && user_input_length < MAX_PASSWORD_LENGTH && valid_password_chars_contained(user_input) == 0)
            return 0;
        return 1;
}

// get the length of a string
int my_strlen(char * str)
{
    int counter = 0;
    while(str[counter] != '\0')// loop until null character is found
        counter++;
    return counter;
}

// returns 1 if str does not contain all required characters
// returns 0 if str does
int valid_password_chars_contained(char * str)
{
    int lower_counter = 0;
    int upper_counter = 0;
    int digit_counter = 0;
    int special_counter = 0;

    int current_index = 0;
    while(str[current_index] != '\0')
    {
        if(str[current_index] >= 'a' && str[current_index] <= 'z')// checks for lowercase
            lower_counter++;
        else if(str[current_index] >= 'A' && str[current_index] <= 'Z') // checks for uppercase
            upper_counter++;
        else if(str[current_index] >= '0' && str[current_index] <= '9') // checks for digit
            digit_counter++;
        else if(str[current_index] >= '!' && str[current_index] <= '/') // checks for !"#$%&'()*+-./
            special_counter++;
        else if(str[current_index] >= ':' && str[current_index] <= '@')// checks for :;<=>?@
            special_counter++;
        
        // breaks if all have already been found
        if(lower_counter > 0 && upper_counter > 0 && digit_counter > 0 && special_counter > 0)
            return 0;

        // increment to next char for next iteration
        current_index++;
    }
    return 1; // all types of chars not found
}