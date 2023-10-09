#include "bits/stdc++.h"

using namespace std;

int partition(vector<int>& arr, int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    vector<vector<int>> arrs={
            {1},
            {2,2},
            {2,1},
            {1,2},
            {1,2,2,2},
            {2,2,2,1},
            {2,2,1,2},
            {3,4,5,6,3,56},
            {2,3,1}
    };
    for(auto arr:arrs){
        quickSort(arr, 0, arr.size()-1);
        for (auto n:arr)cout << n << " ";
        cout << endl;
    }

    return 0;
}