#include <stdio.h>
#include <stdlib.h>

// 归并排序的C语言实现
void fusion(int *a, int low, int high, int mid) {
  int left = low, right = mid + 1;
  int *temp = (int *)malloc(sizeof(int) * (high - low + 1));
  int i = 0;

  while (left <= mid && right <= high) {
    if (a[left] > a[right]) {
      temp[i++] = a[right++];
    } else {
      temp[i++] = a[left++];
    }
  }

  while (left <= mid) {
    temp[i++] = a[left++];
  }

  while (right <= high) {
    temp[i++] = a[right++];
  }

  for (i = 0; i < high - low + 1; i++) {
    a[low + i] = temp[i];
  }
}

void fusionsort(int *a, int l, int r) {
  int mid = (l + r) / 2;
  if (l != r) {
    fusionsort(a, l, mid);
    fusionsort(a, mid + 1, r);
    fusion(a, l, r, mid);
  }
}

// test code
int main(int argc, char **argv) {
  int i;
  int a[12] = {45, 27, 36, 12, 3, 7, 99, 65, 271, 111, 35, 18};

  fusionsort(a, 0, 11);
  for (i = 0; i < 12; i++) {
    printf("%d  ", a[i]);
  }
  printf("\n");

  return 0;
}