# TechOS

Collaborators: <a href="https://github.com/Nicolasnull">Nicolas Null</a>, <a href="https://github.com/John-Morgan11">John Morgan</a>, and <a href="https://github.com/jeremyameadows">Jeremy Meadows</a><br>

Course: CS 450 (Operating Systems)

Course Professor: <a href="https://engineering.wvutech.edu/faculty-and-staff/bhanukiran-gurijala">Dr. BhanuKiran Gurijala</a><br>

College: <a href="https://www.wvutech.edu/">West Virginia University Institute of Technology</a>

<hr>

## Overview
This project was created as a part of our Operating Systems class taken during the Fall 2021 semester at WVU Tech. The project was broken into 5 parts. Each part built upon the last until we reached the final product which has been published in this repository. <br>

This system is a menu driven OS that has a list of commands that users can choose from and then complete the actions required by the choice. The commands can be found in the help functions. Descriptions of each command are also found in the help function. 

### Code info:

<ul>
<li>Language: C </li>
<li>Environment: gcc version 11.2.1 20210728 (Red Hat 11.2.1-1) (GCC)</li>
</ul>

<b>Part of the project allows for creation of files and folders. The root folder should be the farthest users can get to though our OS (ie TechOS should not be able to create files above that directory). This does force our actual source code to be slightly buried but it can be found in the src folder.</b>

### In order to run:
<ol>
<li>gcc execute.c -o execute.out
<li>gcc frontend_cli.c queue.c file_manager.c linked_list.c process_handler.c backend.c login.c -o a.out</li>
<li>./a.out</li>
</ol>
*** This may vary slightly if you try to run on another system other than Linux. The majority of this system should run on any version of C compiler but we did notice that the time.h header file did cause some unexpected issues running outside of the box we tested and debugged on.

<hr>

## Root Admin Login
username: admin<br> 
password: Admin123!<br>

** You can create your own users with different levels of access using the root admin login. However, to access the system initially, you must know at least one username and password we have preprogrammed into the system.

<hr>

## Miscellaneous Info
* The HelpFiles folder contains all of the text files that describe each functionality. This is where the descriptions from the help function and version are pulled from.<br><br>
* Files with .techos file extensions are used to simulate pcb (process control blocks). Processes are run through execute.c (written and given to us by the professor) to simulate different ready, blocked, and suspended processes.<br><br>
* User login info is stored in the user_info file. The passwords are hashed for extra security. The hash used is just a simple xor. The hashed key is stored in the header.h file. Each user has a access level, username and hashed password. <br><br>
* Commands are not case sensitive and whitespace is ignored.<br><br>
* To see all users, passwords and levels at login screen, uncomment line 1449 in backend.c. This helped us a ton during our debugging sessions.

<hr>

## Using Our Code
This code should not be used in any form by persons taking the same OS course at WVU Tech. Copying our code directly for use in your own project is dishonest. Otherwise, if you use any part of our code as inspiration for your own project, a simple citation via link to this repository would be nice.
