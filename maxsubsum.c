#include <stdio.h>

// 给定的数组有正有负也有零，求出最大子序列和
int maxsubsum(int* a, int num) {
  int maxSum = 0, tmpSum = 0;
  int i;

  for (i=0;i<num;i++) {
    tmpSum += a[i];
    if (maxSum < tmpSum) {
      maxSum = tmpSum;
    } else if (tmpSum < 0) {
      tmpSum = 0;
    }
  }

  return maxSum;
}

// test code
int main(int argc, char** argv) {
  int a[12] = {-4, 2, -6, 0, 1, 7, 13, -14, -3, 12, -22, 11};
  printf("Max sub sum is : %d\n", maxsubsum(a, 12));
  return 0;
}