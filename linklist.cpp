#include <iostream>
#include<stdlib.h>
#include<queue>
#include<deque>

using namespace std;

/*  Project 1

    定义一个单向链表node, 每个节点值为int.
    定义函数void create_linklist(…, int n)来生成链表，链表长度由参数n传入,生成的链表要作为参数返回，而不是当初函数返回值返回，链表节点值为1,2,3...n。
    定义函数reverse_linklist(...),　对参数传入的链表进行逆转。
    定义函数insert_node(node *pHead, int Index, int Value ), 在指定链表位置Index插入指定Value
    定义函数delete_node(node *pHead,  int Index),删除链表位置Index所在的节点
    定义函数delete_linklist(),　删除整个链表


    main()函数：
    １．调用create_linklist()生成链表
    ２．调用insert_node()插入节点
    ３．调用delete_node()删除节点
    ４．调用reverse_linklist()逆转
    ５．打印链表所有值
    ６．结束程序工作
*/

struct node
{
    int v;
    node* n;
};

/* 链表长度由参数n传入,生成的链表要作为参数返回，而不是当初函数返回值返回，
   链表节点值为1,2,3...n
*/
void create_linklist(node **pHead, int n)
{
	node* p = nullptr;
	for (int i = 1; i <= n; i++){
		node* tempNode = new node();
		tempNode->v = i;
		if (i == 1){
			*pHead = tempNode;
			p = *pHead;
		}
		else{
			p->n = tempNode;
			p = tempNode;
		}
	}
}


//对参数传入的链表进行逆转
void reverse_linklist(node **pHead)
{
	if (!pHead || !*pHead || !(*pHead)->n)
		return;
	node* p = (*pHead)->n;
	(*pHead)->n = nullptr;
	while (p){
		auto tempNode = p->n;
		//p->n = tempNode->n;
		p->n = *pHead;
		*pHead = p;
		p = tempNode;
	}
}

//在指定链表位置Index插入指定Value
// Index 为 1 ~ n 代表 1 ~ n 个节点
// Index = 0 即为头添加
void insert_node(node **pHead,  int Index, int Value)
{	
	if (!pHead || !*pHead)
		return;
	node* p = *pHead;
	node* preNode = nullptr;
	while (Index && p){
		preNode = p;
		p = p->n;
		Index--;
	}
	node* tempNode = new node();
	tempNode->v = Value;
	if (!preNode){
		tempNode->n = *pHead;
		*pHead = tempNode;
	}
	tempNode->n = preNode->n;
	preNode->n = tempNode;
}

//删除链表位置Index所在的节点
void delete_node(node **pHead,  int Index)
{	
	if (!pHead || !*pHead)
		return;
	node* p = *pHead;
	if (Index == 0)
		*pHead = p->n;
	else{
		node* preNode = p;
		while (Index && p){
			preNode = p;
			p = p->n;
			Index--;
		}

		if (preNode->n){
			node* q = preNode->n;
			preNode->n = q->n;
			delete q;
		}
	}
}

//删除整个链表
void delete_linklist(node **pHead)
{
	node* p = *pHead;
	while (p){
		node* q = p;
		p = p->n;
		delete q;
	}
}


/* Project 2
  编写一个函数 int kth_leaf_on_deepest_level(BTree *pRoot, int k );
  给定一个未知深度的二叉树，求树中最深的一层上第k个叶子节点的value，例如下面的二叉树：

      kth_leaf_on_deepest_level(pRoot, 1) == 41
      kth_leaf_on_deepest_level(pRoot, 3) == 43

                        10
                     /     \
                    /       \
                 21          22
                /  \          \
              31    32         33
                   /           / \
                  41          42 43
  要求：
      1. 不能使用递归函数
      2. 不能开辟新的数据空间存储每层所有叶子节点的value, 可以申请新的数据空间为每层最多存一个叶子节点的value
*/

struct BSTreeNode
{
    BSTreeNode * left;
    BSTreeNode * right;
    int     value;
};

BSTreeNode * creatBTree(int data[], int index, int n)
{
    BSTreeNode * pNode = NULL;

    if(index < n && data[index] != -1)
    {
        pNode = (BSTreeNode *)malloc(sizeof(BSTreeNode));
        if(pNode == NULL)
            return NULL;
        pNode->value = data[index];
        pNode->left = creatBTree(data, 2 * index + 1, n);
        pNode->right = creatBTree(data, 2 * index + 2, n);
    }

    return pNode;
}

int kth_leaf_on_deepest_level(BSTreeNode *pRoot, int k )
{
	if (!pRoot)
		return -1;
	std::queue<BSTreeNode> nodeQueue;
	nodeQueue.push(*pRoot);
	int currentCengNum = 1;
	int nextCengNum = 0;
	bool hasNew = true;
	while (hasNew){
		hasNew = false;
		while (currentCengNum){
			auto p = nodeQueue.front();
			if (p.left){
				if (p.left->left || p.left->right){
					nodeQueue.push(*p.left);
					hasNew = true;
					nextCengNum++;
				}
			}
			if (p.right){
				if (p.right->left || p.right->right){
					nodeQueue.push(*p.right);
					hasNew = true;
					nextCengNum++;
				}
			}
			if (hasNew){
				nodeQueue.pop();
			}

			currentCengNum--;
		}
		currentCengNum = nextCengNum;
		nextCengNum = 0;
	}

	int idx = 0;
	while (!nodeQueue.empty()){
		auto p = nodeQueue.front();
		nodeQueue.pop();
		if (p.left){
			idx++;
			if (idx == k)
				return p.left->value;
		}
		if (p.right){
			idx++;
			if (idx == k)
				return p.right->value;
		}

	}

	return -1;
}

/*  Project 3

    定义一个聚类函数 cluster(int data[], int len, int radius)；
    data中的值没有重复，长度为len,
    把按照数值的聚类进行分为n组，
    对于组G中任意一个数值a,总是能在本组G中找到一个数值b,　使　|a-b| < radius　　.
    在函数内部打印出所有n个组成员，分成n行输出

    (要求：不能使用数组排序操作)

    例如:
       输入 data[] = { 1, 20, 89, 22, 72, 2,39, 3,56,86, 5, 93,13, 15, 18, 73, 79, 81, 25, 38, 43, 83,48, 52, 59,92,84,95,87 };
       正确的屏幕输出为组及成员为(每行为一组,行之间顺序随意,组内成员顺序随意)：
       1, 2, 3, 5,
       13, 15, 18, 20, 22, 25,
       39, 38, 43, 48, 52, 56, 59,
       73, 72,
       79, 89, 92, 84, 95,87,86, 93,81, 83,

*/
void cluster(int data[], int len, int radius)
{
	std::deque<std::deque<int>> res;
	for (int i = 0; i < len; i++){
		bool needNewClus = true;
		for (auto& clus : res){
			if (data[i] - clus.back() > 5 || data[i] - clus.front() < -5)
				continue;
			else{
				if (data[i] > clus.back()) {
					clus.push_back(data[i]);
					needNewClus = false;
					break;
				}
				else if (data[i] < clus.front()){
					clus.push_front(data[i]); 
					needNewClus = false;
					break;
				}
				else{
					int temp = clus.back();
					clus.pop_back();
					clus.push_back(data[i]);
					clus.push_back(temp);
					needNewClus = false;
					break;
				}
			}
		}


		if (needNewClus){
			std::deque<int> clus;
			clus.push_back(data[i]);
			res.push_back(clus);
		}
	}

	/*
	*/
	std::sort(res.begin(), res.end(), [](std::deque<int> clus1, std::deque<int> clus2){
		return clus1.front() < clus2.front();
	});
	for (auto i = 0; i < res.size();){
		int idx = i + 1;
		while (res[idx].empty()) idx++;
		if (abs(res[i].back() - res[idx].front()) < 5){
			res[i].insert(res[i].end(), res[idx].begin(), res[idx].end());
			res[idx].clear();
		}
		i = idx;
		while (i < res.size() && res[i].empty()) i++;
	}
	/*
	//复杂度高
	for (int i = 0; i < res.size();){
		if (res[i].empty()){
			i++;
			continue;
		}
		bool hasMerge = false;
		for (int j = i + 1; j < res.size(); j++){
			if (res[j].empty())
				continue;
			if (abs(res[i].back() - res[j].front()) < 5){
				res[i].insert(res[i].end(), res[j].begin(), res[j].end());
				res[j].clear();
				hasMerge = true;
				break;
			}
			if (abs(res[i].front() - res[j].back()) < 5){
				res[j].insert(res[j].end(), res[i].begin(), res[i].end());
				res[i].clear();
				hasMerge = true;
				break;
			}
		}
		i = hasMerge ? 0 : i + 1;
	}
	*/

	for (auto clus : res){
		if (clus.empty())
			continue;
		for (auto n : clus){
			cout << n << "	";
		}
		cout << endl;
	}
}


int main()
{
    // Project 1
    // 可将头尾指针及节点个数封装结构体，更方便操作
    cout << "==========Project1=================" << endl;
    node *pHead = nullptr;
    // １．调用create_linklist()生成链表
    create_linklist(&pHead,9);
    // ２．调用insert_node()插入节点
    insert_node(&pHead,9,10);
    // ３．调用delete_node()删除节点
    delete_node(&pHead,9);
    // ４．调用reverse_linklist()逆转
    reverse_linklist(&pHead);

    node *pTemp = pHead;
    while (pTemp)
    {
        std::cout << pTemp->v << " ";
        pTemp = pTemp->n;
    }
    cout << endl;
    cout << "==========Project1=================" << endl;
    cout << endl;

    // Project 2
    cout << "==========Project2=================" << endl;
    #define CHECK_VALUE(v,expected) \
    if(v==expected) cout<<"GOOD"<<endl; \
    else cerr<<"Wrong output:"<< v << ",expected:" <<expected<<endl;

    BSTreeNode * pRoot = NULL;
    {

        int data[] = { 10, 21, 22, 31, 32, -1, 33, -1, -1, 41, -1, -1, -1, 42, 43};
/*
                    10
                 /     \
                /       \
             21          22
            /  \          \
           31    32         33
                /          / \
               41         42 43
*/
        pRoot = creatBTree(data, 0 , sizeof(data) / sizeof(int));
        int v = kth_leaf_on_deepest_level(pRoot, 1);
        CHECK_VALUE(v, 41);
        v = kth_leaf_on_deepest_level(pRoot, 3);
        CHECK_VALUE(v, 43);
    }
    {
        int data[] = { 8,
                       2, 9,
                       21, 42, -1, 33,
                       -1, -1, 36, -1, -1, -1, 18, 47,
                       -1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1};
/*
                       8
                    /     \
                   /       \
                 2          9
               /  \          \
             21    42         33
                  /           / \
                 36          18  47
                /              \
               2               11
*/
        pRoot = creatBTree(data, 0 , sizeof(data) / sizeof(int));
        int v = kth_leaf_on_deepest_level(pRoot, 1);
        CHECK_VALUE(v, 2);
        v = kth_leaf_on_deepest_level(pRoot, 2);
        CHECK_VALUE(v, 11);
    }
    cout << "==========Project2=================" << endl;
    cout << endl;

    // Project 3
    cout << "==========Project3=================" << endl;
    int data[] = { 1, 20, 89, 22, 72, 2,39, 3,56,86, 5, 93,13, 15, 18, 73, 79, 81, 25, 38, 43, 83,48, 52, 59,92,84,95,87 };
    cluster(data, sizeof(data)/sizeof(int), 5);
    /*
    正确的输出为组及成员为(组内成员顺序随意)：
    1, 2, 3, 5,
    13, 15, 18, 20, 22, 25,
    39, 38, 43, 48, 52, 56, 59,
    73, 72,
    79, 89, 92, 84, 95,87,86, 93,81, 83,
    */
    cout << "==========Project3=================" << endl;
    return 0;
}

