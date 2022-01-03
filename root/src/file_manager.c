/*
    Nicolas Null, John Morgan and Jeremy Meadows
    Published Fall 2021
    file_manager.c

    This file will be used to handle all of the file management
*/ 

#include "header.h"

char * current_path;

// view the directory
// it's a function
void view_directory(){
    DIR *dir;
    FILE *file;
    char * user_input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * path = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * temp = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    int size_choice;
    printf("Enter the path to the directory you would like to view.\n");
    printf("Some examples: \n\'.\' to view current directory, \n\'root/path_to_directory\' for an absolute path\n\'./path_to_directory\' for a local path\nTechOS> ");
    
    fgets(user_input, MAX_USER_INPUT, stdin);
    char * new_line_index;

    // get rid of new line character
    if((new_line_index =strchr(user_input, '\n')) != NULL)
        *new_line_index = '\0';


    if(strcmp(user_input, ".") == 0) // print current directory
    {
        strcpy(path, current_path);
        dir = opendir(current_path);
    }
    else if(user_input[0] == '.' && user_input[1] == '/') // local path
    {
        memcpy(user_input, &user_input[2], MAX_USER_INPUT-1); // cut off the . at the begining
        strcpy(path, current_path);
        strcat(path, "/");
        strcat(path, user_input);
        dir = opendir(path);
        if(dir == NULL)
        {
            printf("Invalid path.\n");
            return;
        } 
    }
    else if(strstr(user_input, "root") != NULL) // absolute path
    {
        memcpy(user_input, &user_input[4], MAX_USER_INPUT-1); // cut off the root at the begining
        strcpy(path, "../");
        strcat(path, user_input);
        dir = opendir(path);
        if(dir == NULL)
        {
            printf("Invalid path.\n");
            return;
        }
    }
    else
    {
        printf("Invalid path.\n");
        return;
    }

    
    while (1)// get size choice if yes 1 if no 0
    {
        // now ask if user want to see the size of the file
        printf("Would you like to view the file sizes? Enter \'y\' for yes or \'n\' for no.\nTechOS> ");
        fgets(user_input, MAX_USER_INPUT, stdin);

        // get rid of new line character
        if((new_line_index =strchr(user_input, '\n')) != NULL)
            *new_line_index = '\0';

            
        if(strcmp(user_input, "n") == 0 || strcmp(user_input, "N") == 0)
        {
            size_choice = 0;
            break;
        }
        else if(strcmp(user_input, "y") == 0 || strcmp(user_input, "Y") == 0)
        {
            size_choice = 1;
            break;
        }
        else
        {
            printf("Invalid choice.\n");    
        }
    }
    
    struct dirent * next_file;
    int counter = 0;
    while((next_file=readdir(dir)) != NULL)
    {

        // don't print hidden directories
        if(strcmp(next_file->d_name, ".") == 0 || strcmp(next_file->d_name, "..") == 0)
            continue;


        printf("%s", next_file->d_name);

        // print size
        if(size_choice == 1)
        {
            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, next_file->d_name);
            file = fopen(temp, "r");
            int prev = ftell(file);
            fseek(file, 0L, SEEK_END);// less beans
            int size = ftell(file); // beans
            //fseek(file, prev, SEEK_SET);
            rewind(file);

            if(size >= 0)
            { 
                printf("\t%.2lfKB", size/1024.0);
            }
            else
            {
                printf("\tdirectory");
            }
            fclose(file);
        }
        printf("\n");
        counter++;
    }
    
    if(counter == 0)
        printf("<empty>");

    free(user_input);
    free(path);
    free(temp);
    closedir(dir);
}

void change_directory(){
    DIR *dir;
    char * user_input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * path = (char *)malloc(sizeof(char) * MAX_USER_INPUT);

    printf("Enter the path to the directory.\n");
    printf("Some examples:\n\'root/path_to_directory\' for an absolute path\n\'./path_to_directory\' for a local path\nTechOS> ");
    
    fgets(user_input, MAX_USER_INPUT, stdin);
    char * new_line_index;

    // get rid of new line character
    if((new_line_index =strchr(user_input, '\n')) != NULL)
        *new_line_index = '\0';

    printf("%s\n", user_input);

    if(strcmp(user_input, ".") == 0) // print current directory
    {
        strcpy(path, current_path);
        dir = opendir(current_path);
    }
    else if(user_input[0] == '.' && user_input[1] == '/') // local path
    {
        memcpy(user_input, &user_input[2], MAX_USER_INPUT-1); // cut off the . at the begining
        strcpy(path, current_path);
        strcat(path, "/");
        strcat(path, user_input);
        dir = opendir(path);
        if(dir == NULL)
        {
            printf("Invalid path.\n");
            return;
        }
        strcpy(current_path, path);
    }
    else if(strstr(user_input, "root") != NULL) // absolute path
    {
        memcpy(user_input, &user_input[4], MAX_USER_INPUT-1); // cut off the root at the begining
        strcpy(path, "../");
        strcat(path, user_input);
        dir = opendir(path);
        if(dir == NULL)
        {
            printf("Invalid path.\n");
            return;
        }
        strcpy(current_path, path);
    }
    else
    {
        printf("Invalid path.\n");
        return;
    }


    char * output_path = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    memcpy(user_input, &current_path[3], MAX_USER_INPUT-1); // cut off the ../ at the begining
    strcpy(output_path, "root\0");
    strcat(output_path, user_input);
    printf("\nSuccessful: Current directory is %s\n", output_path);
    free(user_input);
    free(path);
    free(output_path);
    closedir(dir);
}

void create_folder(){
    int check;
    char * temp = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * directory = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    printf("\nEnter the directory to be created.\nTechOS> ");
    fgets(temp, MAX_USER_INPUT, stdin);
    char * new_line_index;

    // get rid of new line character
    if((new_line_index =strchr(temp, '\n')) != NULL)
        *new_line_index = '\0';
    strcpy(directory, current_path);
    strcat(directory, "/");
    strcat(directory, temp);
    check = mkdir(directory,0777);
    
    if(!check){
        printf("\nDirectory created!\n");
    }
    else{
        printf("\nDirectory could not be created!\n");
    }
}


// deletes directory if it exists and is empty
void remove_folder(){
    DIR *dir;
    char * user_input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * path = (char *)malloc(sizeof(char) * MAX_USER_INPUT); 
    printf("Enter name of directory to be removed:\nTechOS> ");
    fgets(user_input, MAX_USER_INPUT, stdin);
    
    // get rid of new line character
    char * new_line_index;
    if((new_line_index =strchr(user_input, '\n')) != NULL)
        *new_line_index = '\0';
        
    strcpy(path, current_path);
    strcat(path, "/");
    strcat(path, user_input);

    dir = opendir(path);

    if(dir == NULL)
    {
        printf("Directory does not exist.\n");
        return;
    }
    
    //check if chosen directory has anything in it
    struct dirent *next_file;
    int counter = 0;
    while((next_file=readdir(dir)) != NULL)
    {
        if(++counter > 2) // has files or directories other than .. and .
            break;
    }

    if(counter > 2) // directory is not empty
    {
        printf("Directory is not empty. Empty directory before deleteing.\n");
    }
    else // delete directory
    {   
        int confirmation;
        

        while (1)
        {
            printf("Please enter \'c\' to confirm deletion of folder %s. Enter \'u\' to undo deletion.\nTechOS> ", user_input);
            fgets(user_input, MAX_USER_INPUT, stdin);
            // remove new line
            if((new_line_index =strchr(user_input, '\n')) != NULL)
                *new_line_index = '\0';
            if(strcmp(user_input, "c") == 0 || strcmp(user_input, "C") == 0)
            {
                rmdir(path);
                printf("Directory removed.\n");
                break;
            }
            else if(strcmp(user_input, "u") == 0 || strcmp(user_input, "C") == 0)
            {
                printf("Delete folder canceled.\n");
                break;
            }
            else
            {
                printf("Invalid choice\n");
            }
        }
    }

    free(user_input);
    free(path);
    closedir(dir);
}

void create_file(){
    FILE *file;
    char * temp = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * file_name = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    printf("Enter the file to be created:\nTechOS> ");
    fgets(file_name, MAX_USER_INPUT, stdin);
    char * new_line_index;
    // get rid of new line character
    if((new_line_index =strchr(file_name, '\n')) != NULL)
        *new_line_index = '\0';
    strcpy(temp, current_path);
    strcat(temp, "/");
    strcat(temp, file_name);
    file = fopen(temp, "r");
    if(file == NULL){
        file = fopen(temp, "w");
        if(file == NULL){
            printf("\nFile Could Not Be Created!\n");
        }
        else{
            printf("\nFile Created Successfully!\n");
        }
    }
    else{
        printf("File already exists!");
    }
}

void remove_file(){
    FILE *file;
    char * temp = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * removed = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    char * user_input = (char *)malloc(sizeof(char) * MAX_USER_INPUT);
    printf("\nEnter the name of the file to be deleted.\nTechOS> ");
    fgets(removed, MAX_USER_INPUT, stdin);

    char * new_line_index;
    // get rid of new line character
    if((new_line_index =strchr(removed, '\n')) != NULL)
        *new_line_index = '\0';
    strcpy(temp, current_path);
    strcat(temp, "/");
    strcat(temp, removed);
    file = fopen(temp, "r");
    if(file == NULL){
        printf("\nFile does not exist!\n");
        return;
    }

    // get confirmation of deletion
    while (1)
        {
            printf("Please enter \'c\' to confirm deletion of file %s. Enter \'u\' to undo deletion.\nTechOS> ", user_input);
            fgets(user_input, MAX_USER_INPUT, stdin);
            // remove new line
            if((new_line_index =strchr(user_input, '\n')) != NULL)
                *new_line_index = '\0';
            if(strcmp(user_input, "c") == 0 || strcmp(user_input, "C") == 0)
            {
                if(remove(temp) == 0)
                    printf("File removed.\n");
                else
                    printf("\nUnable to delete the file!\n");
                break;
            }
            else if(strcmp(user_input, "u") == 0 || strcmp(user_input, "C") == 0)
            {
                printf("Delete file canceled.\n");
                break;
            }
            else
            {
                printf("Invalid choice\n");
            }
        }
    // if(remove(temp) == 0){
    //     printf("\nDeleted file successfully!\n");
    // }
    // else{
    //     printf("\nUnable to delete the file!\n");
    // }

    free(user_input);
    free(temp);
    free(removed);
    fclose(file);
}