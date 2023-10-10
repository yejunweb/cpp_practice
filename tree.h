#ifndef _TREE_H_
#define _TREE_H_
 
#include <iostream>
using namespace std;
 
class node
{
public:
	int data;
	node *parent;
	node *lift;
	node *right;
	node() : data(-1) , parent(NULL) , lift(NULL) , right(NULL) {}
	node(int num) : data(num) , parent(NULL) , lift(NULL) , right(NULL) {}
};
 
class trees
{
public:
	trees(int num[] , int len);		// 构造函数，用来插入n个数据
	void insertnode(int data);		// 递归插入
	node *searchnode(int data);		// 递归查找
	void deletenode(int data);		// 递归删除
	void inorder();					// 递归中序遍历
private:
	void insertnode(node* cur , int data);	// 递归插入
	node *searchnode(node* cur , int data); // 递归查找
	void deletenode(node* cur);				// 递归删除
	void inorder(node* cur);				// 递归中序遍历
	node* root;								// 二叉排序树的根节点
};
 
#endif