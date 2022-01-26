#include "BPTNode.hpp"
//  Define constructor of BPTnode
BPTnode::BPTnode (int n)
{
    	order=n;
        parent=NULL;        //  Initialize values of parent, previour and next node to be Null
        prev=NULL;
        next=NULL;
        keys=new int[n];        //  Initialize values of keys
        keyNum=0;
}
//  Destructor of BPTnode
BPTnode::~BPTnode()
{
	delete[] keys;
}
//Returns if the node is a leaf
bool BPTnode::IsLeaf()
{
	return isLeaf;
}
int BPTnode::GetKeyNum ()
{
    return keyNum;
}
int *BPTnode::GetKeys()
{
	return keys;
}
int BPTnode::GetKeyIndex(int key)
{
    int lft=0;
    int rht=keyNum-1;
    int mid;
    
    while (lft<rht) //return the mid while splitting
    {
        mid=(lft + rht)/2;
        if (keys[mid]==key)
            return mid; 
        else if (keys[mid]>key)
            rht=mid - 1;
        else
            lft=mid + 1;
    }  
    if (lft==rht) 
    {
        if (keys[lft]>=key)  
            return lft; 
        else 
            return lft+1;
    }
    return lft;
}
void BPTnode::decKeyNum ()
{
	keyNum--;      //decrease key num
}
BPTnode *BPTnode::GetParent ()
{
    return parent;  //return parent of a node
}
BPTnode *BPTnode::GetNext()
{
    return next;    //return next sibling pointer of node
}
BPTnode *BPTnode::GetPrev ()
{
    return prev;    //return previous pointer of node
}
void BPTnode::SetParent(BPTnode *node)
{
    parent=node;
}
void BPTnode::SetNext(BPTnode *node)
{
    next=node;
}
void BPTnode::SetPrev(BPTnode *node)
{
    prev=node;
}
LNode::LNode(int n):BPTnode(n) //constructor for class LNode
{
    isLeaf=true;
    values=new float[n];
}
LNode::~LNode () //destructor for class LNode
{
    delete[] values;
}
void LNode::insert (int key, float value)   //for inserting n a leaf node
{
    int index=GetKeyIndex (key);
    for (int i=keyNum; i>index; i--) 
    {
        keys[i]=keys[i - 1];
        values[i]=values[i - 1];
    }
    keys[index]=key;
    values[index]=value;
    keyNum++;
}
void LNode::Delete(int key)     //for deleting from a leaf node
{
    int index=GetKeyIndex(key);
    if (keys[index]==key) 
    {
        decKeyNum ();
        for (; index <keyNum; index++) 
        {  
            keys[index]=keys[index + 1];
            values[index]=values[index + 1];
        }
    } 
    else 
    {
        cout<<key<<" Not found!"<<endl;
        return;
    }
    if (keyNum < order/2) 
    {
        if (next && next->GetKeyNum()>order/2) 
        {
            next->decKeyNum();
            keys[keyNum]=next->GetKeys()[0];
            values[keyNum++] =next->getValues()[0];
            for (int i=0; i<next->GetKeyNum (); i++) 
            {
                next->GetKeys()[i]=next->GetKeys()[i + 1];
                next->getValues ()[i]=next->getValues ()[i + 1];
            }
            BPTnode *lftParent=parent;
            BPTnode *rightParent=next->GetParent();
            while (lftParent!=rightParent) 
            {
                lftParent=lftParent->GetParent();
                rightParent=rightParent->GetParent();
            }
            int pindex=lftParent->GetKeyIndex(keys[keyNum -1]);
            if (lftParent->GetKeys()[pindex]!=keys[keyNum - 1])
                pindex--;
            lftParent->GetKeys()[pindex]=next->GetKeys()[0];
        } 
        else if(prev&&prev->GetKeyNum()>order/2) 
        { 
            prev->decKeyNum();
            for (int i=keyNum; i>0; i--) 
            { 
                keys[i]=keys[i - 1];
                values[i]=values[i - 1];
            }
            keys[0]=prev->GetKeys ()[prev->GetKeyNum ()];
            values[0]=prev->getValues()[prev->GetKeyNum ()];
            keyNum++;
            BPTnode *lftParent=prev->GetParent ();
            BPTnode *rightParent=parent;
            while (lftParent!=rightParent) 
            {
                lftParent=lftParent->GetParent ();
                rightParent=rightParent->GetParent (); 
            }  
            int pindex=lftParent->GetKeyIndex(keys[0]);
            lftParent->GetKeys()[pindex]=keys[0];
        } 
        else if (next) 
            merge(next); 
        else if (prev)
            prev->merge (this);
    }
}
void LNode::merge(BPTnode* rtNode) //merge operation while deleting
{
    int *rightKeys=rtNode->GetKeys();
    float *rightValues=rtNode->getValues();
    int down=rightKeys[0];

    for (int i=0; i<rtNode->GetKeyNum(); i++) 
    {
        keys[keyNum+i]=rightKeys[i];
        values[keyNum+i]=rightValues[i];
    }
    keyNum+=rtNode->GetKeyNum();
    next=rtNode->GetNext();

    if (next)
        next->SetPrev(this);
    rtNode->GetParent()->Delete(down);
    delete rtNode;
}

BPTnode *LNode::split(int &key) //split operation while inserting
{
    // New rht node
    LNode *rtNode=new LNode(order);
    // The key to be moved into the parent
    key=keys[keyNum/2];
    // Copy every key and value after keyNum / 2 to rtNode
    for(int i=keyNum/2; i<keyNum; i++) 
    {
        rtNode->keys[i-keyNum/2]=keys[i];
        rtNode->values[i-keyNum/2]=values[i];
    }
    rtNode->keyNum=keyNum-keyNum/2;
    keyNum=keyNum/2;
    return rtNode;
}
float *LNode::getValues() 
{
    return values;
}
//  Define InNode
InNode::InNode(int n):BPTnode(n)    //constructor of InNode
{
    isLeaf=false;
    children=new BPTnode*[n+1];
}
InNode::~InNode()   //destructor of InNode
{
    delete[] children;
}
// insert a new key and node into the current internal node
void InNode::insert(int key, BPTnode* rtChild)
{
    int index=GetKeyIndex(key);
    // move every key and child after index
    for (int i=keyNum; i>index; i--) 
    {
        keys[i]=keys[i - 1];
        children[i+1]=children[i];
    }
    keys[index]=key;
    children[index+1]=rtChild;
    rtChild->SetNext(children[index]->GetNext ());
    rtChild->SetPrev(children[index]);
    children[index]->SetNext (rtChild);
    if (rtChild->GetNext ())
        rtChild->GetNext ()->SetPrev(rtChild); 
    keyNum++; 
}
// Used only when creating new internal Node
void InNode::insert(int key, BPTnode* lftChild, BPTnode* rtChild)
{
    keys[keyNum]=key;
    rtChild->SetPrev(lftChild);
    rtChild->SetNext(lftChild->GetNext());
    lftChild->SetNext(rtChild);
    if (rtChild->GetNext ())
        rtChild->GetNext ()->SetPrev (rtChild);
    children[keyNum++]=lftChild;
    children[keyNum]=rtChild;
}
void InNode::Delete(int key)
{
    int index=GetKeyIndex(key);
    decKeyNum();
    if (keys[index]>key) 
    {
        for (; index < keyNum; index++) 
        { 
            keys[index]=keys[index + 1];
            children[index]=children[index + 1];
        }
        children[index]=children[index + 1];
    } 
    else 
    {  
        for (; index < keyNum; index++) 
        {
            keys[index] = keys[index + 1];
            children[index + 1] = children[index + 2];
        }  
    }
    if (keyNum<order/2) 
    {
        if (next && next->GetKeyNum ()>order/2) 
        {
            next->decKeyNum();
            int up=next->GetKeys()[0];
            BPTnode *borrowed=next->getChildren()[0];
            borrowed->SetParent(this);
            BPTnode *lftParent=parent;
            BPTnode *rightParent=next->GetParent();
            while (lftParent!= rightParent) 
            {
                lftParent=lftParent->GetParent();
                rightParent=rightParent->GetParent();
            }
            int pindex=lftParent->GetKeyIndex(up);
            if (lftParent->GetKeys()[pindex]<up)
                pindex--;
            int down = lftParent->GetKeys ()[pindex];
            lftParent->GetKeys ()[pindex]=up;
            keys[keyNum++]=down;
            children[keyNum]=borrowed;
            
            for (int i=0; i<next->GetKeyNum(); i++) 
            {
                next->GetKeys()[i]=next->GetKeys()[i + 1];
                next->getChildren ()[i]=next->getChildren()[i + 1];
            }
            next->getChildren()[next->GetKeyNum()]=next->getChildren()[next->GetKeyNum()+1];   
        } 
        else if(prev && prev->GetKeyNum()>order/2) 
        {
            prev->decKeyNum ();
            int up = prev->GetKeys ()[prev->GetKeyNum ()];
            BPTnode *borrowed = prev->getChildren ()[prev->GetKeyNum () + 1];
            borrowed->SetParent (this);
            BPTnode *lftParent = prev->GetParent ();
            BPTnode *rightParent = parent;
            
            while (lftParent != rightParent)
            {
                lftParent=lftParent->GetParent();
                rightParent=rightParent->GetParent();
            } 
            int pindex=lftParent->GetKeyIndex(up);
            int down=lftParent->GetKeys()[pindex];
            lftParent->GetKeys()[pindex]=up;
            for (int i =keyNum; i>0; i--) 
            {
                keys[i]=next->GetKeys()[i+1];
                children[i+1]=children[i];
            }
            children[1]=children[0];
            keys[0]=down;
            children[0]=borrowed;
        } 
        else if (next)
            merge (next);
        else if (prev)
            prev->merge (this);
    }
}
void InNode::merge(BPTnode* rtNode)
{
    int *rightKeys=rtNode->GetKeys();
    BPTnode **rtChildren=rtNode->getChildren();
    BPTnode *rightParent=rtNode->GetParent();
    BPTnode *lftParent=parent;
    
    while (lftParent!=rightParent) 
    {
        lftParent=lftParent->GetParent();
        rightParent=rightParent->GetParent();
    }
    int pindex=lftParent->GetKeyIndex(rightKeys[0]);
    if (pindex == lftParent->GetKeyNum () || lftParent->GetKeys ()[pindex] > rightKeys[0])
        pindex--;
    int down=lftParent->GetKeys()[pindex];  
    if (keyNum==0) 
    {
        keys[keyNum]=down;
        for (int i=0; i<=rtNode->GetKeyNum(); i++) 
        { 
            keys[keyNum+i+1]=rightKeys[i];
            children[keyNum+i+1]=rtChildren[i];
            children[keyNum+i+1]->SetParent(this);
        }
    } 
    else 
    {
        keys[keyNum - 1]=down;
        for (int i = 0; i<=rtNode->GetKeyNum(); i++) 
        {
            keys[keyNum+i] =rightKeys[i];
            children[keyNum+i+1]=rtChildren[i];
            children[keyNum+i+1]->SetParent (this);   
        } 
    }
    keyNum+=1+rtNode->GetKeyNum();
    next=rtNode->GetNext();
    if (next) 
        next->SetPrev (this);
    rtNode->GetParent ()->Delete (down);
    delete rtNode;
}

BPTnode *InNode::split(int &key)
{
    // New rht node
    InNode *rtNode=new InNode(order);
    // The key to be moved into the the parent
    key=keys[keyNum/2];
    // Copy every key and child after keyNum / 2 to rtNode
    for (int i=keyNum/2 +1; i<keyNum; i++) 
    {
        rtNode->keys[i-keyNum/2-1]=keys[i];
        rtNode->children[i-keyNum/2-1]=children[i];
    }
    rtNode->keyNum=keyNum-keyNum/2-1;
    rtNode->children[rtNode->keyNum]=children[keyNum];
    keyNum=keyNum/2;
    for (int i=0; i<=rtNode->GetKeyNum(); i++) 
    {
        rtNode->children[i]->SetParent(rtNode);
    }
    return rtNode;
}
BPTnode **InNode::getChildren()
{
    return children;
}

