#include <stdio.h>
#include <stdlib.h>
#include "fib_heap.h"
#include <limits.h>
#include <math.h>


/* Helper to initialize a new heap */
FibHeap* make_fib_heap() {
    FibHeap *H = (FibHeap*)malloc(sizeof(FibHeap));
    H->min = NULL;
    H->n = 0;
    return H;
}

/* Helper to create a new node */
FibNode* make_fib_node(int key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;
    return node;
}

/* Insert node into heap */
FibNode* fib_heap_insert(FibHeap *H, int key) {
    FibNode *x = make_fib_node(key);
    if (H->min == NULL) {
        H->min = x;
    } else {
        // Insert into root list
        x->left = H->min;
        x->right = H->min->right;
        H->min->right->left = x;
        H->min->right = x;
        if (x->key < H->min->key)
            H->min = x;
    }
    H->n++;
    return x;
}

/* Return min node */
FibNode* fib_heap_min(FibHeap *H) {
    return H->min;
}

/* Merge two heaps */
FibHeap* fib_heap_union(FibHeap *H1, FibHeap *H2) {
    FibHeap *H = make_fib_heap();
    H->min = H1->min;
    if (H->min == NULL || (H2->min != NULL && H2->min->key < H1->min->key))
        H->min = H2->min;

    if (H1->min != NULL && H2->min != NULL) {
        // Concatenate root lists
        FibNode *H1_right = H1->min->right;
        FibNode *H2_left = H2->min->left;
        H1->min->right = H2->min;
        H2->min->left = H1->min;
        H1_right->left = H2_left;
        H2_left->right = H1_right;
    }

    H->n = H1->n + H2->n;
    free(H1);
    free(H2);
    return H;
}

/* Helper: Link two trees of same degree */
static void fib_heap_link(FibHeap *H, FibNode *y, FibNode *x) {
    // Remove y from root list
    y->left->right = y->right;
    y->right->left = y->left;
    // Make y a child of x
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

/* Consolidate after extracting min */
static void fib_heap_consolidate(FibHeap *H) {
    int D = (int)(log(H->n) / log(2)) + 1;
    FibNode **A = (FibNode**)calloc(D, sizeof(FibNode*));

    FibNode *w = H->min;
    FibNode *start = w;
    if (w == NULL) return;

    do {
        FibNode *x = w;
        int d = x->degree;
        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            fib_heap_link(H, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        w = w->right;
    } while (w != start);

    H->min = NULL;
    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            if (H->min == NULL) {
                H->min = A[i];
                A[i]->left = A[i]->right = A[i];
            } else {
                A[i]->left = H->min;
                A[i]->right = H->min->right;
                H->min->right->left = A[i];
                H->min->right = A[i];
                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }
    free(A);
}

/* Extract minimum node */
FibNode* fib_heap_extract_min(FibHeap *H) {
    FibNode *z = H->min;
    if (z != NULL) {
        // Move z’s children to root list
        FibNode *x = z->child;
        if (x != NULL) {
            FibNode *start = x;
            do {
                FibNode *next = x->right;
                // Add x to root list
                x->parent = NULL;
                x->left = H->min;
                x->right = H->min->right;
                H->min->right->left = x;
                H->min->right = x;
                x = next;
            } while (x != start);
        }

        // Remove z from root list
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
            H->min = NULL;
        else {
            H->min = z->right;
            fib_heap_consolidate(H);
        }
        H->n--;
    }
    return z;
}

/* Helper: Cut and Cascading Cut */
static void fib_heap_cut(FibHeap *H, FibNode *x, FibNode *y) {
    // Remove x from child list of y
    if (x->right == x)
        y->child = NULL;
    else {
        if (y->child == x)
            y->child = x->right;
        x->left->right = x->right;
        x->right->left = x->left;
    }
    y->degree--;

    
    x->parent = NULL;
    x->left = H->min;
    x->right = H->min->right;
    H->min->right->left = x;
    H->min->right = x;
    x->mark = 0;
}

static void fib_heap_cascading_cut(FibHeap *H, FibNode *y) {
    FibNode *z = y->parent;
    if (z != NULL) {
        if (y->mark == 0)
            y->mark = 1;
        else {
            fib_heap_cut(H, y, z);
            fib_heap_cascading_cut(H, z);
        }
    }
}



void fib_heap_decrease_key(FibHeap *H, FibNode *x, int k) {
    if (k > x->key) {
        printf("Error: new key is greater than current key\n");
        return;
    }
    x->key = k;
    FibNode *y = x->parent;
    if (y != NULL && x->key < y->key) {
        fib_heap_cut(H, x, y);
        fib_heap_cascading_cut(H, y);
    }
    if (x->key < H->min->key)
        H->min = x;
}


void fib_heap_delete(FibHeap *H, FibNode *x) {
    fib_heap_decrease_key(H, x, INT_MIN);
    fib_heap_extract_min(H);
}


void fib_heap_print(FibHeap *H) {
    if (H->min == NULL) {
        printf("Heap is empty.\n");
        return;
    }
    FibNode *x = H->min;
    FibNode *start = x;
    printf("Root list: ");
    do {
        printf("%d ", x->key);
        x = x->right;
    } while (x != start);
    printf("\n");
}


static void free_node(FibNode *node) {
    if (node == NULL) return;
    FibNode *start = node;
    FibNode *curr = node;
    do {
        FibNode *next = curr->right;
        if (curr->child != NULL)
            free_node(curr->child);
        free(curr);
        curr = next;
    } while (curr != start);
}

void fib_heap_free(FibHeap *H) {
    if (H->min != NULL)
        free_node(H->min);
    free(H);
}




int main() {
    FibHeap *H = make_fib_heap();
    FibNode *node;
    int choice, key, newKey;
    
    printf("\n===== Fibonacci Heap Implementation =====\n");

    while (1) {
        printf("\n-----------------------------------------\n");
        printf("1. Insert a key\n");
        printf("2. Find Minimum\n");
        printf("3. Extract Minimum\n");
        printf("4. Decrease a Key\n");
        printf("5. Delete a Node\n");
        printf("6. Display Heap (Root List)\n");
        printf("7. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                fib_heap_insert(H, key);
                printf("Inserted key %d successfully.\n", key);
                break;

            case 2:
                node = fib_heap_min(H);
                if (node)
                    printf("Minimum key in heap: %d\n", node->key);
                else
                    printf("Heap is empty.\n");
                break;

            case 3:
                node = fib_heap_extract_min(H);
                if (node) {
                    printf("Extracted minimum key: %d\n", node->key);
                    free(node); // free extracted node
                } else
                    printf("Heap is empty.\n");
                break;

            case 4:
                printf("Enter the current key to decrease: ");
                scanf("%d", &key);
                printf("Enter new (smaller) key value: ");
                scanf("%d", &newKey);
                
                // Search node manually in root list (simple demo)
                node = H->min;
                if (node == NULL) {
                    printf("Heap is empty.\n");
                    break;
                }
                FibNode *start = node;
                int found = 0;
                do {
                    if (node->key == key) {
                        fib_heap_decrease_key(H, node, newKey);
                        printf("Decreased key %d to %d successfully.\n", key, newKey);
                        found = 1;
                        break;
                    }
                    node = node->right;
                } while (node != start);

                if (!found)
                    printf("Node with key %d not found in root list (note: deep search not implemented).\n", key);
                break;

            case 5:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                node = H->min;
                if (node == NULL) {
                    printf("Heap is empty.\n");
                    break;
                }
                start = node;
                found = 0;
                do {
                    if (node->key == key) {
                        fib_heap_delete(H, node);
                        printf("Deleted node with key %d successfully.\n", key);
                        found = 1;
                        break;
                    }
                    node = node->right;
                } while (node != start);

                if (!found)
                    printf("Node with key %d not found in root list (deep search not implemented).\n", key);
                break;

            case 6:
                fib_heap_print(H);
                break;

            case 7:
                fib_heap_free(H);
                printf("Exiting program. Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}