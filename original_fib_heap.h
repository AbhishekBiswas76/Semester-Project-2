#ifndef ORIGINAL_FIB_HEAP_H
#define ORIGINAL_FIB_HEAP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct FibNode {
    int key;
    int degree;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
    int mark; /* 0 or 1 */
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int n; /* number of nodes */
} FibHeap;

/* creation */
FibHeap* make_fib_heap();
FibNode* make_fib_node(int key);

/* basic operations */
FibNode* fib_heap_insert(FibHeap *H, int key);
FibNode* fib_heap_min(FibHeap *H);
FibHeap* fib_heap_union(FibHeap *H1, FibHeap *H2);
FibNode* fib_heap_extract_min(FibHeap *H);
void fib_heap_decrease_key(FibHeap *H, FibNode *x, int k);
void fib_heap_delete(FibHeap *H, FibNode *x);

/* utility / debug */
void fib_heap_print(FibHeap *H);
void fib_heap_free(FibHeap *H);

#endif /* FIB_HEAP_H */