#include <vector>
#include <future>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

bool make_thread = true; 

void merge(std::vector<int>& arr, int const left, int const mid, int const right) {
    auto const subArrayOne = mid - left + 1; 
    auto const subArrayTwo = right - mid; 

    std::vector<int> leftArray(subArrayOne), rightArray(subArrayTwo);

    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = arr[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = arr[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            arr[indexOfMergedArray++] = leftArray[indexOfSubArrayOne++];
        }
        else {
            arr[indexOfMergedArray++] = rightArray[indexOfSubArrayTwo++];
        }
    }
    while (indexOfSubArrayOne < subArrayOne) {
        arr[indexOfMergedArray++] = leftArray[indexOfSubArrayOne++];
    }
    while (indexOfSubArrayTwo < subArrayTwo) {
        arr[indexOfMergedArray++] = rightArray[indexOfSubArrayTwo++];
    }
}

void mergeSort(std::vector<int>& arr, int const begin, int const end) {
    if (begin >= end)
        return;

    auto mid = begin + (end - begin) / 2;

    if (make_thread && (end - begin > 100))
    {
        auto f = async(launch::async, mergeSort, std:: ref(arr), begin, mid); 
        mergeSort(arr, mid + 1, end);
        f.wait();
    }
    else {
        mergeSort(arr, begin, mid); 
        mergeSort(arr, mid + 1, end); 
    }
    merge(arr, begin, mid, end);
}
int main()
{
    srand(0);
    long arr_size = 1000;
    std::vector<int> array(arr_size);

    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500;
    }
    cout << "Unsorted array: ";
    for (long i = 0; i < arr_size; i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    time_t start, end;
   
    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);

    double seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    cout << "Sorted array: ";
    for (long i = 0; i < arr_size; i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    for (long i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }

    return 0;
}