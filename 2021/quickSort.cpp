#include "bits/stdc++.h"

using namespace std;

// 将数组分割为两部分，并返回基准点的索引
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // 选择最后一个元素作为基准点
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);  // 将较小的元素交换到左侧
        }
    }

    swap(arr[i + 1], arr[high]);  // 将基准点放置到正确的位置
    return i + 1;
}

// 快速排序的递归函数
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);  // 获取基准点的索引
        quicksort(arr, low, pivotIndex - 1);  // 对基准点左侧的子数组进行快速排序
        quicksort(arr, pivotIndex + 1, high);  // 对基准点右侧的子数组进行快速排序
    }
}


/****************************************/
void QuickSort(vector<int>& v, int low, int high) {
    if (low >= high)		// 结束标志
        return;
    int first = low;		// 低位下标
    int last = high;		// 高位下标
    int key = v[first];		// 设第一个为基准

    while (first < last)
    {
        // 将比第一个小的移到前面
        while (first < last && v[last] >= key)
            last--;
        if (first < last)
            v[first++] = v[last];

        // 将比第一个大的移到后面
        while (first < last && v[first] <= key)
            first++;
        if (first < last)
            v[last--] = v[first];
    }
    // 基准置位
    assert(first==last);
    v[first] = key;
    // 前半递归
    QuickSort(v, low, first - 1);
    // 后半递归
    QuickSort(v, first + 1, high);
}

void QuickSort2(vector<int>& arr, int start,int end){
    if(start>=end)return;
    int low=start;
    int high=end;
    int key=arr[end];
    while(low<high){
        while(low<high && arr[low]<=key)low++;
        if(low<high)arr[high--]=arr[low];
        while(low<high&&arr[high]>=key)high--;
        if(low<high)arr[low++]=arr[high];
    }
    assert(low==high);
    arr[low]=key;
    QuickSort2(arr,start,low-1);
    QuickSort2(arr,low+1,end);
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
            {2,3,1},
            {4,3,2,1},
            {5,4,3,2,1},
            {2,3,4,1},
            {2,3,4,5,1},
            {2,3,5,4,1},
    };
    for(auto arr:arrs){
//        quickSort(arr, 0, arr.size()-1);
        QuickSort2(arr, 0, arr.size()-1);
//        quickSort(arr);
        for (auto n:arr)cout << n << " ";
        cout << endl;
    }

    return 0;
}