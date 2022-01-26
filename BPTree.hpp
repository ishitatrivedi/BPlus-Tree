#ifndef BPTree_hpp
#define BPTree_hpp

#include "BPTNode.hpp"

class BPTree 
{   
    private:
        int order;
        BPTnode *root;
        BPTnode *search_Recursive(BPTnode *node, int key); 
    public:
        BPTree ();
        ~BPTree ();
        //  Operations for Initialize, insert, Delete and search
        void Initialize(int m);
        void insert(int key, float value);
        bool search(int key, float &value);
        bool *search(int key1, int key2, float *&values, int &num);
        void Delete(int key);
};
#endif /* BPTree_hpp */
