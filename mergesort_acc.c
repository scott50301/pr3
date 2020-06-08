#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
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
int min(int x, int y) { return (x<y)? x :y; }

/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(int arr[], int n)
{
	int curr_size;  // For current size of subarrays to be merged
	               // curr_size varies from 1 to n/2
	int left_start; 
	             
	#pragma acc data copy(arr[0:n])
	{
	  for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size){
	  #pragma acc parallel loop
	   
	   for (left_start=0; left_start<n-1; left_start += 2*curr_size){
	       // Find ending point of left subarray. mid+1 is starting
	       // point of right
	       int mid = left_start + curr_size - 1;
	
	       int right_end = min(left_start + 2*curr_size - 1, n-1);
	
	       // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
	       if (mid < right_end) merge(arr, left_start, mid, right_end);
	   }
	 }
	}
}

/* Function to merge the two haves */
 #pragma acc routine(merge) vector
 void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;
	
	/* create temp arrays of left and right parts*/
	int *L, *R;
	 L = (int *)malloc(sizeof(int) * n1);
	 R = (int *)malloc(sizeof(int) * n2);
	/* Copy data to temp arrays L[] and R[] */
	 #pragma acc loop independent
	for (i = 0; i < n1; i++)
	    L[i] = arr[l + i];
	 #pragma acc loop independent
	for (j = 0; j < n2; j++)
	    R[j] = arr[m + 1+ j];
	
	/* Merge the left and right arrays to original array */
	i = 0;
	j = 0;
	k = l;
	
	while (i < n1 && j < n2)
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
	
	while (i < n1)
	{
	    arr[k] = L[i];
	    i++;
	    k++;
	}
	
	/* Copy the remaining elements of right part */
	while (j < n2)
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
	char operation = "";
	char verify = "-v";
	
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
	if (strcmp(operation, verify) == 0){
		bool sorted = check_sorted(arr,n);
 		printf("%s\n", sorted ? "true" : "false");
	}
	
	printf("\nTime: %g\n",endTime-startTime);
	
	exit(0);

}

