/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    linked_list.c

    We handled users by adding them into a linked list.
    We created our own linked list to handle user info.
*/

#include "header.h"

// initializes a linked list
void initialize_linked_list(struct linked_list * ll)
{
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
}


// adds to linked list
// return new size
int add_ll(struct linked_list * ll, struct user * added_user)
{
    struct ll_node * user_info = (struct ll_node *)malloc(sizeof(struct ll_node));
    user_info->data = added_user;
    user_info->next = NULL;
    if(ll->size == 0)
    {
        ll->head = user_info;
        ll->tail = user_info;
        user_info->previous = NULL;
    }
    else
    {
        ll->tail->next = user_info;
        user_info->previous = ll->tail;
        ll->tail = user_info;
    }
    return ++ll->size;
}

// searches for user in linked list
// returns -1 if not found and access level if found
// will be used to log in user
int login_check(struct linked_list * ll, char * username, char * password)
{
    struct ll_node * current_node = ll->head;
    while(current_node != NULL)
    {
        if(strcmp(username,current_node->data->username) == 0 && strcmp(password, current_node->data->password) == 0)
            return current_node->data->access_level;
        current_node = current_node->next;
    }
    return -1;
}

// will be used to make sure usernames are unique
// return 0 if does not exist
// return 1 if does exist
int does_user_exist(struct linked_list * ll, char * username)
{
    struct ll_node * current_node = ll->head;
    while(current_node != NULL)
    {
        if(strcmp(username,current_node->data->username) == 0)
            return 1;
        current_node = current_node->next;
    }
    return 0;
}

// prints out all of the users stored in the linked list
void print_ll(struct linked_list * ll)
{
    struct ll_node * current_node = ll->head;
    printf("Users:\n{\n");
    if(ll->head == NULL)
        printf("Empty\n");

    while(current_node != NULL)
    {
        printf("access level: %d  username: %s  password: %s\n", current_node->data->access_level,current_node->data->username, current_node->data->password);
        current_node = current_node->next;
    }

    printf("}\n");
}


// returns nodes of a user
// will return NULL if user doesn't exist
struct ll_node * get_user(char * username)
{
    //returned_user = (struct ll_node *)malloc(sizeof(struct ll_node));
    struct ll_node * returned_user = users->head;
    while(returned_user != NULL)
    {
        if(strcmp(username, returned_user->data->username) == 0) // usernames match
        {
            return returned_user;
        }
        returned_user = returned_user->next; // advances to next node
    }
    return NULL;
}


// return 2 if user does not have access level to remove node
// return 1 if successful deletion
// return 0 if user does not exist
int remove_node(char * username, int access_level)
{
    struct ll_node * current_node = users->head;
    while(current_node != NULL)
    {
        //printf("%d,%s,%s\n", current_node->data->access_level, current_node->data->username, current_node->data->password);
        if(strcmp(username, current_node->data->username) == 0) // usernames match
        {
            // make sure user has access level to delete this user
            if(access_level <= current_node->data->access_level)
                return 2;

            // removing first node
            if(current_node->previous == NULL)
            {
                current_node->next->previous = NULL;
                users->head = current_node->next;
                free(current_node);
            }
            else if(current_node->next == NULL) // removing last node
            {
                current_node->previous->next = NULL;
                users->tail = current_node->previous;
                free(current_node);
            }
            else // removing middle node
            {
                current_node->previous->next = current_node->next;
                current_node->next->previous = current_node->previous;
                free(current_node);
            }
            return 1;// successful deletion
        }
        current_node = current_node->next;
    }
    return 0; // username not found
}