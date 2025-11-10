#ifndef MODIFIED_FIB_HEAP_H
#define MODIFIED_FIB_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

/*-----------------------------------------
  STRUCTURE DEFINITIONS
------------------------------------------*/

/* Node structure for Fibonacci Heap */
typedef struct FibNode {
    int key;                    // Value/key stored in the node
    int degree;                 // Number of children
    int mark;                   // Mark for cascading cut
    struct FibNode *parent;     // Pointer to parent node
    struct FibNode *child;      // Pointer to one of its children
    struct FibNode *left;       // Left sibling (circular list)
    struct FibNode *right;      // Right sibling (circular list)
} FibNode;

/* Fibonacci Heap structure */
typedef struct FibHeap {
    FibNode *min;               // Pointer to the minimum node
    int n;                      // Number of nodes in heap
} FibHeap;


/*-----------------------------------------
  FUNCTION DECLARATIONS
------------------------------------------*/

/* Core Heap Operations */
FibHeap* make_fib_heap();                              // Create a new heap
FibNode* make_fib_node(int key);                       // Create a new node
FibNode* fib_heap_insert(FibHeap *H, int key);         // Insert key
FibNode* fib_heap_min(FibHeap *H);                     // Get min node
FibNode* fib_heap_extract_min(FibHeap *H);             // Remove min node
FibHeap* fib_heap_union(FibHeap *H1, FibHeap *H2);     // Merge two heaps

/* Key Modification Operations */
void fib_heap_decrease_key(FibHeap *H, FibNode *x, int k); // Decrease key
void fib_heap_delete(FibHeap *H, FibNode *x);              // Delete node

/* Utility Operations */
void fib_heap_print(FibHeap *H);                       // Print root list
void fib_heap_free(FibHeap *H);                        // Free heap

/* Internal Helpers (used internally, can be static in main file) */
static void fib_heap_link(FibHeap *H, FibNode *y, FibNode *x);
static void fib_heap_consolidate(FibHeap *H);
static void fib_heap_cut(FibHeap *H, FibNode *x, FibNode *y);
static void fib_heap_cascading_cut(FibHeap *H, FibNode *y);
static void free_node(FibNode *node);

#endif // MODIFIED_FIB_HEAP_H
