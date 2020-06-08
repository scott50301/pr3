#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <stdbool.h>

void merge(int* arr, int l, int m, int r)
{
    int* tmp_arr = NULL;
    int i = 0;
    int l1 = l;
    int r1 = m;
    int l2 = m + 1;
    int r2 = r;

    tmp_arr = (int*)malloc(sizeof(int) * (r - l + 1));

    while ((l1 <= r1) && (l2 <= r2))
    {
        if (arr[l1] < arr[l2])
            tmp_arr[i++] = arr[l1++];
        else
            tmp_arr[i++] = arr[l2++];
    }

    while (l1 <= r1) tmp_arr[i++] = arr[l1++];
    while (l2 <= r2) tmp_arr[i++] = arr[l2++];

    for (i = l; i <= r; i++) arr[i] = tmp_arr[i - l];

    free(tmp_arr);
}

void mergesort(int* arr, int l, int r)
{
    int m = 0;

    if (l < r)
    {
        m = (l + r) / 2;
        mergesort(arr, l, m);
        mergesort(arr, m + 1, r);

        merge(arr, l, m, r);
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

//static void
//print_time(double const seconds)
//{
//  printf("Operation Time: %0.04fs\n", seconds);
//}

int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    //int n = 10000000;
    int * arr;
    char *operation = "";
	char *verify = "-v";
    int i ;
    //clock_t ts, te;
    double startTime, endTime;

	if (argc == 3){
		operation = argv[2];
	}
    arr = (int*)malloc(sizeof(int) * n);
    srand(time(0));
    for (i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }

    printf("\n");
    //ts = clock();
    startTime = omp_get_wtime();
    mergesort(arr, 0, n - 1);
    endTime = omp_get_wtime();
   //te = clock();
   
    if(!strcasecmp(verify, operation)){
    	bool res = check_sorted(arr, n);
    	//for (i = 0; i < n; i++) printf(" %d", arr[i]);
   		printf("The array is sorted:%s\n", res ? "true" : "false");
   	}
   	printf("\nTime: %g\n",endTime-startTime);
    //print_time((double)(te-ts)/CLOCKS_PER_SEC);
}
