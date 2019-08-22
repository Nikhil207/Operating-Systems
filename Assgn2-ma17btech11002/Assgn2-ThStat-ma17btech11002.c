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
#include <pthread.h>



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





double m1,m2,m3;int n;
double ar[500];
void *mean(void *param)   //THe mean function.

{
	double result=0;int i;
	for(i=0;i<n;i++)
	{
		result=result+ar[i];
	}
	result=result/n;
	m1= result;

}

void *median(void *param)    //The median function.
{
	


	if(n%2==1)
	{
		m2= ar[n/2];
	}
	else
	{
		m2= (ar[n/2]+ar[(n/2)-1])/2;
	}


}
 
void *deviation(void *param)     //The standard deviation function.
{
	
	

	double y=0;double z;

	for(int i=0;i<n;i++)
	{
		z=ar[i]-m1;z=z*z;
		y=y+z;

	}
	y=y/(n-1);
	double k=sqrt(y);
	m3= k;
}

int main(int argc, char** argl)   //finally the main function.
{

	FILE* fp;
	FILE* fp1;
	fp=fopen(argl[1],"r");
	int nn;
	fscanf(fp,"%d",&nn);
	double array[nn];int i;n=nn;

	for(i=0;i<nn;i++)
	{
		fscanf(fp,"%lf",&array[i]);
		ar[i]=array[i];
		

	}
	fclose(fp);int j;double temp;

	for(int i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(ar[i]>ar[j])
			{
				temp=ar[i];
				ar[i]=ar[j];
				ar[j]=temp;
			}

		}

	}
    //sprintf(starttime, "%lf", getcurrenttime());
    pthread_t tid[3];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&tid[0],&attr, mean, array); //thread 1
    pthread_join(tid[0],NULL);

    pthread_create(&tid[1],&attr, median, array); //thread 2
    pthread_join(tid[1],NULL);

    pthread_create(&tid[2],&attr, deviation, array); //thread 3
    pthread_join(tid[2],NULL);




    fp1=fopen("result.txt","w");
    fprintf(fp1,"The mean is %lf.\nThe median is %lf.\nThe standard deviation is %lf.",m1,m2,m3);
	fclose(fp1);



	/*endtime=getcurrenttime();
		endtime=endtime-convintodoub((char*)starttime);
		printf("Elapsed time: %lf ms\n", endtime);*/


return 0;
}