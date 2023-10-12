#include <iostream>
#include <queue>
using namespace std;

/**
 * https://blog.csdn.net/standingflower/article/details/128126394
 * https://blog.csdn.net/qq_54162207/article/details/117871598
 */

/* 二叉排序树的二叉链表的存储表示 */
#define N 10

typedef int KeyType;
typedef int InfoType;

typedef struct
{
	KeyType key;		// 关键子项
	InfoType otherinfo; // 其他数据域
} ElemType;

typedef struct BSTNode
{
	ElemType data;					 // 每个结点的数据域包括关键字项和其他数据项
	struct BSTNode *lchild, *rchild; // 左右孩子指针
} BSTNode, *BSTree;

/* 二叉排序树的插入 */
void InsertBST(BSTree &T, ElemType e)
{
	if (!T)
	{
		BSTNode *S = new BSTNode;	  // 生成新结点*S
		S->data = e;				  // 新结点*S的数据域置为e
		S->lchild = S->rchild = NULL; // 新结点*S作为叶子结点
		T = S;						  // 把新结点*S链接到已找到的插入位置
	}
	else if (e.key < T->data.key)
		InsertBST(T->lchild, e); // 将*S插入左子树
	else if (e.key > T->data.key)
		InsertBST(T->rchild, e); // 将*S插入右子树
}

/* 二叉排序树的创建 */
void CreatBST(BSTree &T)
{
	// 依次读入一个关键字为key的结点，将此结点插入二叉排序树T中
	int num;
	int index = 0;
	T = NULL; // 将二叉排序树T初始化为空树
	ElemType e;
	cout << "请依次输入" << N << "个1-100的整数，组成数组以生成排序树\n";
	while (index != N)
	{
		cin >> num;
		if (num > 100 || num < 1)
		{
			cout << "数字" << num << "不在要求值域中，请重新输入符合规范的值!" << endl;
			if (index != 0)
			{
				index--; // 不符合值域回退索引
			}
		}
		else
		{
			e.key = num;
			InsertBST(T, e);
			index++;
		}
	}
}

/* 二叉排序树的删除 */
void DeleteBST(BSTree &T, KeyType key)
{
	// 从二叉排序树T中删除关键字等于key的结点
	BSTNode *p = new BSTNode;
	BSTNode *f = new BSTNode;
	p = T;
	f = NULL; // 初始化
	// 下面的while循环从根开始查找关键字等于key的结点*p
	while (p)
	{
		if (p->data.key == key)
			break; // 找到关键字等于key的结点*p，结束循环
		f = p;	   // *f为*p的双亲结点
		if (p->data.key > key)
			p = p->lchild; // 在*p的左子树中继续查找
		else
			p = p->rchild; // 在*p的右子树中继续查找
	}
	if (!p)
		return; // 找不到被删结点则返回
	// 考虑3种情况实现p所指子树内部的处理：*p左右子树均不空、无右子树、无左子树
	BSTNode *q = new BSTNode;
	q = p;
	if ((p->lchild) && (p->rchild))
	{
		// 被删结点*p左右子树均不空
		BSTNode *s = new BSTNode;
		s = p->lchild;
		while (s->rchild)
		{
			// 在*p的左子树中继续查找其前驱结点，即右下结点
			q = s;
			s = s->rchild; // 向右到尽头
		}
		p->data = s->data; // s指向被删结点的“前驱”
		if (q != p)
			q->rchild = s->lchild; // 重接*q的右子树
		else
			q->lchild = s->lchild; // 重接*q的左子树
		delete s;
		return;
	}
	else if (!p->rchild) // 被删结点*p无右子树，只需重接其左子树
		p = p->lchild;
	else if (!p->lchild) // 被删结点*p无左子树，只需重接其右子树
		p = p->rchild;
	/* 将p所指的子树挂接到其双亲结点*f相应的位置 */
	if (!f)
		T = p; // 被删结点为根结点
	else if (q == f->lchild)
		f->lchild = p; // 挂接到*f的左子树位置
	else
		f->rchild = p; // 挂接到*f的右子树位置
	delete q;
}

/* 二叉排序树的递归查找 */
BSTree SearchBST(BSTree T, KeyType key)
{
	// 在根指针T所指二叉排序树中递归地查找某关键字等于key的数据元素
	// 若查找成功，则返回指向该数据元素结点的指针，否则返回空指针
	if ((!T) || key == T->data.key)
		return T; // 查找结束
	else if (key < T->data.key)
		return SearchBST(T->lchild, key); // 在左子树中继续查找
	else
		return SearchBST(T->rchild, key); // 在右子树中继续查找
}

/* 二叉树的中序遍历 */
void InorderTree(BSTree T)
{
	if (T)
	{
		InorderTree(T->lchild);
		cout << T->data.key << " ";
		InorderTree(T->rchild);
	}
}

/* 二叉树的层次遍历 */
void LevelOrderTree(BSTree T)
{
	if (T)
	{
		queue<BSTree> q;
		q.push(T);
		while (!q.empty())
		{
			// 当队列非空时，还有没有遍历的parent结点
			BSTree temp = q.front();
			q.pop();
			cout << temp->data.key << " ";
			if (temp->lchild != NULL)
				q.push(temp->lchild);
			if (temp->rchild != NULL)
				q.push(temp->rchild);
		}
	}
}

int main()
{
	BSTree T = NULL;
	// 创建一棵二叉排序树
	CreatBST(T);
	// 中序遍历二叉树
	cout << "中序遍历输出结果为：";
	InorderTree(T);
	cout << "\n";
	// 层次遍历二叉树
	cout << "层次遍历输出结果为：";
	LevelOrderTree(T);
	cout << "\n";

	// cout << endl;
	// cout << "请输入您要查询的元素：" << endl;
	// int k = 0;
	// cin >> k;
	// BSTree T1 = NULL; // 定义T1用来接收查找结果
	// T1 = SearchBST(T, k);
	// if (T1)
	// 	cout << "存在此元素！" << endl;
	// else
	// 	cout << "不存在此元素！" << endl;
	// int key = 0;
	// cout << "请输入您要删除的元素：" << endl;
	// cin >> key;
	// DeleteBST(T, key); // 删除元素
	// InorderTree(T);	   // 再次中序遍历二叉树
	return 0;
}
