#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "modified_fib_heap.h"

/* ============================
   BASIC CREATION FUNCTIONS
   ============================ */
FibHeap* make_fib_heap() {
    FibHeap *H = (FibHeap*)malloc(sizeof(FibHeap));
    H->min = NULL;
    H->n = 0;
    return H;
}

FibNode* make_fib_node(int key) {
    FibNode *x = (FibNode*)malloc(sizeof(FibNode));
    x->key = key;
    x->degree = 0;
    x->mark = 0;
    x->parent = NULL;
    x->child = NULL;
    x->left = x;
    x->right = x;
    return x;
}

/* Insert into root list */
static void insert_into_root(FibHeap *H, FibNode *x) {
    if (H->min == NULL) {
        H->min = x;
        x->left = x->right = x;
    } else {
        x->right = H->min->right;
        x->left = H->min;
        H->min->right->left = x;
        H->min->right = x;
        if (x->key < H->min->key)
            H->min = x;
    }
}

FibNode* fib_heap_insert(FibHeap *H, int key) {
    FibNode *x = make_fib_node(key);
    insert_into_root(H, x);
    H->n++;
    return x;
}

FibNode* fib_heap_min(FibHeap *H) {
    return H->min;
}

/* ============================
   UNION OF TWO HEAPS
   ============================ */
FibHeap* fib_heap_union(FibHeap *H1, FibHeap *H2) {
    if (H1->min == NULL) return H2;
    if (H2->min == NULL) return H1;

    FibHeap *H = make_fib_heap();

    /* concatenate root lists */
    FibNode *a = H1->min;
    FibNode *b = H2->min;

    a->right->left = b->left;
    b->left->right = a->right;

    a->right = b;
    b->left = a;

    /* pick new min */
    H->min = (H1->min->key < H2->min->key) ? H1->min : H2->min;
    H->n = H1->n + H2->n;

    free(H1);
    free(H2);

    return H;
}

/* ============================
   LINK TWO TREES
   ============================ */
static void fib_heap_link(FibHeap *H, FibNode *y, FibNode *x) {

    /* remove y from root list */
    y->left->right = y->right;
    y->right->left = y->left;

    /* make y child of x */
    y->parent = x;
    y->mark = 0;

    if (x->child == NULL) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
}

/* ============================
   CONSOLIDATE
   ============================ */
static void fib_heap_consolidate(FibHeap *H) {

    int D = (int)(log2(H->n)) + 10;
    FibNode **A = calloc(D, sizeof(FibNode*));

    FibNode *start = H->min;
    FibNode *w = H->min;

    /* collect root list nodes first */
    int root_count = 0;
    do {
        root_count++;
        w = w->right;
    } while (w != start);

    FibNode **rootList = malloc(root_count * sizeof(FibNode*));

    w = start;
    for (int i = 0; i < root_count; i++) {
        rootList[i] = w;
        w = w->right;
    }

    /* consolidate */
    for (int i = 0; i < root_count; i++) {
        FibNode *x = rootList[i];
        int d = x->degree;

        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (y->key < x->key) {
                FibNode *tmp = x;
                x = y;
                y = tmp;
            }
            fib_heap_link(H, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    free(rootList);

    /* rebuild root list */
    H->min = NULL;
    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            A[i]->left = A[i]->right = A[i];
            if (H->min == NULL) {
                H->min = A[i];
            } else {
                insert_into_root(H, A[i]);
            }
        }
    }

    free(A);
}

/* ============================
   EXTRACT MINIMUM
   ============================ */
FibNode* fib_heap_extract_min(FibHeap *H) {
    FibNode *z = H->min;

    if (z != NULL) {
        /* promote children */
        if (z->child != NULL) {
            FibNode *c = z->child;
            FibNode *start = c;

            do {
                FibNode *next = c->right;
                c->parent = NULL;
                insert_into_root(H, c);
                c = next;
            } while (c != start);
        }

        /* remove z */
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

/* ============================
   CUT OPERATIONS
   ============================ */
static void fib_heap_cut(FibHeap *H, FibNode *x, FibNode *y) {
    /* remove x from child list of y */
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
    x->mark = 0;

    insert_into_root(H, x);
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

/* ============================
   DECREASE KEY
   ============================ */
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

/* ============================
   DELETE NODE
   ============================ */
void fib_heap_delete(FibHeap *H, FibNode *x) {
    fib_heap_decrease_key(H, x, INT_MIN);
    FibNode *m = fib_heap_extract_min(H);
    free(m);
}

/* ============================
   FIND NODE (DEEP SEARCH)
   ============================ */
FibNode* fib_heap_find(FibNode *root, int key) {
    if (!root) return NULL;

    FibNode *start = root;
    FibNode *w = root;

    do {
        if (w->key == key)
            return w;

        if (w->child) {
            FibNode *res = fib_heap_find(w->child, key);
            if (res) return res;
        }

        w = w->right;
    } while (w != start);

    return NULL;
}

/* ============================
   PRINT ROOT LIST
   ============================ */
void fib_heap_print(FibHeap *H) {
    if (H->min == NULL) {
        printf("Heap is empty.\n");
        return;
    }

    FibNode *x = H->min;
    printf("Root list: ");
    do {
        printf("%d ", x->key);
        x = x->right;
    } while (x != H->min);

    printf("\n");
}

/* ============================
   FREE HEAP
   ============================ */
static void free_recursive(FibNode *x) {
    if (!x) return;

    FibNode *start = x;
    FibNode *w = x;

    do {
        FibNode *next = w->right;
        if (w->child)
            free_recursive(w->child);
        free(w);
        w = next;
    } while (w != start);
}

void fib_heap_free(FibHeap *H) {
    if (H->min)
        free_recursive(H->min);
    free(H);
}

/* ============================
   MAIN
   ============================ */

int main() {
    FibHeap *H = make_fib_heap();
    int choice, key, newKey;

    printf("\n=============================================\n");
    printf("        Fibonacci Heap Implementation         \n");
    printf("=============================================\n");

    while (1) {
        printf("\n---------------------------------------------\n");
        printf("1. Insert a key\n");
        printf("2. Find Minimum\n");
        printf("3. Extract Minimum\n");
        printf("4. Decrease a Key\n");
        printf("5. Delete a Key\n");
        printf("6. Display Root List\n");
        printf("7. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Exiting...\n");
            break;
        }

        switch (choice) {

            /* ---------------------- INSERT ---------------------- */
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);

                fib_heap_insert(H, key);
                printf("Inserted key %d successfully.\n", key);
                break;

            /* ---------------------- FIND MIN ---------------------- */
            case 2: {
                FibNode *min = fib_heap_min(H);
                if (min)
                    printf("Minimum element: %d\n", min->key);
                else
                    printf("Heap is empty.\n");
                break;
            }

            /* ---------------------- EXTRACT MIN ---------------------- */
            case 3: {
                FibNode *minNode = fib_heap_extract_min(H);
                if (minNode) {
                    printf("Extracted minimum key: %d\n", minNode->key);
                    free(minNode);
                } else {
                    printf("Heap is empty.\n");
                }
                break;
            }

            /* ---------------------- DECREASE KEY ---------------------- */
            case 4: {
                printf("Enter the key to decrease: ");
                scanf("%d", &key);

                FibNode *node = fib_heap_find(H->min, key);
                if (!node) {
                    printf("Key %d not found (deep search).\n", key);
                    break;
                }

                printf("Enter new (smaller) key: ");
                scanf("%d", &newKey);

                fib_heap_decrease_key(H, node, newKey);
                printf("Key %d decreased to %d successfully.\n", key, newKey);
                break;
            }

            /* ---------------------- DELETE KEY ---------------------- */
            case 5: {
                printf("Enter key to delete: ");
                scanf("%d", &key);

                FibNode *node = fib_heap_find(H->min, key);
                if (!node) {
                    printf("Key %d not found (deep search).\n", key);
                    break;
                }

                fib_heap_delete(H, node);
                printf("Deleted key %d successfully.\n", key);
                break;
            }

            /* ---------------------- PRINT ROOT LIST ---------------------- */
            case 6:
                fib_heap_print(H);
                break;

            /* ---------------------- EXIT ---------------------- */
            case 7:
                fib_heap_free(H);
                printf("Exiting... Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    fib_heap_free(H);
    return 0;
}
