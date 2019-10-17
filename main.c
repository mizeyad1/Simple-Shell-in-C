#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <sys/types.h>
#include  <signal.h>
//Function Prototypes
void sighandler_t(int);
void printLog();

int main()
{
while(1){// Infinite Loop to get many commands after each other
printf("\nEnter a new command!\n");
//Flags to Handle Argument Count and Aysnch Execution Detection
int itIsACommandOnly=0;
int itIsACommandWithAnd=0;
int itIsACommandWithArgument=0;
int itIsACommandWithArgumentWithAnd=0;
//The Array of String that will contain the Arguments to execvp function
char * arrayOfString[3];
int flagArgumentOne=0;
int flagArgumentTwo=0;
char * inputString=(char*)malloc(200*sizeof(char));
fgets(inputString,200,stdin); //Getting User Input
char * token=strtok(inputString," ,\n"); //Getting the first token which is the command
//Allocating spaces for command and Arguments
char * command=(char*)malloc(60*sizeof(char));
char * firstArgument=(char*)malloc(60*sizeof(char));
char * secondArgument=(char*)malloc(60*sizeof(char));
strcpy(command,token);

token=strtok(NULL," ,\n"); //Getting the second token
if(token!=NULL)
{
flagArgumentOne=1;
strcpy(firstArgument,token);
token=strtok(NULL," ,\n");
}

if(token!=NULL) //Getting the third token if it is not NULL
{
flagArgumentTwo=1;
strcpy(secondArgument,token);
}
/*
if(flagArgumentOne==1)
printf("First Argument is %s",firstArgument);
else
printf("First Argument is not present\n");
if(flagArgumentTwo==1)
printf("Second Argument is %s",secondArgument);
else
printf("Second Argument is not present\n");*/




//Checking if command is exit()
if(strcmp("exit",command)==0)
{
 exit(0);

}
//Creating the Array of Strings
if(flagArgumentOne==0 && flagArgumentTwo==0)
{
//char *arrayOfString[2];
arrayOfString[0]=command;
arrayOfString[1]=NULL;
itIsACommandOnly=1;

}
else if (flagArgumentOne==1 && flagArgumentTwo==0)
{
//char*arrayOfString[3];
arrayOfString[0]=command;
if(strcmp("&",firstArgument)!=0){
arrayOfString[1]=firstArgument;
arrayOfString[2]=NULL;
itIsACommandWithArgument=1;
}
else
{
arrayOfString[1]=NULL;
itIsACommandWithAnd=1;
}
}
else{
//char* arrayOfString[3];
arrayOfString[0]=command;
arrayOfString[1]=firstArgument;
arrayOfString[2]=NULL;
itIsACommandWithArgumentWithAnd=1;


}


//Check if the execution is asynchronus
if(strcmp(firstArgument,"&")==0 || strcmp(secondArgument,"&")==0)
{

pid_t child_pid; //declaring a child process ID
//pid_t tpid;
int childStatus;
child_pid=fork(); //making a duplicate process
if(child_pid<0){
printf("Could not fork the process!\n");
exit(1);
}
if(child_pid==0)
{
if(execvp(arrayOfString[0],arrayOfString)<0) //Executing the command by providing first argument as a command and other argument as array of string of commands terminated with NULL
{
printf("Execution Failed!\n");
}

}

}
else
{


pid_t child_pid;
pid_t tpid;
int childStatus;
child_pid=fork();
if(child_pid<0){
printf("Could not fork the process!\n");
exit(1);
}
if(child_pid==0)
{

if(execvp(arrayOfString[0],arrayOfString)<0)
{
printf("Execution Failed!\n");
}
}
else{ //Wait for the child to terminate

     waitpid(child_pid,NULL,0);
}


}

signal(SIGCHLD,sighandler_t); // a Signal Handler to detect if a child process has terminated First Argument is type of signal to detect and 2nd Argument is the handler

}

    return 0;
}
void sighandler_t(int signNo){ //Signal Handler Function

 printLog(); //Print the log to a txt file

}
void printLog()
{
printf("Recieved a signal and should print to file!\n");
FILE* pFile=fopen("LogFile.txt","a");
fprintf(pFile,"Child Process has Terminated!\n");
fclose(pFile);
}
