#include <stdio.h>

// 快速排序的C语言实现
void quicksort(int *a, int l, int r) {
  if (l < r) {
    int temp = a[l];
    int low = l, high = r;

    while (l < r) {
      while (l < r && temp <= a[r])
        r--;
      if (l < r) {
        a[l] = a[r];
      }

      while (l < r && temp >= a[l])
        l++;
      if (l < r) {
        a[r] = a[l];
      }
    }

    a[l] = temp;
    quicksort(a, low, r - 1);
    quicksort(a, l + 1, high);
  } else {
    return;
  }
}

// test code
int main(int argc, char **argv) {
  int i;
  int a[12] = {45, 27, 36, 12, 3, 7, 99, 65, 271, 111, 35, 18};

  quicksort(a, 0, 11);
  for (i = 0; i < 12; i++) {
    printf("%d  ", a[i]);
  }
  printf("\n");

  return 0;
}