#include <fstream>
using namespace std;


#include "omp.h"


void merge(int * const array, int start_1, int end_1, int start_2, int end_2)
{
    int * const temp = new int[end_2 - start_1 + 1];
    int i, j, k;
    i = start_1;
    j = start_2;
    k = 0;
    
    while(i <= end_1 && j <= end_2)
    {
        if (array[i] < array[j])
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }
    
    while(i <= end_1) {
        temp[k++] = array[i++];
    }
        
    while(j <= end_2) {
        temp[k++] = array[j++];
    }
        
    for(int i = start_1, j = 0; i <= end_2; i++, j++) {
        array[i] = temp[j];
    }

    delete temp;
}


void mergesort(int * const array, int start, int end)
{
    int mid;
        
    if(start < end)
    {
        mid = start + (end - start) / 2;
        
        #pragma omp parallel sections 
        {

            #pragma omp section
            {
                mergesort(array, start, mid);
            }

            #pragma omp section
            {
                mergesort(array, mid + 1, end);
            }
        }

        merge(array, start, mid, mid + 1, end);
    }
}


int main()
{
    ifstream ifs("input");
    ofstream ofs("output");

    int num_elements;
    ifs >> num_elements;

    int * const array = new int[num_elements];
    for(int i = 0; i < num_elements; i++)
        ifs >> array[i];

    mergesort(array, 0, num_elements - 1);
    
    ofs << "Sorted array:" << endl;
    for(int i = 0; i < num_elements; i++)
        ofs << array[i] << " ";
    ofs << endl;

    delete array;

    return 0;
}
