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

# Time Complexity

| Function              | Time Complexity    |
| :--------------------:| :-----------------:|
| fib_heap_insert       | O(1) amortized     |
| fib_heap_min          | O(1)               |
| fib_heap_union        | O(1)               |
| fib_heap_extract_min  | O(log n) amortized |
| fib_heap_consolidate  | O(log n)           |
| fib_heap_decrease_key | O(1) amortized     |
| fib_heap_delete       | O(log n) amortized |
| fib_heap_find         | O(n)               |
| fib_heap_print        | O(r)<=O(n)         |
| fib_heap_free         | O(n)               |

# Errors Found & Fixes Applied

## 1. Incorrect insertion into root list in fib_heap_insert()

### Original:
x->left = H->min;
x->right = H->min->right;
H->min->right->left = x;
H->min->right = x;

### Fixed:
x->right = H->min->right;
x->left  = H->min;
H->min->right->left = x;
H->min->right = x;

---

## 2. Unsafe traversal during consolidation (fib_heap_consolidate)

### Original Issue:
Iterating directly over root list while linking nodes modifies the list and corrupts traversal.

### Fixed:
Create a temporary array copy of the entire root list BEFORE modifying it:

Root list collected safely:
FibNode **rootList = malloc(root_count * sizeof(FibNode*));

Then consolidate using the safe snapshot.

---

## 3. Incorrect array size for degree table A[] in consolidation

### Original:
int D = (int)(log(H->n) / log(2)) + 1;

### Fixed:
int D = (int)(log2(H->n)) + 10;

Added margin to prevent overflow when linking repeatedly.

---

## 4. Incorrect linking of nodes in fib_heap_link()

### Original Issue:
Child insertion logic didn’t maintain circular list integrity.

### Fixed:
Proper circular child insertion:

if (x->child == NULL) {
    x->child = y;
    y->left = y->right = y;
} else {
    y->right = x->child->right;
    y->left  = x->child;
    x->child->right->left = y;
    x->child->right = y;
}

---

## 5. Incorrect child promotion during extract_min

### Original Issue:
Children inserted without resetting parent or mark flags.

### Fixed:
c->parent = NULL;
insert_into_root(H, c);

Ensures correctness for future decrease-key operations.

---

## 6. Unsafe circular list removal in extract_min

### Original Issue:
Root list removal corruption when z->right == z not handled correctly.

### Fixed:
if (z == z->right)
    H->min = NULL;
else {
    H->min = z->right;
    fib_heap_consolidate(H);
}

---

## 7. fib_heap_cut() child removal bug

### Original Issue:
Child pointer not updated when the removed child is y->child.

### Fixed:
if (y->child == x)
    y->child = x->right;

---

## 8. Missing mark reset after cut

### Original:
(no mark reset)

### Fixed:
x->mark = 0;

---

## 9. Missing deep search for decrease-key and delete

### Added:
FibNode* fib_heap_find(FibNode *root, int key);

Used recursively:

- In decrease-key
- In delete

---

## 10. Union logic did not properly concatenate circular root lists

### Original:
Partially concatenated lists incorrectly.

### Fixed:
Proper circular list stitching:

a->right->left = b->left;
b->left->right = a->right;
a->right = b;
b->left = a;

---

## 11. Freeing nodes unsafely in fib_heap_free()

### Original:
free_node() double-freed child lists.

### Fixed:
Safe recursive freeing maintaining circular boundaries.

---

## 12. insert_into_root() rewritten for consistent safe insertion

Centralized canonical root insertion logic:
if (H->min == NULL) { ... } else { ... }

---

## 13. Consolidation uses temporary root list snapshot

Avoids modifying list during iteration.

---

## 14. main.c updated with deep search + safe input handling

New features:
- fib_heap_find() used for decrease/delete
- free(extracted_node)
- No root-only search
- Cleaner menu logic

---

## 15. Added protection to avoid using log(0)

Consolidation only runs when H->n > 0.

---

## 16. General code cleanup and correctness improvements

- Removed undefined behavior
- Fixed circular list manipulations
- Ensured correct parent-child relations
- Ensured correct marking rules
- Ensured proper rebuild of root list after consolidation




