#include "tree.h"

//--------------------------------------------------------
//���ܣ�
//	���������
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
//���ܣ�
//	���������
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
//���ܣ�
//	���������
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
//���ܣ�
//	�ڸ�ָ��root��ָ��Ķ����������еݹ������ؼ��ֵ���
//	data������Ԫ��
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
//���ܣ�
//	������data������root��
//--------------------------------------------------------
void InsertBST(pTree& root,sFuncBlock * pFuncBlock)
{
	pTree p;	//��Ų��ҽ��
	pTree s;	//ָ���½ڵ�
	
	if(!SearchBST(root,pFuncBlock->sFuncName,NULL,p))
	{
		s = new node;
		s->funcBlock = pFuncBlock;
		s->lchild = NULL;
		s->rchild = NULL;
		
		if(p == NULL)//��Ϊ�յ�ʱ��
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
//���ܣ�
//	�ͷ�����ʹ�õ��ڴ�ռ�
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
