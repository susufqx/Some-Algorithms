#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode {
  char data;
  struct BiTNode *Lchild, *Rchild;
} BiTree;

BiTree *CreateBiTree(BiTree *T);
void PreOrderT(BiTree *T);
void InOrderT(BiTree *T);
void PostOrderT(BiTree *T);
void DestroyBiTree(BiTree *T);

// 递归创建二叉树
BiTree *CreateBiTree(BiTree *T) {
  char ch;
  scanf("%c", &ch);
  if (ch == '#')
    return 0;

  T = (BiTree *)malloc(sizeof(BiTree));
  T->data = ch;
  T->Lchild = CreateBiTree(T->Lchild);
  T->Rchild = CreateBiTree(T->Rchild);

  return T;
}

// 前序遍历
void PreOrderT(BiTree *T) {
  if (T) {
    printf("%c", T->data);
    PreOrderT(T->Lchild);
    PreOrderT(T->Rchild);
  }
}

// 中序遍历
void InOrderT(BiTree *T) {
  if (T) {
    InOrderT(T->Lchild);
    printf("%c", T->data);
    InOrderT(T->Rchild);
  }
}

// 后序遍历
void PostOrderT(BiTree *T) {
  if (T) {
    PostOrderT(T->Lchild);
    PostOrderT(T->Rchild);
    printf("%c", T->data);
  }
}

// 销毁二叉树
void DestroyBiTree(BiTree *T) {
  if (T) {
    DestroyBiTree(T->Lchild);
    DestroyBiTree(T->Rchild);
    free(T);
  }
}

int main() {
  BiTree *T;
  printf("请输入二叉树的数据，并以#为空节点\n");
  T = CreateBiTree(T);
  printf("该树的先序遍历结果为：");
  PreOrderT(T);
  printf("\n");
  printf("该树的中序遍历结果为：");
  InOrderT(T);
  printf("\n");
  printf("该树的后序遍历结果为：");
  PostOrderT(T);
  printf("\n");
  DestroyBiTree(T);
  return 0;
}