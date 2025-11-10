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

# Time Complexity:

# Errors Found:

## 1) Root List Insertion Bugs:

### Before

x->left = H->min;

x->right = H->min->right;

H->min->right->left = x;

H->min->right = x;

### After

x->right = H->min->right;
x->left = H->min;
H->min->right->left = x;
H->min->right = x;

## 2) Concatenation Bug in fib_heap_union() function:

### Before

FibNode *H1_right = H1->min->right;
FibNode *H2_left = H2->min->left;
H1->min->right = H2->min;
H2->min->left = H1->min;
H1_right->left = H2_left;
H2_left->right = H1_right;

### After

FibNode *H1_left = H1->min->left;
FibNode *H2_left = H2->min->left;

H1->min->left = H2_left;
H2_left->right = H1->min;
H2->min->left = H1_left;
H1_left->right = H2->min;

## 3) Traversal & Array Index Bug  in fib_heap_consolidate() function:

## Before

int D = (int)(log(H->n) / log(2)) + 1;

### After

int D = (int)(log2(H->n)) + 10;  // safer margin

## 4) Not Reseting After Moving The Children in fib_heap_extract_min() function:

### Before

Not Inputed.

### After

z->child = NULL; // At Line 161

## 5) Check Allocation Safety:

After every malloc()/calloc():

if (!H) { perror("malloc failed"); exit(EXIT_FAILURE); }

## 6) Search Limitation Notice:

printf("(search limited to root list)\n"); 






