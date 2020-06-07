#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <omp.h>
#include <stdbool.h>

void merge(int* arr, int start, int mid, int end)
{
    int* tmp_arr = NULL;
    int i = 0;
    int l1 = start;
    int r1 = mid;
    int l2 = mid + 1;
    int r2 = end;

    tmp_arr = (int*)malloc(sizeof(int) * (end - start + 1));

    while ((l1 <= r1) && (l2 <= r2))
    {
        if (arr[l1] < arr[l2])
            tmp_arr[i++] = arr[l1++];
        else
            tmp_arr[i++] = arr[l2++];
    }

    while (l1 <= r1) tmp_arr[i++] = arr[l1++];
    while (l2 <= r2) tmp_arr[i++] = arr[l2++];

    for (i = start; i <= end; i++) arr[i] = tmp_arr[i - start];

    free(tmp_arr);
}

void mergesort(int* arr, int start, int end)
{
    int mid = 0;

    if (start < end)
    {
        mid = (start + end) / 2;
        mergesort(arr, start, mid);
        mergesort(arr, mid + 1, end);

        merge(arr, start, mid, end);
    }
}

bool check_sorted(int* arr, int N){
	int i ;
	for(i=1; i < N; ++i){
		if(arr[i] > arr[i-1]){
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
    clock_t ts, te;

    arr = (int*)malloc(sizeof(int) * n);
    srand(time(0));
    for (i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }
    //printf("Array before sorting: ");
    //for (i = 0; i < (sizeof(arr) / sizeof(int)); i++) printf(" %d", arr[i]);
    printf("\n");
    ts = clock();
    mergesort(arr, 0, n - 1);
    te = clock();
   
    bool res = check_sorted(arr, n);
    //for (i = 0; i < n; i++) printf(" %d", arr[i]);
    printf("%s\n", res ? "true" : "false");
    print_time((double)(te-ts)/CLOCKS_PER_SEC);
}
