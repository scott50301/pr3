#include<stdlib.h>
#include<stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
#include <openacc.h>


//Function to test if the output is in asending order or not
bool test(int a[], int n) {
	int i;
	for( i=1; i < N; ++i){
		if(array[i] < array[i-1]){
			return false;
		}
	}
	return true;
}

/* Function to merge */
void merge(int arr[], int l, int m, int r);

// Utility function to find minimum of two integers
#pragma acc routine seq
int min(int x, int y) { return (x<y)? x :y; }

/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(int arr[], int n)
{
	int curr_size;  // For current size of subarrays to be merged
	               // curr_size varies from 1 to n/2
	int left_start; // For picking starting index of left subarray
	               // to be merged
	#pragma acc data copy(arr[0:n])// pcopying (R[0:n2])
	      {
	  for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
	 {
	  #pragma acc parallel loop
	   // Pick starting point of different subarrays of current size
	   for (left_start=0; left_start<n-1; left_start += 2*curr_size)
	   {
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

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[]
*/
 #pragma acc routine(merge) vector
 void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;
	
	/* create temp arrays */
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
	
	/* Merge the temp arrays back into arr[l..r]*/
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
	
	/* Copy the remaining elements of L[], if there are any */
	
	while (i < n1)
	{
	    arr[k] = L[i];
	    i++;
	    k++;
	}
	
	/* Copy the remaining elements of R[], if there are any */
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
	char operation;
	
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
	bool sorted = test(arr,n);
	printf("%s\n", sorted ? "true" : "false");
	printf("\nTime: %g\n",endTime-startTime);
	
	exit(0);

}

