/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    process_handler.c

    This file will have all of the backbone functions for anything having to do with pcbs and processes
*/
#include "header.h"

//function to allocate memory to a pcb pointer and return if successful and return NULL if not
struct pcb* allocatePCB(){
    struct pcb* my_pcb = (struct pcb*)malloc(sizeof(struct pcb));
    if(my_pcb != NULL){
        return my_pcb;
    }
    else{
        return NULL;
    }
}
//function to free the memory of a pcb pointer returns 1 if successful and 0 if not
int freePCB(struct pcb* my_pcb){
    free(my_pcb);
    my_pcb = NULL;
    if(my_pcb == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
//function to setup a pcb with name, class, and priority as parameters and will return the pcb pointer, returns NULL if something goes wrong
struct pcb* setupPCB(char * name, char class, int priority, char * file){
    //checks to see if the inputs are valid
    if(name == NULL || priority < 0 || priority > 9){
        return NULL;
    }
    //allocates memory to pcb and checks to see if it worked
    struct pcb* my_pcb = allocatePCB();
    if(my_pcb == NULL){
        return NULL;
    }
    //allocates memory to the name
    my_pcb->name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);
    my_pcb->file_path = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    // removes new line character in name if it exists
    //assigns the input values and default values to the respective parts of the pcb
    strcpy(my_pcb->name, name);
    my_pcb->class = class;
    my_pcb->priority = priority;
    my_pcb->state = 0;
    my_pcb->is_suspended = 1;
    my_pcb->offset = 0;
    strcpy(my_pcb->file_path, file);
    return my_pcb;
}
//function to find a pcb by its name, returns the pcb if found and NULL if it is not found
struct pcb* findPCB(char * name){
    //checks to see if the name is longer than the max name length
    if(strlen(name)>PROCESS_NAME_SIZE){
        return NULL;
    }
    //node for running through the queue
    struct node * check = ready->head;
    //checks to see if the pcb is in the ready queue
    for(int i = 0; i<ready->size; i++){
        if(strcmp(check->data->name, name) == 0){
            return check->data;
        }
        check = check->forwards_pointer;
    }
    check = readySuspended->head;
    //checks to see if the pcb is in the queue for ready and suspended
    for(int i = 0; i<readySuspended->size; i++){
        if(strcmp(check->data->name, name) == 0){
            return check->data;
        }
        check = check->forwards_pointer;
    }
    check = blocked->head;
    //checks to see if the pcb is in the queue for blocked
    for(int i = 0; i<blocked->size; i++){
        if(strcmp(check->data->name, name) == 0){
            return check->data;
        }
        check = check->forwards_pointer;
    }
    check = blockedSuspended->head;
    //checks to see if the pcb is in the queue for blocked and suspended
    for(int i = 0; i<blockedSuspended->size; i++){
        if(strcmp(check->data->name, name) == 0){
            return check->data;
        }
        check = check->forwards_pointer;
    }
    //returns NULL if nothing is found
    return NULL;
}
//function to insert a pcb into its respective queue
void insertPCB(struct pcb * my_pcb){
    //inserts for the ready queue
    if(my_pcb->state == 0 && my_pcb->is_suspended == 1){
        enqueue_pq(ready, my_pcb);
    }
    //insert for the ready and suspended queue
    else if(my_pcb->state == 0 && my_pcb->is_suspended == 0){
        enqueue_pq(readySuspended, my_pcb);
    }
    //inserts for the blocked queue
    else if(my_pcb->state == 2 && my_pcb->is_suspended == 1){
        enqueue(blocked, my_pcb);
    }
    //inserts for the blocked and suspended queue
    else if(my_pcb->state == 2 && my_pcb->is_suspended == 0){
        enqueue(blockedSuspended, my_pcb);
    }
}
//function for removing a pcb
//returns -1 for a nonexistent or running process, 0 for a nonexistent process, 1 for successful removal
int removePCB(struct pcb * my_pcb){
    //nodes for going through the queues and removing one
    struct node * previous = NULL;
    struct node * current = NULL;
    struct node * next = NULL;
    //removal for a process in the ready queue
    if(my_pcb->state == 0 && my_pcb->is_suspended == 1){
        current = ready->head;
        next = current->forwards_pointer;
        //checks to see if the process is in the queue and will remove from the queue correctly depending on where and how many processes are in the queue
        for(int i = 0; i<ready->size; i++){
            if((strcmp(current->data->name, my_pcb->name) == 0) && (current->data->class == my_pcb->class) && (current->data->priority == my_pcb->priority) && (current->data->state == my_pcb->state) && (current->data->is_suspended == my_pcb->is_suspended)){
                if(previous == NULL && next == NULL){
                    ready->head = NULL;
                    ready->tail = NULL;
                    ready->size--;
                }
                else if(previous == NULL){
                    ready->head = next;
                    ready->head->backwards_pointer = NULL;
                    ready->size--;
                }
                else if(next == NULL){
                    ready->tail = previous;
                    ready->tail->forwards_pointer = NULL;
                    ready->size--;
                }
                else{
                    previous->forwards_pointer = next;
                    next->backwards_pointer = previous;
                    ready->size--;
                }
                return 1;
            }
            previous = current;
            current = next;
            next = current->forwards_pointer;
        }
        return 0;
    }
    //removal for a process in the ready and suspended queue
    else if(my_pcb->state == 0 && my_pcb->is_suspended == 0){
        current = readySuspended->head;
        next = current->forwards_pointer;
        //checks to see if the process is in the queue and will remove from the queue correctly depending on where and how many processes are in the queue
        for(int i = 0; i<readySuspended->size; i++){
            if((strcmp(current->data->name, my_pcb->name) == 0) && (current->data->class == my_pcb->class) && (current->data->priority == my_pcb->priority) && (current->data->state == my_pcb->state) && (current->data->is_suspended == my_pcb->is_suspended)){
                if(previous == NULL && next == NULL){
                    readySuspended->head = NULL;
                    readySuspended->tail = NULL;
                    readySuspended->size--;
                }
                else if(previous == NULL){
                    readySuspended->head = next;
                    readySuspended->head->backwards_pointer = NULL;
                    readySuspended->size--;
                }
                else if(next == NULL){
                    readySuspended->tail = previous;
                    readySuspended->tail->forwards_pointer = NULL;
                    readySuspended->size--;
                }
                else{
                    previous->forwards_pointer = next;
                    next->backwards_pointer = previous;
                    readySuspended->size--;
                }
                return 1;
            }
            previous = current;
            current = next;
            next = current->forwards_pointer;
        }
        return 0;
    }
    //removal for a process in the blocked queue
    else if(my_pcb->state == 2 && my_pcb->is_suspended == 1){
        current = blocked->head;
        next = current->forwards_pointer;
        //checks to see if the process is in the queue and will remove from the queue correctly depending on where and how many processes are in the queue
        for(int i = 0; i<blocked->size; i++){
            if((strcmp(current->data->name, my_pcb->name) == 0) && (current->data->class == my_pcb->class) && (current->data->priority == my_pcb->priority) && (current->data->state == my_pcb->state) && (current->data->is_suspended == my_pcb->is_suspended)){
                if(previous == NULL && next == NULL){
                    blocked->head = NULL;
                    blocked->tail = NULL;
                    blocked->size--;
                }
                else if(previous == NULL){
                    blocked->head = next;
                    blocked->head->backwards_pointer = NULL;
                    blocked->size--;
                }
                else if(next == NULL){
                    blocked->tail = previous;
                    blocked->tail->forwards_pointer = NULL;
                    blocked->size--;
                }
                else{
                    previous->forwards_pointer = next;
                    next->backwards_pointer = previous;
                    blocked->size--;
                }
                return 1;
            }
            previous = current;
            current = next;
            next = current->forwards_pointer;
        }
        return 0;
    }
    //removal for a process in the blocked and suspended queue
    else if(my_pcb->state == 2 && my_pcb->is_suspended == 0){
        current = blockedSuspended->head;
        next = current->forwards_pointer;
        //checks to see if the process is in the queue and will remove from the queue correctly depending on where and how many processes are in the queue
        for(int i = 0; i<blockedSuspended->size; i++){
            if((strcmp(current->data->name, my_pcb->name) == 0) && (current->data->class == my_pcb->class) && (current->data->priority == my_pcb->priority) && (current->data->state == my_pcb->state) && (current->data->is_suspended == my_pcb->is_suspended)){
                if(previous == NULL && next == NULL){
                    blockedSuspended->head = NULL;
                    blockedSuspended->tail = NULL;
                    blockedSuspended->size--;
                }
                else if(previous == NULL){
                    blockedSuspended->head = next;
                    blockedSuspended->head->backwards_pointer = NULL;
                    blockedSuspended->size--;
                }
                else if(next == NULL){
                    blockedSuspended->tail = previous;
                    blockedSuspended->tail->forwards_pointer = NULL;
                    blockedSuspended->size--;
                }
                else{
                    previous->forwards_pointer = next;
                    next->backwards_pointer = previous;
                    blockedSuspended->size--;
                }
                return 1;
            }
            previous = current;
            current = next;
            next = current->forwards_pointer;
        }
        return 0;
    }
    return -1;
}
//function to print processes
void printProcess(struct pcb * process)
{
    
        printf("<name:%s, class:", process->name);
        if(process->class == 's')
            printf("app, ");
        else
            printf("sys, ");
            
        printf("state:");
        if(process->state == 0)
            printf("ready, ");
        else if(process->state == 1)
            printf("running, ");
        else
            printf("blocked, ");
            
        printf("suspended_status:");
        if(process->is_suspended == 0)
            printf("suspended, ");
        else
            printf("not suspended, ");
            
        printf("priority:%d>\n", process->priority);
}