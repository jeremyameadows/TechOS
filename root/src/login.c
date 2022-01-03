/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    login.c

    This file will be used to store all of the methods reguarding
    logging in and user accounts
*/ 

#include "header.h"

// uses bitwise xor to hash str using the key
void hash(char * str, char * key, char * result)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        result[i] = str[i] ^ key[i];
    }
    result[i] = '\0';
}

// gets all of the usernames from file and saves into linked list
void load_users()
{

    // create file pointer
    FILE * file;
    char *file_name = (char *)malloc(sizeof(char) * 20);
    strcpy(file_name, "user_info");

    // open user info file as read only
    file = fopen(file_name, "r");

    char * current_username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
    char * current_encrypted_password = (char *)malloc(sizeof(char) * KEY_LENGTH);
    char * current_password = (char *)malloc(sizeof(char) * KEY_LENGTH);
    strcpy(current_encrypted_password, "");
    strcpy(current_password, "");
    strcpy(current_username,"");
    int current_access_level;
    char str[2] = {'a', '\0'};
    
    // gets current access level from user
    str[0] = fgetc(file);

    // loops through each line
    while(str[0] != EOF)
    {
        // saves current access level
        current_access_level = (int)(str[0]-48);

        str[0] = fgetc(file);// comma

        str[0] = fgetc(file);// first letter of username
        
        // loops through username until it hits the comma that separates username from password
        while(str[0] != ',')
        {
            strcat(current_username, str);
            str[0] = fgetc(file);//gets next char
            
        }
        str[0] = fgetc(file);// first letter of password
        
        // loops through encrypted password from file
        for(int i = 0; i < MAX_PASSWORD_LENGTH-1; i++)
        {
            strcat(current_encrypted_password, str);
            str[0] = fgetc(file); // gets next char
        }
        current_encrypted_password[MAX_PASSWORD_LENGTH-1] = '\0'; // add null char

        // decrypt password
        hash(current_encrypted_password, HASH_KEY, current_password);

        // find ~ to mark end of actual password
        int index_of_password_end = -1;
        for(int i = 0; i < MAX_PASSWORD_LENGTH-1; i++)
        {
            if(current_password[i] == '~')
            {
                index_of_password_end = i;
                break;
            }
        }
        // truncate password at ~
        if(index_of_password_end != -1)
            current_password[index_of_password_end] = '\0';
        // add current user to linked list
        struct user * current_user = (struct user *)malloc(sizeof(struct user));

        current_user->access_level = current_access_level;

        current_user->username = (char *)malloc(sizeof(char) * MAX_USERNAME_LENGTH);
        strcpy(current_user->username, current_username);
        current_user->password = (char *)malloc(sizeof(char) * MAX_PASSWORD_LENGTH);

        strcpy(current_user->password, current_password);

        add_ll(users, current_user);

        // get next character
        str[0] = fgetc(file); //gets next char

        // reset strings for next iteration
        strcpy(current_encrypted_password, "");
        strcpy(current_password, "");
        strcpy(current_username,"");
    }

    free(current_username);
    free(current_password);
    free(current_encrypted_password);
    fclose(file);
}


void linked_list_to_file()
{
    // open file as write
    FILE * file;
    file = fopen("user_info", "w");

    struct ll_node * current_node = users->head;
    char * encrypted_password = (char *)malloc(sizeof(char) * KEY_LENGTH+1);
    char * temp = (char *)malloc(sizeof(char) * KEY_LENGTH+1);
    while(current_node != NULL)
    {
        // copy to temp so don't mess with ll password
        strcpy(temp, current_node->data->password);

        // add ~ to mark end of actual password
        strcat(temp, "~\0");
        int current_len = my_strlen(temp);

        // create random salt and pepper to end of password
        char * random_salt_pepper = (char *)malloc(sizeof(char) * KEY_LENGTH-current_len);
        for(int i = 0; i < MAX_PASSWORD_LENGTH-current_len-1; i++)
            random_salt_pepper[i] = (char)(rand()%(125-33)+33);

        
        hash(temp, HASH_KEY, encrypted_password);
        strcat(encrypted_password, random_salt_pepper);
        fprintf(file, "%d,%s,%s\n", current_node->data->access_level, current_node->data->username, encrypted_password);
        current_node = current_node->next;
    }

    free(encrypted_password);
    fclose(file);
}


// 0 user not unique
// 1 user created
int create_user(char * username, char * password, int a_lvl)
{
    // force username to lowercase
    for(int i = 0; username[i] != '\0'; i++)
        username[i] = tolower(username[i]);

    
    // see if username is unique
    if(does_user_exist(users, username) == 1)
        return 0;

    
    // create user struct
    struct user * new_user = (struct user *)malloc(sizeof(struct user));
    new_user->username = username;
    new_user->password = password;
    new_user->access_level = a_lvl;

    // add to linked list
    add_ll(users, new_user);

    // add changes to file
    linked_list_to_file();
    return 1;
}

// dont have acces to delete this user return 2
// successful deletion return 1
// user doesn't exist return 0
int remove_user(char * username_to_delete, int access_level)
{
    // remove user's node from linked list
    int result = remove_node(username_to_delete, access_level);
    if(result == 1)
    {
        linked_list_to_file();
        return result;
    }
    return result;
}

// dont have acces to change this user's password return 2
// successful deletion return 1
// user doesn't exist return 0
int change_password(char * username_to_change, int access_level, char * current_username)
{
    struct ll_node * user_node;// = (struct ll_node *)malloc(sizeof(struct ll_node));
    user_node = get_user(username_to_change);
    if(user_node == NULL) // user does not exist
    {
        return 0;
    }
    // user has permissions to change password
    else if(access_level > user_node->data->access_level || strcmp(current_username, user_node->data->username) == 0)
    {
        // get new password and store in linkedlist
        int is_valid_password;
        do
        {
            is_valid_password = get_password(user_node->data->password);
            if(is_valid_password == 1)
            {
                printf("Invalid password.\n");
                printf("Password must contain at least 1 uppercase, 1 lowercase, 1 digit, and 1 special character.\n");
                printf("Password must be between %d and %d in length.\n", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH-1);
            }

        }while(is_valid_password == 1); // loops until valid password is entered

        // sync file with linked list
        linked_list_to_file();
        return 1;
    }
    else // does not have permission
        return 2;
}

//if user is already an admin, returns 2
//if user is successfully made an admin, returns 1
//if user does not exist, returns 0
int addAdmin(char * username_to_change, int access_level){
    //creates a node to act as the current node
    struct ll_node * user_node;
    //sets value of node to desired user
    user_node = get_user(username_to_change);
    //if this user does not exist, enters this if statement
    if(user_node == NULL){
        return 0;
    }
    //if the user is already an admin, enters this if statement
    else if(user_node->data->access_level == 1){
        return 2;
    }
    //if neither of the previous conditions are met, enters this else statement
    else{
        //sets the user to admin
        user_node->data->access_level = 1;
        //saves the updated user to file
        linked_list_to_file();
        //returns 1 to the call function
        return 1;
    }
}

//if user trying to demote is root returns 3
//if user is already not an admin, returns 2
//if user is successfully stripped of their admin role, returns 1
//if user does not exist, returns 0
int removeAdmin(char * username_to_change, int access_level){
    //creates a node to act as the current node
    struct ll_node * user_node;
    //sets the value of the node to the desired user
    user_node = get_user(username_to_change);
    //if this user does not exist, enters this if statement
    if(user_node == NULL){
        return 0;
    }
    //if the user is already not an admin, enters this if statement
    else if(user_node->data->access_level == 0){
        return 2;
    }
    // user is root admin and should not be demoted
    else if(user_node->data->access_level == 2){
        return 3;
    }
    //user exists and can be demoted, enters this else statement
    else{
        //admin is removed from user
        user_node->data->access_level = 0;
        //updated user is saved to file
        linked_list_to_file();
        //returns value to call function
        return 1;
    }
}