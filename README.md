# Details

## Students

| Roll Number   |        Name        |                 
| :-----------: | :----------------: | 
|    2451033    |   Abhishek Biswas  |                                             
|    2451035    |     Aman Kumar     |                                             
|    2451040    |    Sougata Patra   |                                             
|    2451049    |    Aditiya Raj     |                                             

# Problem Statement:

WAP in C to create the following Fibonacci Heap data structure: Very fast Dijkstra's Shortest Path Problem Solving Technique 

# Acknowledgement:

Firstly, we would like to thank our family and friends for their constant support throughout our university life.

Moreover, we would like to express our sincere gratitude to the Department of Computer Science, for allowing us to apply our expertise in this assignment, and for helping us develop the required knowledge to program using the C programming language for this project.


# Errors Found:

## Incorrect Insertion Into Root List In fib_heap_insert() Function

### Problem

x->left = H->min;
x->right = H->min->right;
H->min->right->left = x;
H->min->right = x;

### Fixing

x->right = H->min->right;
x->left  = H->min;
H->min->right->left = x;
H->min->right = x;

## Incorrect Consolidation Traversal In fib_heap_consolidate() Function

### Problem

FibNode *w = H->min;
FibNode *start = w;
do {
    ...
    w = w->right;
} while (w != start);

### Fixing

FibNode *w = H->min;
FibNode *start = w;
FibNode *next;

do {
    next = w->right;     // Safe backup
    ...
    w = next;
} while (w != start);

## Children Moving To Root List Incorrectly In fib_heap_extract_min() Function

### Problem

x->left = H->min;
x->right = H->min->right;

Issue: You never detach child from its circular sibling list → circular child ring is left intact → root list gets corrupted → infinite loops or crashes.

### Fixing

Before adding a child to root list:

x->left->right = x->right;
x->right->left = x->left;

Only then insert into root list.

## Frees Circular List Incorrectly In free_node() Function

### Problem

do {
    next = curr->right;
    free(curr);
    curr = next;
} while (curr != start);

Issue: You free a node while its right still points to freed memory → undefined behavior.

### Fixing

FibNode *start = node;
FibNode *curr = start->right;

// break circle
start->left->right = NULL;

while (curr != NULL) {
    FibNode *next = curr->right;
    if (curr->child) free_node(curr->child);
    free(curr);
    curr = next;
}

if (start->child) free_node(start->child);
free(start);

## Frees Input Heaps Incorrectly In fib_heap_union() Function

### Problem

free(H1);
free(H2);

### Fixing

// free(H1);
// free(H2);

## Degree Array Size Is Too Small In fib_heap_consolidate() Function

### Problem

int D = (int)(log(H->n) / log(2)) + 1;

### Fixing

int D = (int)(log(H->n) / log(1.61803)) + 2;

## No NULL Checking In fib_heap_insert() Function

### Problem

if (x->key < H->min->key)

### Fixing

if (H->min == NULL || x->key < H->min->key)



