//
//  BPTnode.hpp
//  BPTreeNode_Declaration
#ifndef BPTNode_hpp
#define BPTNode_hpp

#include<stdio.h>
#include<cstddef>
#include<iostream>

using namespace std;

class BPTnode 
{
    protected: 
        int order;
        int *keys;
        int keyNum;
        bool isLeaf;
        BPTnode *parent;
        BPTnode *prev;
        BPTnode *next;
    public: 
        BPTnode(int n);
        ~BPTnode();
        bool IsLeaf();
        int *GetKeys();
        int GetKeyNum();
        void decKeyNum();
        int GetKeyIndex(int key);
        BPTnode *GetParent();
        BPTnode *GetNext();
        BPTnode *GetPrev();
        void SetParent(BPTnode *node);
        void SetNext(BPTnode *node);
        void SetPrev(BPTnode *node);
        //virtual functions for run time polymorphism
        virtual void insert(int key, float value) {}  
        virtual void insert(int key, BPTnode* rtChild) {}
        virtual void insert(int key, BPTnode* lftChild, BPTnode* rtChild) {}
        virtual void Delete(int key) {}
        virtual void merge(BPTnode* rtNode) {}
        virtual void search(int key) {}
        virtual void search(int key1, int key2) {}
        virtual BPTnode *split(int &key) { return NULL; }
        virtual BPTnode **getChildren() { return NULL; }
        virtual float *getValues() { return NULL; }      
};


// internal node
class InNode:public BPTnode
{
    private:
        BPTnode **children;
    public:   
        InNode(int n);
        ~InNode();
        void insert(int key, BPTnode* rtChild);
        void insert(int key, BPTnode* lftChild, BPTnode* rtChild);
        void Delete(int key);
        void merge(BPTnode* rtNode);
        BPTnode *split(int &key);
        BPTnode **getChildren(); 
};


// leaf node
class LNode:public BPTnode
{
    private:
        float *values;   
    public:
        LNode(int n);
        ~LNode(); 
        void insert(int key, float value);
        void Delete(int key);
        void merge(BPTnode* rtNode);
        BPTnode *split(int &key);
        float *getValues(); 
};

#endif /* BPTreeNode_hpp */
