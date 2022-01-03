/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    queue.c

    We implemented both a priority queue and a regular queue to handle the queues required for process handling
*/

#include "header.h"

// initializes a queue
void initialize_queue(struct queue * q)
{
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

// adds a process to the back
void enqueue(struct queue * q, struct pcb * data)
{
    struct node * add_node;
    add_node = (struct node *)malloc(sizeof(struct node));
    add_node->data = data;

    if(q->head == NULL && q->tail == NULL) // queue is empty
    {
        add_node->backwards_pointer = NULL;
        add_node->forwards_pointer = NULL;
        q->head = add_node;
        q->tail = add_node;
    }
    else
    {
        q->tail->forwards_pointer = add_node;
        add_node->backwards_pointer = q->tail;
        add_node->forwards_pointer = NULL;
        q->tail = add_node;
    }
    q->size++;
}

// NOT DONE NEED TO CONSIDER ONLY 1 ELEMENT IN QUEUE
struct pcb * dequeue(struct queue * q)
{
    struct pcb * return_pcb = (struct pcb *)malloc(sizeof(struct pcb));
    return_pcb->name = (char *)malloc(sizeof(char) * PROCESS_NAME_SIZE);

    if(q->size == 0)// there is nothing in the queue
    {
        printf("No pcb's to dequeue\n");
        return NULL;
    }
    else if(q->size == 1)
    {
        strcpy(return_pcb->name, q->head->data->name);
        return_pcb->class = q->head->data->class;
        return_pcb->priority = q->head->data->priority;
        return_pcb->state = q->head->data->state;
        return_pcb->is_suspended = q->head->data->is_suspended;

        q->head = NULL;
        q->tail = NULL;
        q->size--;
    }
    else
    {
        // dequeues the head
        strcpy(return_pcb->name, q->head->data->name);
        return_pcb->class = q->head->data->class;
        return_pcb->priority = q->head->data->priority;
        return_pcb->state = q->head->data->state;
        return_pcb->is_suspended = q->head->data->is_suspended;

        q->head->forwards_pointer->backwards_pointer = NULL;
        q->head = q->head->forwards_pointer;
        q->size--;
    }
    return return_pcb;
}

// priority queue based on pcb.priority
void enqueue_pq(struct queue * q, struct pcb * data)
{
    struct node * add_node;
    add_node = (struct node *)malloc(sizeof(struct node));
    add_node->data = data;

    if(q->head == NULL && q->tail == NULL) // queue is empty
    {
        add_node->backwards_pointer = NULL;
        add_node->forwards_pointer = NULL;
        q->head = add_node;
        q->tail = add_node;
    }
    else
    {
        // if it has the highest priority
        if(data->priority < q->head->data->priority)
        {
            q->head->backwards_pointer = add_node;
            add_node->forwards_pointer = q->head;
            add_node->backwards_pointer = NULL;
            q->head = add_node;
        }
        else if(data->priority >= q->tail->data->priority)// least priority
        {
            q->tail->forwards_pointer = add_node;
            add_node->forwards_pointer = NULL;
            add_node->backwards_pointer = q->tail;
            q->tail = add_node;
        }
        else // it's somewhere in between
        {
            struct node * next = q->head;
            while(1)
            {
                // loop until the next process is less than next
                if(add_node->data->priority < next->data->priority)
                {
                    next->backwards_pointer->forwards_pointer = add_node;
                    add_node->backwards_pointer = next->backwards_pointer;
                    add_node->forwards_pointer = next;
                    next->backwards_pointer = add_node;
                    break;
                }
                next = next->forwards_pointer;
            }
        }
    }
    q->size++;
}

void print_queue(struct queue * q)
{
    struct node * next = q->head;
    int counter = 1;
    if(q->size == 0)
    {
        printf("<Empty>\n");
        return;
    }
    while(next != NULL)
    {
        printf("%d. ", counter++);
        printProcess(next->data);
        next = next->forwards_pointer;
    }
}