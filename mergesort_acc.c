#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <openacc.h>


//Function to check the array is sorted
bool check_sorted(int* arr, int N){
	int i ;
	for(i=1; i < N; ++i){
		if(arr[i] < arr[i-1]){
			return false;
		}
	}
	return true;
}


void merge(int arr[], int l, int m, int r);

#pragma acc routine seq
int compare(int n1, int n2) { return (n1 < n2)? n1 : n2; }

void mergeSort(int arr[], int n)
{
	int currSize;  
	int leftStart; 
	             
	#pragma acc data copy(arr[0:n])
	{
	  for (currSize=1; currSize<=n-1; currSize = 2*currSize){
	  #pragma acc parallel loop
	   
	   for (leftStart=0; leftStart<n-1; leftStart += 2*currSize){

	       int mid = leftStart + currSize - 1;
	
	       int rightEnd = compare(leftStart + 2*currSize - 1, n-1);
	
	       if (mid < rightEnd) merge(arr, leftStart, mid, rightEnd);
	   }
	 }
	}
}

/* Function to merge the two haves */
 #pragma acc routine(merge) vector
 void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int ln = m - l + 1;
	int rn =  r - m;
	
	/* create temp arrays of left and right parts*/
	int *L, *R;
	 L = (int *)malloc(sizeof(int) * ln);
	 R = (int *)malloc(sizeof(int) * rn);
	/* Copy data to temp arrays L[] and R[] */
	 #pragma acc loop independent
	for (i = 0; i < ln; i++)
	    L[i] = arr[l + i];
	 #pragma acc loop independent
	for (j = 0; j < rn; j++)
	    R[j] = arr[m + 1+ j];
	
	/* Merge the left and right arrays to original array */
	i = 0;
	j = 0;
	k = l;
	
	while (i < ln && j < rn)
	{
	    if (L[i] <= R[j])
	    {
	        arr[k] = L[i];
	        i++;
	    }
	    else
	    {
	        arr[k] = R[j];
	        j++;
	    }
	    k++;
	}
	
	/* Copy the remaining elements of left part */
	
	while (i < ln)
	{
	    arr[k] = L[i];
	    i++;
	    k++;
	}
	
	/* Copy the remaining elements of right part */
	while (j < rn)
	{
	    arr[k] = R[j];
	    j++;
	    k++;
	}
	free(L);
	free(R);
}



int main(int argc, char ** argv)
 {
 	int n = atoi(argv[1]);;
	int i, *arr;
	double startTime, endTime;
	char *operation = "";
	char *verify = "-v";
	
	if (argc == 3){
		operation = argv[2];
	}
	
	arr = (int *)malloc(sizeof(int) * n);
	srand(time(0));
	for(i = 0; i < n; i++){
		arr[i] = rand() % n;
	}

	startTime = omp_get_wtime();
	mergeSort(arr,n);
	endTime = omp_get_wtime();
	printf("Size of the array is %d\n",n);
	if(!strcasecmp(verify, operation)){
		bool sorted = check_sorted(arr,n);
 		printf("The array is sorted:%s\n", sorted ? "true" : "false");
	}
	
	printf("\nTime: %g\n",endTime-startTime);
	
	exit(0);
}

