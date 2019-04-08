#include <stdio.h>

// a 是一个数组，其中只有一个数出现奇数次，其余都是偶数次，
// 通过O(n)复杂度来实现一个算法
int findoddcounts(int *a, int num) {
  int temp = a[0];
  int i;

  for (i = 1; i < num; i++) {
    temp ^= a[i];
  }

  return temp;
}

// test code
int main(int argc, char **argv) {
  int a[15] = {2, 2, 4, 3, 5, 6, 4, 4, 4, 3, 3, 5, 6, 2, 2};
  printf("Res is : %d\n", findoddcounts(a, 15));
  return 0;
}