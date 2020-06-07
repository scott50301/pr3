#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>

void merge(int* arr, int left, int mid, int right)
{
    int* tmp_arr = NULL;
    int i = 0;
    int l1 = left;
    int r1 = mid;
    int l2 = mid + 1;
    int r2 = right;

    tmp_arr = (int*)malloc(sizeof(int) * (right - left + 1));

    while ((l1 <= r1) && (l2 <= r2))
    {
        if (arr[l1] < arr[l2])
            tmp_arr[i++] = arr[l1++];
        else
            tmp_arr[i++] = arr[l2++];
    }

    while (l1 <= r1) tmp_arr[i++] = arr[l1++];
    while (l2 <= r2) tmp_arr[i++] = arr[l2++];

    for (i = left; i <= right; i++) arr[i] = tmp_arr[i - left];

    free(tmp_arr);
}

void mergesort(int* arr, int left, int right)
{
    int mid = 0;

    if (left < right)
    {
        mid = (left + right) / 2;
        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

bool check_sorted(int* arr, int N){
	int i ;
	for(i=1; i < N; ++i){
		if(arr[i] < arr[i-1]){
			return false;
		}
	}
	return true;
}

static void
print_time(double const seconds)
{
  printf("Operation Time: %0.04fs\n", seconds);
}

int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    //int n = 10000000;
    int * arr;
    int i ;
    double startTime, endTime;

    arr = (int*)malloc(sizeof(int) * n);
    srand(time(0));
    for (i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }
    printf("\n");
    startTime = omp_get_wtime();
    mergesort(arr, 0, n - 1);
    endTime = omp_get_wtime();
   
    bool res = check_sorted(arr, n);
 
    printf("%s\n", res ? "true" : "false");
    printf("\nTime: %g\n",endTime-startTime);
}
