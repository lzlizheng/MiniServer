#include "tree.h"

//--------------------------------------------------------
//功能：
//	中序遍历树
//--------------------------------------------------------
void InOrderTraverse(pTree root)
{
	if(root)
	{
		InOrderTraverse(root->lchild);
		cout<<root->funcBlock->sFuncName<<endl;
		InOrderTraverse(root->rchild);
	}
}


//--------------------------------------------------------
//功能：
//	先序遍历树
//--------------------------------------------------------
void PreOrderTraverse(pTree root)
{
	if(root)
	{
		cout<<root->funcBlock->sFuncName<<endl;
		PreOrderTraverse(root->lchild);
		PreOrderTraverse(root->rchild);
	}
}


//--------------------------------------------------------
//功能：
//	后序遍历树
//--------------------------------------------------------
void PostOrderTraverse(pTree root)
{
	if(root)
	{
		PostOrderTraverse(root->lchild);
		PostOrderTraverse(root->rchild);
		cout<<root->funcBlock->sFuncName<<endl;
	}
}


//--------------------------------------------------------
//功能：
//	在根指针root所指向的二叉排序树中递归查找其关键字等于
//	data的数据元素
//--------------------------------------------------------
bool SearchBST(pTree root,char * data,pTree f,pTree& p)
{
	if(!root)
	{
		p = f;
		return false;
	}
	else if(strcmp(data, root->funcBlock->sFuncName) == 0)
	{
		p = root;
		return true;
	}
	else if(strcmp(data, root->funcBlock->sFuncName) < 0)
		return SearchBST(root->lchild,data,root,p);
	else if(strcmp(data, root->funcBlock->sFuncName) > 0)
		return SearchBST(root->rchild,data,root,p);
	else
	{
		cout<<"search error"<<endl;
		return false;
	}
}


//---------------------------------------------------------
//功能：
//	将数据data插入树root中
//--------------------------------------------------------
void InsertBST(pTree& root,sFuncBlock * pFuncBlock)
{
	pTree p;	//存放查找结果
	pTree s;	//指向新节点
	
	if(!SearchBST(root,pFuncBlock->sFuncName,NULL,p))
	{
		s = new node;
		s->funcBlock = pFuncBlock;
		s->lchild = NULL;
		s->rchild = NULL;
		
		if(p == NULL)//树为空的时候
		{
			root = s;
		}
		else if(strcmp(pFuncBlock->sFuncName, p->funcBlock->sFuncName) < 0)
		{
			p->lchild = s;
		}
		else
		{
			p->rchild = s;
		}
	}
}

//--------------------------------------------------------
//功能：
//	释放树所使用的内存空间
//--------------------------------------------------------
void DeleteBST(pTree root)
{
	if(root)
	{
		DeleteBST(root->lchild);
		DeleteBST(root->rchild);
		delete root;
	}
}
