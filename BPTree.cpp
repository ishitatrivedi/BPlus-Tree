#include "BPTree.hpp"
#include <stdio.h>

#ifdef WIN32
	#include <io.h>
#endif

#ifdef LINUX
	#include <unistd.h>
#endif

//  Define constructor for BPTree
BPTree::BPTree()
{
    root=NULL;
    order=0;
}
//  Define destructor for BPTree
BPTree::~BPTree()
{
    delete(root);
}
//  Define function Initialize()
void BPTree::Initialize(int m) 
{
    order=m;
}
//  Define function search_Recursive to implement recursive search for (node, key)
BPTnode *BPTree::search_Recursive(BPTnode *node, int key)
{ 
    if (node->IsLeaf()) 
        return node;	//  If the pointer of node is leafnode, return it
    else 
    {
        //  If the pointer of node is not leafnode, get the key
        int index=node->GetKeyIndex(key);
        if (index<node->GetKeyNum()&&key==node->GetKeys()[index]) 
            return search_Recursive(node->getChildren()[index + 1],key);	// If key is found, move pointer to right and increase index
        else
            return search_Recursive(node->getChildren()[index],key); // If key is not found, move pointer to left
    }
}
//  Define function insert()
void BPTree::insert(int key, float value) 
{
    if (!root)
        root=new LNode(order);
    int midKey=0;
    bool needNewRoot=false;		// Define boolean for loops
    BPTnode *rtNode=nullptr;
    BPTnode *node=search_Recursive(root, key);
    node->insert(key, value);
    while(node->GetKeyNum()==order) //  Get parent and split tree
    {  
        BPTnode *parent=node->GetParent();
        rtNode=node->split(midKey);
        if (parent) 
        { 
            parent->insert(midKey, rtNode);	// If parent exists, insert
            rtNode->SetParent(parent);
            node=parent; 
        } 
        else 
        { 		// Else break and deal with it outside
            needNewRoot=true;
            break; 
        }
    }
    // Generate a new root and replace the old one
    if (needNewRoot) 
    {
        InNode *newRoot = new InNode (order);
        newRoot->insert (midKey, node, rtNode);
        node->SetParent (newRoot);
        rtNode->SetParent (newRoot);
        
        root = newRoot;
    }
}
//  Define function Delete()
void BPTree::Delete(int key)
{
    BPTnode *node=search_Recursive(root, key);
    node->Delete (key);
    if (root->GetKeyNum()==0) 
    {
        root=root->getChildren()[0];
        delete root->GetParent();
        root->SetParent(NULL);  
    }   
}
//  Define function search()
bool BPTree::search(int key, float &value)
{
    if (root->GetKeyNum()==0) // The tree is empty
        return false;
    LNode *node=(LNode *)search_Recursive(root, key); //searches for the key recursively
    int index=node->GetKeyIndex(key);
    if (key==node->GetKeys()[index]) 
    {
        value = node->getValues()[index];   //if value present, return value and true
        return true;
    } 
    else 
        return false;  
}
//  Define function search()
bool *BPTree::search (int key1, int key2, float *&values, int &num) 
{
    num=0;
    int buffSize=1;
    bool *status=new bool[2];
    
    status[0]=false;
    status[1]=false;
    
    BPTnode *node1=search_Recursive (root, key1); //searches for key 1
    BPTnode *node2=search_Recursive (root, key2);  //searches for key 2
    // cout << node1 << " " << node2 << endl;
    int index1=node1->GetKeyIndex(key1);
    int index2=node2->GetKeyIndex(key2);
    
    if (index1<node1->GetKeyNum()&& key1==node1->GetKeys()[index1])
        status[0]=true;   
    
    if (index2<node2->GetKeyNum() && key2==node2->GetKeys()[index2])
        status[1]=true;
    
    if (key1<key2) 
    {
        int index=index1;
        values=new float[buffSize];
        if ((!index1)<node1->GetKeyNum()) 
        {
            index=0;
            node1=node1->GetNext ();    //get the next values 
        }
        while (node1!=node2->GetNext()) 
        {
            for (; index<node1->GetKeyNum();index++) 
            {
                if (node1->GetKeys()[index]<=key2) 
                {
                    if (num==buffSize) 
                    {
                        buffSize *=2;
                        float *newValues= new float[buffSize];
                        for (int i=0; i<buffSize/2; i++)
                            newValues[i]=values[i];
                        delete[] values;
                        values=newValues;
                    }
                    values[num++]=node1->getValues()[index];
                    // cout << values[num - 1] << endl;
                } 
                else
                    break; 
            }
            node1=node1->GetNext();
            index=0;  
        }  
    } 
	else 
	{
        values=new float[2];

        if (status[0])
            values[0] = node1->getValues ()[index1]; 
        
        if (status[1])
            values[1] = node2->getValues ()[index2];
        num = 2;   
    }
    return status;
}
