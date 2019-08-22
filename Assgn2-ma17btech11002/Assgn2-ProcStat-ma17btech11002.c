#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>      //various header files for the program to execute.

#include <sys/mman.h>
#include <sys/time.h>
#include <math.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



/*double getcurrenttime()  //this is a function for getting to know the current time as mentioned in the pdf.
{
	struct timeval current;
	gettimeofday(&current, NULL);

	double i;
	i=current.tv_usec;
	i=i/1000;
	i=i+(current.tv_sec*1000);
	return i;

}*/





double mean(double ar[],int n)     //function to calculate the mean of the values passed through an array.
{
	double result=0;int i;
	for(i=0;i<n;i++)
	{
		result=result+ar[i];
	}
	result=result/n;
	return result;
}

double median(double ar[],int n)    //function to caculate the median of values passed through an array.
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(ar[i]>ar[j])
			{
				double temp;
				temp=ar[i];
				ar[i]=ar[j];
				ar[j]=temp;
			}
		}

	}


	if(n%2==1)
	{
		return ar[n/2];
	}
	else
	{
		return (ar[n/2]+ar[(n/2)-1])/2;
	}


}                                                 

double deviation(double ar[],int n)            //function to caculate the standard deviation of values passed through an array.
{
	double x=mean(ar,n);
	

	double y=0;double z;

	for(int i=0;i<n;i++)
	{
		z=ar[i]-x;z=z*z;
		y=y+z;

	}
	y=y/(n-1);
	double k=sqrt(y);
	return k;
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

int main(int argc, char** argl)    //finally the main function.
{

	FILE* fp;
	FILE* fp1;              //various operations on files.
	fp=fopen(argl[1],"r");
	int n;
	fscanf(fp,"%d",&n);
	double array[n];int i;

	for(i=0;i<n;i++)
	{
		fscanf(fp,"%lf",&array[i]);
		

	}
	fclose(fp);
	

	const int size=4096;      //creating 3 shared memories.
	const char *name="os1";
	const char *fame="os2";
	const char *blame="os3";

	void *xx;void *yy; void *zz;
	int fd1,fd2,fd3;

	fd1 = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd1, size);

	fd2 = shm_open(fame, O_CREAT | O_RDWR, 0666);
	ftruncate(fd2, size);

	fd3 = shm_open(blame, O_CREAT | O_RDWR, 0666);
	ftruncate(fd3, size);

	

	xx=mmap(0, size, PROT_WRITE, MAP_SHARED, fd1, 0);

	yy=mmap(0, size, PROT_WRITE, MAP_SHARED, fd2, 0);

	zz=mmap(0, size, PROT_WRITE, MAP_SHARED, fd3, 0);


	

	

	pid_t p1;
	pid_t p2;         //foking the process so that we get 4 processes in total.
	p1=fork();
	p2=fork();



	
	if(p1<0&&p2<0)     //error statement.
	{
		printf("fork failed\n");
	}

	if(p1==0&&p2==0)  //worker process.
	{
		//sprintf(starttime, "%lf", getcurrenttime());
		sprintf(xx,"%lf",mean(array,n));
		

	}
	if(p1==0&&p2>0)    //worker process.
	{
		sprintf(yy,"%lf",median(array,n));

	}
	if(p1>0&&p2==0)    //worker process.
	{
		sprintf(zz,"%lf",deviation(array,n));

	}
	if(p1>0&&p2>0)     //parent process.
	{
		double xxx=convintodoub((char*)xx);
		double yyy=convintodoub((char*)yy);
		double zzz=convintodoub((char*)zz);
		fp1=fopen("result.txt","w");
		fprintf(fp1,"The mean is %lf.\nThe median is %lf.\nThe standard deviation is %lf.",xxx,yyy,zzz);
		fclose(fp1);

		/*endtime=getcurrenttime();
		endtime=endtime-convintodoub((char*)starttime);
		printf("Elapsed time: %lf ms\n", endtime);*/
        

	}
	
	return 0;

}