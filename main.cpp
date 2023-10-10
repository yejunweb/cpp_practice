#include "tree.h"
#include "tree.cpp"

#define N 10

int main()
{
	// int list[N] = {5, 3, 1, 4, 8, 9, 7, 6, 2, 11, 34, 99, 100};
	int list[N];
	cout << "请输入" << N << "个1-100的整数，组成数组以生成排序树\n";
	for (int i = 0; i < N; i++)
	{
		cin >> list[i];
	}
	cout << "所输入的数组为：";
	for (int i = 0; i < N; i++)
	{
		cout << list[i] << ' ';
		if (i + 1 == N)
		{
			cout << '\n';
		}
	}

	trees treeSet(list, N);
	treeSet.inorder();
	cout << endl;
	// treeSet.insertnode(101);
	// treeSet.inorder();
	// cout << endl;
	// treeSet.deletenode(101);
	// treeSet.inorder();
	return 0;
}