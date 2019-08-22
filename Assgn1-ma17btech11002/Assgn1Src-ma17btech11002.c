#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <sys/mman.h>
#include <sys/time.h>
#include <math.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

double getcurrenttime()  //this is a function for getting to know the current time as mentioned in the pdf.
{
	struct timeval current;
	gettimeofday(&current, NULL);

	double i;
	i=current.tv_usec;
	i=i/1000;
	i=i+(current.tv_sec*1000);
	return i;

}
double func(char *a, int n)  //This is used in the below function.
{
	int i;double d=0;
	for(i=0;i<n;i++)
	{
		d=d*10 + (a[i]-'0');
	}
	return d;
}

double convintodoub(char *a)  //This function returns the double value from character.
{
	double d,n1,n2;
	int l;
	char *b ,*c;
	
	b=strtok(a, ".");
	c=strtok(NULL, ".");
	l=strlen(b);
	n1=func(b, l);
	l=strlen(c);
	n2=func(c, l);
	n2 *=pow(10,-l);
	d=n1+n2;
	return d;



}

int main(int argc, char** argl)
{
	  

	//process for creating shared memory.
	const int size=4096;
	const char *name="OS";
    double endtime;
	int fd;
	void *starttime;   //pointer for shared memory.

	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, size);

	starttime=mmap(0, size, PROT_WRITE, MAP_SHARED, fd, 0);
	pid_t pid;
	pid=fork();

	char **temp;
	int i;

	for(i=1;i<argc;i++)
	{
		temp[i-1]=argl[i];
	}
	temp[i-1]=NULL;

	if(pid<0)
	{
		printf("fork failed\n");
	}
	else if(pid==0)   //The child process.
	{
		

		sprintf(starttime, "%lf", getcurrenttime());
		execvp(temp[0],temp);

	}
	else        //The parent process.
	{
		wait(NULL);

		
		endtime=getcurrenttime();
		endtime=endtime-convintodoub((char*)starttime);
		printf("Elapsed time: %lf ms\n", endtime);

	}

	return 0;



}

