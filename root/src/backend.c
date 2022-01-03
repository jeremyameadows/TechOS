/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    backend.c

    This file will be used to handle all of the backend
    or our system. So, all of the features will be added
    here. The way the features will be called will be 
    handled by the frontend (either cli or gui)
*/ 

#include "header.h"

// will hold any changes in system time in seconds
// this will allow us to keep the system clock running
// on the system which will include the changes the user makes
// in set_date and set_time
long change_in_system_time;
int time_type;
char ** history;
int access_level = -1;
char * current_users_username;
// this method will be called to initialize the system
// needs to handle the welcome message and initializing all of
// the system variables
void start_tech_os_cli()
{
    // Welcome message
    printf("Welcome to TechOS!\n");

    // initialize system time and date to local time and date
    // initialize time type to standard
    time_t current_time;
    time(&current_time);
    set_system_date_and_time((long)current_time);
    set_time_type(0);
    ready = (struct queue*)malloc(sizeof(struct queue));
    readySuspended = (struct queue*)malloc(sizeof(struct queue));
    blocked = (struct queue*)malloc(sizeof(struct queue));
    blockedSuspended = (struct queue*)malloc(sizeof(struct queue));
    history = malloc(sizeof(char *) * MAX_USER_INPUT);
    current_path = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    strcpy(current_path,"../");
    // Initialize history array
    for(int i = 0; i < HISTORY_SIZE; i++)
    {
        history[i] = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
        history[i] = NULL;
    }

    // initialize users
    users = (struct linked_list *)malloc(sizeof(struct linked_list));
    initialize_linked_list(users);
    load_users();
    current_users_username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
    login();
}


// large if tree of using our OS syntax to call our backend functions
// turns everything to lowercase so we are not case sensitive
// will return "invalid syntax" if the input does not match a function
char * call_function(char * function_name)
{
    format_input(function_name);

    // add to history if the call was anything but history
    if(strcmp(function_name, "history") != 0)
        add_to_history(function_name);

    // Large if tree of all functions
    if(strcmp(function_name, "displaydate") == 0) 
    {
        return get_date();
    }
    else if(strcmp(function_name, "displaytime") == 0)
    {
        return get_time(time_type);
    }
    else if(strcmp(function_name, "changedate") == 0)
    {
        int month, day, year;
        get_date_input(&month, &day, &year);
        set_date(month, day, year);
        
        // return new system date
        char * new_date = get_date();
        char * prompt = (char *)malloc(sizeof(char) * 99);
        strcpy(prompt, "The new system date is: \0");
        strcat(prompt, new_date);
        
        return prompt;
    }
    else if(strcmp(function_name, "changetimeformat") == 0)
    {
        int format;
        get_time_format_input(&format);
        set_time_type(format);
        return NULL;
    }
    else if (strcmp(function_name, "help") == 0)
    {
        help_function();
        return NULL;
    }
    else if (strcmp(function_name, "version") == 0)
    {
        get_version();
        return NULL;
    }
    else if (strcmp(function_name, "quit") == 0){
        terminate_tech_os();
        return NULL;
    }
    else if(strcmp(function_name, "history") == 0)
    {
        char * history_choice = history_function();
        if(history_choice == NULL)
            return NULL;
        return call_function(history_choice);
    }
    else if(strcmp(function_name, "createpcb") == 0)
    {
        get_create_pcb_input();
        return NULL;
    }
    else if(strcmp(function_name, "showallpcb") == 0)
    {
        showAllPCB();
        return NULL;
    }
    else if(strcmp(function_name, "showreadypcb") == 0)
    {
        showReadyPCB();
        return NULL;
    }
    else if(strcmp(function_name, "showblockedpcb") == 0)
    {
        showBlockedPCB();
        return NULL;
    }
    else if(strcmp(function_name, "suspendpcb") == 0)
    {
        char * process_name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);
        get_process_name_input(process_name);
        int found = suspendPCB(process_name);

        if(found == 1)
            return "Process not found.\0";
        else if(found == 2)
            return "Process already suspended.\0";
        else
            return "Process suspended.\0";
    }
    else if(strcmp(function_name, "resumepcb") == 0)
    {
        char * process_name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);
        get_process_name_input(process_name);
        int found = resumePCB(process_name);

        if(found == 1)
            return "Process not found.\0";
        else if(found == 2)
            return "Process already resumed.\0";
        else
            return "Process resumed.\0";
    }
    else if(strcmp(function_name, "setprioritypcb") == 0)
    {
        char * process_name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);
        get_process_name_input(process_name);
        struct pcb * process = findPCB(process_name);
        if(process == NULL)
            return "Process not found.\0";

        int new_priority; 
        get_priority_input(&new_priority);
        int same = setPriorityPCB(process, new_priority);

        if(same == 1)
            return "Process already had that priority level.\0";
        else
            return "Process priority updated\0";
    }
    else if(strcmp(function_name, "showpcb") == 0)
    {
        char * process_name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);
        get_process_name_input(process_name);
        int found = showPCB(process_name);

        if(found == 1)
            return "Process not found.\0";
        return NULL;
    }
    else if(strcmp(function_name, "dispatch") == 0)
    {
        dispatch();
        return NULL;
    }
    else if(strcmp(function_name, "viewdirectory") == 0){
        //view directory stuffs here
        view_directory();
        return NULL;
    }
    else if(strcmp(function_name, "changedirectory") == 0){
        //change directory stuffs here
        change_directory();
        return NULL;
    }
    else if(strcmp(function_name, "createfolder") == 0){
        //create folder stuffs here
        create_folder();
        return NULL;
    }
    else if(strcmp(function_name, "removefolder") == 0){
        //remove folder stuffs here
        remove_folder();
        return NULL;
    }
    else if(strcmp(function_name, "createfile") == 0){
        //create file stuffs here
        create_file();
        return NULL;
    }
    else if(strcmp(function_name, "removefile") == 0){
        //remove file stuffs here
        remove_file();
        return NULL;
    }
    else if(strcmp(function_name, "logout") == 0){
        // revoke access level
        access_level = -1;

        // reset history
        history[0] = NULL;

        // take back to login screen
        login();
        return NULL;
    }
    else if(strcmp(function_name, "createuser") == 0){
        // only call function if user has access
        if(access_level == 0)
            return "invalid syntax\0";
        
        // get username and password from user
        char * username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
        char * password = (char *)malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
        int a_lvl;
        get_add_user_input(username, password, &a_lvl);
        if(create_user(username, password, a_lvl) == 1)
            return "User created\0";
        else
            return "Username must be unique\0";

    }
    else if(strcmp(function_name, "removeuser") == 0){
        if(access_level < 1) // must be admin or higher to use this method
            return "invalid syntax\0";

        printf("-----Removing user-----\n");
        // get username from user
        char * username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
        get_username(username);
        int results = remove_user(username, access_level);
        if(results == 0)
            return "User does not exist\0";
        else if(results == 1)
            return "User successfully deleted\0";
        else
            return "You do not have permission to delete this user\0";

        free(username);
    }
    else if(strcmp(function_name, "changepassword") == 0){
        // change password
        printf("-----Change password-----\n");
        if(access_level == 0) // only have access to changing own username
        {
            if(change_password(current_users_username, access_level, current_users_username) == 1)
                return "Password successfully updated\0";
            else
                return "Password update failed\0";
        }
        else
        {
            char * username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
            get_username(username);
            int results = change_password(username, access_level, current_users_username);
            free(username);
            
            
            if(results == 1)// update successful
                return "Password successfully updated\0";
            else if(results == 0) // user does not exist
                return "User entered does not exist\0";
            else
                return "You do not have permission to update this user\'s password\0";
        }
        
    }
    //case for adding an admin
    else if(strcmp(function_name, "addadmin") == 0){
        //checks if the user is of appropriate access level to add an admin
        if(access_level >= 1){
            //creates a pointer to act as the username
            char * username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
            //gets the desired username for the user to change
            get_username(username);
            //calls the addAdmin method and saves the return value
            int results = addAdmin(username, access_level);
            //frees the pointer for username
            free(username);

            //if 1 is returned, admin is added successfully
            if(results == 1)
                return "Admin Added Successfully!\0";
            //if 0 is returned, user does not exist
            else if(results == 0)
                return "User not found!\0";
            //if 2 is returned, user is already an admin
            else if(results == 2)
                return "User is already an admin!\0";
            //if something random happened, this is printed
            else
                return "Admin Add Failed!\0";
        }
        //if user does not have permission for this command, user is alerted
        else{
            return "invalid syntax\0";
        }
    }
    //case for removing an admin
    else if(strcmp(function_name, "removeadmin") == 0){
        //checks if the user is of appropriate access level to remove an admin
        if(access_level == 2){
            //creates a pointer to act as a username
            char * username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
            //gets the desired username for the user to change
            get_username(username);
            //calls the removeAdmin and stores the returned value
            int results = removeAdmin(username, access_level);
            //frees the pointer
            free(username);

            //if result is 1, admin removed successfully
            if(results == 1)
                return "Admin Removed Successfully!\0";
            //if 0 is returned, user was not found
            else if(results == 0)
                return "User not found!\0";
            //if 2 is returned, user is already not an admin
            else if(results == 2)
                return "User is already not an admin!\0";
            // cannot demote root admin
            else if(results == 3)
                return "Cannot demote root admin!\0";
            //if anything else happens, this is returned
            else
                return "Admin Remove Failed!\0";
        }
        //if user does not have access to this command, user is alerted
        else{
            return "invalid syntax\0";
        }
    }
    else
    {
        return "invalid syntax\0";
    }
}

//help method to print corrent results based on user input
//uses large group of if statements to account for each txt file
//prints contents of each txt file character by character
void help_function()
{
    printf("\nWhat command would you like help with?\nEnter \'help\' to describe the help function and get a complete list of TechOS syntax.\nTechOS>");
    char * help_user = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    setbuf(stdin, NULL);
    fgets(help_user, MAX_USER_INPUT, stdin);

    format_input(help_user);

    char c;
    if (strcmp(help_user, "displaydate") == 0)
    {
        FILE *file = fopen("HelpFiles/display_date.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "changedate") == 0)
    {
        FILE *file = fopen("HelpFiles/change_date.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "changetimeformat") == 0)
    {
        FILE *file = fopen("HelpFiles/change_time_format.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "history") == 0)
    {
        FILE *file = fopen("HelpFiles/history.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "displaytime") == 0)
    {
        FILE *file = fopen("HelpFiles/display_time.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "quit") == 0)
    {
        FILE *file = fopen("HelpFiles/terminate_tech_os.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "version") == 0)
    {
        FILE *file = fopen("HelpFiles/version_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    //help file for basic user
    else if (strcmp(help_user, "help") == 0 && access_level < 1)
    {
        FILE *file = fopen("HelpFiles/help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    //help file for admin user
    else if (strcmp(help_user, "help") == 0 && access_level == 1)
    {
        FILE *file = fopen("HelpFiles/help_admin.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    //help file for root admin user
    else if (strcmp(help_user, "help") == 0 && access_level == 2)
    {
        FILE *file = fopen("HelpFiles/help_root_admin.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "createpcb") == 0)
    {
        FILE *file = fopen("HelpFiles/create_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "showallpcb") == 0)
    {
        FILE *file = fopen("HelpFiles/show_all_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "showreadypcb") == 0)
    {
        FILE *file = fopen("HelpFiles/show_ready_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "showblockedpcb") == 0)
    {
        FILE *file = fopen("HelpFiles/show_blocked_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "suspendpcb") == 0)
    {
        FILE *file = fopen("HelpFiles/suspend_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "resumepcb") == 0)
    {
        FILE *file = fopen("HelpFiles/resume_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "setprioritypcb") == 0)
    {
        FILE *file = fopen("HelpFiles/set_priority_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "showpcb") == 0)
    {
        FILE *file = fopen("HelpFiles/show_pcb_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "dispatch") == 0)
    {
        FILE *file = fopen("HelpFiles/dispatch_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "viewdirectory") == 0)
    {
        FILE *file = fopen("HelpFiles/view_directory_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "changedirectory") == 0)
    {
        FILE *file = fopen("HelpFiles/change_directory_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "createfolder") == 0)
    {
        FILE *file = fopen("HelpFiles/create_folder_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "removefolder") == 0)
    {
        FILE *file = fopen("HelpFiles/remove_folder_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "createfile") == 0)
    {
        FILE *file = fopen("HelpFiles/create_file_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "removefile") == 0)
    {
        FILE *file = fopen("HelpFiles/remove_file_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "createuser") == 0 && access_level > 0)
    {
        FILE *file = fopen("HelpFiles/create_user.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "removeuser") == 0 && access_level > 0)
    {
        FILE *file = fopen("HelpFiles/remove_user.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "changepassword") == 0 && access_level > 0)
    {
        FILE *file = fopen("HelpFiles/change_password.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "changepassword") == 0 && access_level == 0)
    {
        FILE *file = fopen("HelpFiles/change_password_self.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "addadmin") == 0 && access_level >= 1)
    {
        FILE *file = fopen("HelpFiles/add_admin.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "removeadmin") == 0 && access_level == 2)
    {
        FILE *file = fopen("HelpFiles/remove_admin.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else if (strcmp(help_user, "logout") == 0)
    {
        FILE *file = fopen("HelpFiles/logout_help.txt", "r");
        c = fgetc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(file);
        }
        printf("\n");
        fclose(file);
    }
    else
    {
        printf("Invalid Help Request\n");
    }
}
//method to get the current running version of the operating system
void get_version()
{
    //prints the contents of the version file character by character
    char c;
    FILE *file = fopen("HelpFiles/version.txt", "r");
    c = fgetc(file);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(file);
    }
    printf("\n");
    fclose(file);
}
// makes everything lowercase and gets rid of whitespace
void format_input(char * str)
{
    // make sure not case sensitive and no whitespace
    int j = 0;
    for(int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
    {
        if(str[i] != ' ')
        {
            str[j] = tolower(str[i]);
            j++;
        }
    }
    str[j] = '\0';
}
// this will set the change in system date and time
void set_system_date_and_time(long new_time)
{
    time_t current_time;
    time(&current_time);
    change_in_system_time = current_time - new_time;
}

// This will allow you to see the date
// NEED TO FREE THE CHAR RETURNED AFTER DISPLAYING TO USER
char * get_date()
{
    char * date = (char*)malloc(sizeof(char) * DATE_STRING_LENGTH);
    date[0] = '\0';
    //set up the tm variable
    time_t current_time;
    time(&current_time);
    current_time -= change_in_system_time;

    struct tm * info = localtime(&current_time);


    // add the day of the week
    switch(info->tm_wday)
    {
        case(0):
            strcat(date, "Sunday \0");
            break;
        case(1):
            strcat(date, "Monday \0");
            break;
        case(2):
            strcat(date, "Tuesday \0");
            break;
        case(3):
            strcat(date, "Wednesday \0");
            break;
        case(4):
            strcat(date,"Thursday \0");
            break;
        case(5):
            strcat(date, "Friday \0");
            break;
        default:
            strcat(date, "Saturday \0");
    }

    // add the month
    switch(info->tm_mon)
    {
        case(0):
            strcat(date, "January \0");
            break;
        case(1):
            strcat(date, "February \0");
            break;
        case(2):
            strcat(date, "March \0");
            break;
        case(3):
            strcat(date, "April \0");
            break;
        case(4):
            strcat(date,"May \0");
            break;
        case(5):
            strcat(date, "June \0");
            break;
        case(6):
            strcat(date, "July \0");
            break;
        case(7):
            strcat(date, "August \0");
            break;
        case(8):
            strcat(date, "September \0");
            break;
        case(9):
            strcat(date, "October \0");
            break;
        case(10):
            strcat(date, "November \0");
            break;
        default:
            strcat(date,"December \0");
    }

    // add the day
    char num[5];
    sprintf(num, "%0#2d", info->tm_mday); // turns an int into a string
    strcat(date, strcat(num,", "));

    // add the year
    sprintf(num, "%d", info->tm_year+1900); // years is the number of years since 1900
    strcat(date, num);

    info = NULL;
    return date;
}

// will get the system time
// 0 will display in standard time
// 1 will display in military time
// must free return value after displaying result
char * get_time(int type)
{
    char * am_pm = (char *)malloc(sizeof(char) * 4);
    char * return_time = (char*)malloc(sizeof(char) * 10);
    return_time[0] = '\0';
    //set up the tm variable
    time_t current_time;
    time(&current_time);
    current_time -= change_in_system_time;

    struct tm * info = localtime(&current_time);

    char num[3];

    if(type == 1) //military time
    {
        // get hour
        sprintf(num, "%0#2d", info->tm_hour);
        strcat(return_time, strcat(num, ":"));

        // get minute
        sprintf(num, "%0#2d", info->tm_min);
        strcat(return_time, strcat(num,":"));

        // get seconds
        sprintf(num, "%0#2d", info->tm_sec);
        strcat(return_time, num);
    }
    else// standard time
    {
        // hours
        if(info->tm_hour > 12) // PM
        {
            am_pm = " PM\0";
            sprintf(num, "%d", info->tm_hour - 12);
            strcat(return_time, strcat(num,":"));
        }
        else if(info->tm_hour == 12) // noon
        {
            am_pm = " PM\0";
            sprintf(num, "%d", info->tm_hour);
            strcat(return_time, strcat(num,":"));
        }
        else if(info->tm_hour > 0) // AM
        {
            am_pm = " AM\0";
            sprintf(num, "%d", info->tm_hour);
            strcat(return_time, strcat(num,":"));
        }
        else // midnight
        {
            am_pm = " AM\0";
            sprintf(num, "%d", 12);
            strcat(return_time, strcat(num,":"));
        }
        // minutes
        sprintf(num, "%0#2d", info->tm_min);
        strcat(return_time, strcat(num,":"));

        

        //seconds
        sprintf(num, "%0#2d", info->tm_sec);
        strcat(return_time, strcat(num, am_pm));
    }
    return return_time;
}

// sets the time type to either standard or military
// 0 for standard 1 for military
void set_time_type(int var)
{
    time_type = var;
}


// set the date of the system
void set_date(int month, int day, int year)
{
    time_t current_time;
    time(&current_time);

    struct tm * info = localtime(&current_time);

    info->tm_mon = month-1;
    info->tm_mday = day;
    info->tm_year = year-1900;

    change_in_system_time = current_time - mktime(info);// updates the change_in_system_time
    
}


// returns if a day of the month is valid
// 1 if true 0 if false
int is_date_valid(int month, int day, int year)
{
    int leap_year; // 1 for yes 0 for no
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(day <= 31 && day >=1)
                return 1;
            else
                return 0;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(day <= 30 && day >=1)
                return 1;
            else
                return 0;
            break;
        default:
        
            if(year % 400 == 0)
                leap_year = 1;
            else if(year % 100 == 0)
                leap_year = 0;
            else if(year % 4 == 0)
                leap_year = 1;
            else    
                leap_year = 0;
            
            if(leap_year == 1)
            {
                if(day <= 29 && day >= 0)
                    return 1;
                else
                    return 0;
            }
            else
            {
                if(day <= 28 && day >= 0)
                    return 1;
                else
                    return 0;
            }
    }
}

//asks the user if they actually wants to quit. If so, prints a goodbye message then exits the program and if not, the program will continue as normal
void terminate_tech_os(){
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    int is_valid_input = 0;
    do
    {
        printf("Are you sure you want to quit? (y/n)\nTechOS> ");
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        format_input(input);

        if(strcmp(input, "y") == 0)
        {
            free(history);
            free(ready);
            free(readySuspended);
            free(blocked);
            free(blockedSuspended);
            printf("Goodbye!\n");
            exit(0);
        }
        else if(strcmp(input, "n") == 0)
            break;
        printf("Invalid Choice.\n");
    }while(is_valid_input == 0);
}


// BONUS history
void add_to_history(char * str)
{
    //shift to make room for new syntax call
    for(int i = HISTORY_SIZE-2; i >= 0; i--)
    {
        history[i+1] = history[i];
    }

    // add new syntax call
    history[0] = str;
}

// shows history and gives option to recall same function
char * history_function()
{
    int num_of_history = 0;
    
    for(int i = 0; i < HISTORY_SIZE; i++)
    {
        if(history[i] != NULL)
        {
            if(++num_of_history == 1)
                printf("The recent calls are:\n");
            printf("%d> %s\n", i+1, history[i]);
        }
        else
        {
            break;
        }
    }

    if(num_of_history == 0)
    {
        printf("There is no recent history\n");
        return NULL;
    }
    
    // makes sure input is valid
    int is_int;
    int history_choice;
    char * input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    while(1)
    {
        printf("If you would like to re-run one of the above commands, enter the cooresponding number. Otherwise enter -1 to continue\nTechOS> ");
    
        // makes sure input is an integer
        fgets(input, MAX_USER_INPUT, stdin);
        setbuf(stdin, NULL);
        is_int = 1;
        for(int i = 0; is_int == 1 && (input[i] != '\0' && input[i] != '\n'); i++)
        {
            if(input[i] == '-')
                continue;
            if(isdigit(input[i]) == 0)
            {
                is_int = 0;
            }
                
        }

        if(is_int != 0) // is an integer
        {
            sscanf(input, "%d", &history_choice);
            if(history_choice == -1)
                return NULL;
            if(history_choice >= 1 && history_choice <= num_of_history)
                return history[history_choice-1];
            printf("Invalid choice.\n");
        }
        else // was not an integer
        {
            printf("Must enter an integer.\n");
            continue;
        }
    }
}
//function to create a pcb and insert it from the user input given by the get_create_pcb function in the frontend
void createPCB(char * name, char class, int priority, char * file){
    //allocate memory for a pcb
    struct pcb* my_pcb = (struct pcb*)malloc(sizeof(struct pcb*));
    //use the the setup function from the process handler
    my_pcb = setupPCB(name, class, priority, file);
    //if it worked insert it using the insert function from the process handler
    if(my_pcb != NULL){
        insertPCB(my_pcb);
    }
    else{
        printf("Something went wrong.");
    }
}


void showAllPCB()
{
    showReadyPCB();
    showBlockedPCB();
}

void showReadyPCB()
{
    printf("Ready Queue:\n{\n");
    print_queue(ready);
    printf("}\nReady Suspended Queue:\n{\n");
    print_queue(readySuspended);
    printf("}\n");
}

void showBlockedPCB()
{
    printf("Blocked Queue:\n{\n");
    print_queue(blocked);
    printf("}\nBlocked Suspended Queue\n{\n");
    print_queue(blockedSuspended);
    printf("}\n");
}

// finds the process with matching name
// removes from appropriate queue
// sets value to suspended
// puts back into appropriate queue
// returns 0 if process exists 1 if not 2 if already suspended
int suspendPCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }
    
    // sees if process was already suspended
    if(process->is_suspended == 0)
        return 2;
    
    removePCB(process);
    process->is_suspended = 0;
    insertPCB(process);
    return 0;
}

// finds the process with matching name
// removes from appropriate queue
// sets value to not suspended
// puts back into appropriate queue
// returns 0 if process exists 1 if not and 2 if already not suspended
int resumePCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }

    // sees if process was already not suspended
    if(process->is_suspended == 1)
        return 2;

    removePCB(process);
    process->is_suspended = 1;
    insertPCB(process);
    return 0;
}

// finds the process with matching name
// removes from appropriate queue
// sets value to blocked
// puts back into appropriate queue
// returns 0 if process exists 1 if not and 2 if already blocked
int blockPCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }

    // sees if process was already suspended
    if(process->state == 2)
        return 2;

    removePCB(process);
    process->state = 2;
    insertPCB(process);
    return 0;
}

// finds the process with matching name
// removes from appropriate queue
// sets value to ready
// puts back into appropriate queue
// returns 0 if process exists 1 if not 2 if already unblocked
int unblockPCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }

    // sees if process was already unblocked
    if(process->state == 0)
        return 2;
    removePCB(process);
    process->state = 0;
    insertPCB(process);
    return 0;
}

// finds the process with matching name
// removes from appropriate queue
// sets priority value
// puts back into appropriate queue
// returns 0 if priority updated 1 if priority stayed the same
// need to have already checked that valid process
int setPriorityPCB(struct pcb * process, int new_priority)
{
    if(process->priority == new_priority)
        return 1;

    removePCB(process);
    process->priority = new_priority;
    insertPCB(process);
    return 0;
}

// finds the process with matching name
// prints information

int showPCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }

    printProcess(process);
}

// finds the process with matching name
// removes from appropriate queue
// frees up memory that was allocated
int deletePCB(char * name)
{
    struct pcb * process = findPCB(name);
    
    // makes sure the process actually existed
    if(process == NULL)
    {
        return 1;
    }

    removePCB(process);
    freePCB(process);
    return 0;
}

//function to trim whitespace for filepath
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;

}

void dispatch(){
    //while ready queue is not empty
    while(ready->size != 0){
        //creates first portion of the executable command
        char * execute = (char *)malloc(sizeof(char)*100);
        strcpy(execute, "./execute.out \0");

        //get pcb in front of ready queue
        struct pcb * pcb_in_cpu = findPCB(ready->head->data->name);
        
        // remove current pcb from ready queue
        removePCB(pcb_in_cpu); 

        char *nameOfFile = pcb_in_cpu->file_path;
        char * offset = (char *)malloc(sizeof(char)*10);
        sprintf(offset, "%d", pcb_in_cpu->offset + 1);

        strcat(execute,nameOfFile);
        strcat(execute," ");
        strcat(execute,offset);
        //execute command returns a value
        int status = system(execute)/256;//system(execute);
        
        //status =0;
        //if process is done
        if(status == 0){
            //will delete process
            printf("Process %s is done.\n", pcb_in_cpu->name);
            deletePCB(pcb_in_cpu->name);
        }
        else{
            //will handle interrupt
            pcb_in_cpu->offset = status % 256;
            printf("Process %s has stopped at %d.\n", pcb_in_cpu->name, pcb_in_cpu->offset);
            insertPCB(pcb_in_cpu);
            suspendPCB(pcb_in_cpu->name);
            blockPCB(pcb_in_cpu->name);
            resumePCB(pcb_in_cpu->name);
        }
        // handle unblocking
        if(ready->size == 0 && blocked->size != 0) // 100% chance
        {
            pcb_in_cpu = blocked->head->data;
            unblockPCB(pcb_in_cpu->name);
        }
        else if(ready->size !=0 && blocked->size != 0) // 50% chance
        {
            
            // generates a random number between 0 and RAND_MAX
            int random_number = rand();

            // set threshold to be 50% of threshold
            int threshold = RAND_MAX/2;

            if(random_number > threshold) // if random number surpasses threshold unblock process
            {
                pcb_in_cpu = blocked->head->data;
                unblockPCB(pcb_in_cpu->name);
            }
            
        }
        pcb_in_cpu = NULL;
        free(pcb_in_cpu);
        free(execute);
        free(offset);
    }
}

// sets access level
void login(void)
{
    //print_ll(users);
    char * entered_username = (char *)malloc(sizeof(char)*MAX_USERNAME_LENGTH);
    char * entered_password = (char *)malloc(sizeof(char)*MAX_PASSWORD_LENGTH);
    do
    {
        printf("Enter username: TechOS> ");
        fgets(entered_username, MAX_USERNAME_LENGTH, stdin);

        // gets rid of newline character
        char * new_line_index;
        if((new_line_index =strchr(entered_username, '\n')) != NULL)
            *new_line_index = '\0';

        // forces to lowercase so username won't be case sensitive
        for(int i = 0; entered_username[i] !='\0'; i++)
            entered_username[i] = towlower(entered_username[i]);


        printf("Enter password: TechOS> ");

        // gets rid of newline character
        fgets(entered_password, MAX_PASSWORD_LENGTH, stdin);
        
        if((new_line_index =strchr(entered_password, '\n')) != NULL)
            *new_line_index = '\0';

        access_level = login_check(users, entered_username, entered_password);

        if(access_level < 0) // invalid credentials
            printf("Invalid username or password.\n");
        else // correct credentials
            strcpy(current_users_username, entered_username);
    } while (access_level < 0); // loop until correct user info is entered
    
    free(entered_username);
    free(entered_password);
    //printf("%d", access_level);
}