#include "tree.h"

trees::trees(int num[], int len)
{
    root = new node(num[0]);

    for (int i = 1; i < len; i++)
    {
        insertnode(num[i]);
    }
}

// 递归插入
void trees::insertnode(int data)
{
    if (root != NULL)
    {
        insertnode(root, data); // 调用递归插入的方法
    }
}
void trees::insertnode(node *cur, int data)
{
    // 首先要查看插入的数据和当前结点的data的大小
    if (data < cur->data)
    {
        if (cur->lift == NULL)
        {
            cur->lift = new node(data);
            cur->lift->parent = cur;
        }
        else
        {
            insertnode(cur->lift, data); // 这里就进入递归了，因为一个结点的左子树单独来看也是一个二叉树
        }
    }
    else if (data > cur->data)
    {
        if (cur->right == NULL)
        {
            cur->right = new node(data);
            cur->right->parent = cur;
        }
        else
        {
            insertnode(cur->right, data);
        }
    }
    return;
}

// 递归查找
node *trees::searchnode(int data)
{
    node *p = new node(data);
    if (root != NULL)
    {
        p = searchnode(root, data);
        return p;
    }
    return NULL;
}
node *trees::searchnode(node *cur, int data)
{
    if (data < cur->data)
    {
        if (cur->lift == NULL)
        {
            return NULL;
        }
        return searchnode(cur->lift, data);
    }
    else if (data > cur->data)
    {
        if (cur->right == NULL)
        {
            return NULL;
        }
        return searchnode(cur->right, data);
    }
    return cur;
}

// 递归删除
void trees::deletenode(int data)
{
    node *cur = new node(data);
    cur = searchnode(data);
    if (cur != NULL)
    {
        deletenode(cur);
    }
}

void trees::deletenode(node *cur)
{
    if (cur->lift != NULL)
    {
        deletenode(cur->lift);
    }
    if (cur->right != NULL)
    {
        deletenode(cur->right);
    }
    if (cur->parent == NULL)
    {
        delete cur;
        root = NULL;
        return;
    }
    if (cur->parent->data > cur->data)
    {
        cur->parent->lift = NULL;
    }
    else
    {
        cur->parent->right = NULL;
    }
    delete cur;
}

// 中序遍历
void trees::inorder()
{
    if (root == NULL)
    {
        return;
    }
    inorder(root);
}
void trees::inorder(node *cur)
{
    if (cur != NULL)
    {
        inorder(cur->lift);
        cout << cur->data << " ";
        inorder(cur->right);
    }
}