#include <stdio.h>
#include <stdlib.h>

typedef struct FibNode {
    int key;
    int degree;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
    int mark; 
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int n;
} FibHeap;


FibHeap* make_fib_heap();
FibNode* make_fib_node(int key);


FibNode* fib_heap_insert(FibHeap *H, int key);
FibNode* fib_heap_min(FibHeap *H);
FibHeap* fib_heap_union(FibHeap *H1, FibHeap *H2);
FibNode* fib_heap_extract_min(FibHeap *H);
void fib_heap_decrease_key(FibHeap *H, FibNode *x, int k);
void fib_heap_delete(FibHeap *H, FibNode *x);


void fib_heap_print(FibHeap *H);
void fib_heap_free(FibHeap *H);