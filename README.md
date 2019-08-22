# Operating-Systems
exploring
This child process will execute the command through a function called execvp().
Before the child executes the command we record the current time stamp .The parent
Process waits till the child process gets terminated since used wait(). Once the child process
erminates the parent process recorrds the time stamp again and prints the difference as
Elapsed time. For this we use a mechanism for the program to share variables using shared
memory. We use a function called gettimeoftheday() to record the current time stamp. For
Inter process communication between the parent process and the child process ,the contents
of the shared memory pointer are assigned the struct timeval representing the starting time.
The output is the diffence of the time stamps and is in milliseconds.
