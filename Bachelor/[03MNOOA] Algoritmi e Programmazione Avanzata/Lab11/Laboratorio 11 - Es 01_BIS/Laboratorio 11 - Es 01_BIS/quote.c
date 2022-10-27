//
//  quote.c
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "quote.h"


/* --- DATA --- */

//NODE OF QUOTATION
struct BSTnode {
    
    //TREE NODE
    quote_t node;
    
    //LINK WITH PARENT
    link father;
    
    //LINK WITH NODE'S CHILD
    link left;
    link right;
};

//COLLECTION OF QUOTATIONS
struct BinarySearchTree {
    //TREE ROOT
    link root;
    
    //SENTINEL NODE
    link z;
    
};

/* --- FUNCTIONS --- */

//Create and free
BST BSTinit (void) {
    
    //DYNAMIC ALLOCATION
    BST bst = (BST) malloc(sizeof(struct BinarySearchTree));
    if (bst == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //SETTING NULL THE ROOT OF THE TREE
    bst->root = (bst->z = NEW(exchange_SET_NULL(), NULL, NULL, NULL));
    
    return bst;
}
quote_t exchange_SET_NULL (void) {
    quote_t exchange;
    exchange_INIT(&exchange);
    return exchange;
}
void BSTfree (BST bst) {
    if (bst == NULL)
        return;
    
    //TREE FREE
    tree_FREE(bst->root, bst->z);
    free(bst->z);
    free(bst);
}
void tree_FREE (link root, link z) {
    if (root == z)
        return;
    
    tree_FREE(root->left, z);
    tree_FREE(root->right, z);
    free(root);
}
void exchange_INIT (quote_t *exchange) {
    
    //INITIALIZING DATE
    exchange->date.year = 0;
    exchange->date.month = 0;
    exchange->date.day = 0;
    exchange->date.time.hours = 0;
    exchange->date.time.minutes = 0;
    
    //INITIALIZING VALUES
    exchange->numerator = 0;
    exchange->denominator = 0;
}

//Input and output
void exchange_READ (FILE *fp, quote_t *exchange) {
    
    //READING DATES
    date_READ(fp, &exchange->date);
    
    //STORING USEFUL INFORMATIONS ABOUT THE QUOTATION
    int amount;
    float value;
    fscanf(fp, "%f %d\n", &value, &amount);
    
    exchange->numerator = exchange->numerator + value*amount;
    exchange->denominator = exchange->denominator + amount;
}
void BST_SHOW (BST bst) {
    BST_SHOW_R(bst->root, bst->z);
}
void BST_SHOW_R (link root, link z) {
    
    //MOVING LEFT IN RECURSION
    if (root->left != z)
        BST_SHOW_R(root->left, z);
    
    //PRINTING DATA
    quotation_SHOW(root);
    
    //MOVING RIGHT
    if (root->right != z)
        BST_SHOW_R(root->right, z);
    
    return;
    
}
void quotation_SHOW (link x) {
    date_SHOW(x->node.date);
    fprintf(stdout, "QUOTATION VALUE: %.4f\n", (x->node.numerator)/(x->node.denominator));
}
link sentinel_CHECK (BST bst) {
    return bst->z;
}

//Operation
void BSTinsert_leaf (BST bst, quote_t exchange) {
    bst->root = insert(bst->root, exchange, bst->z);
}
void BSTupload (link x, quote_t exchange) {
    x->node.numerator = x->node.numerator + exchange.numerator;
    x->node.denominator = x->node.denominator + exchange.denominator;
}
link BSTsearch (BST bst, date_t date) {
    return BSTsearch_R(bst->root, date, bst->z);
}
link BSTsearch_R (link root, date_t date, link z) {
    
    //TERMINAL CONDITION
    if (root == z)
        return z;
    
    //RECURSIVELY MOVING IN THE TREE
    //MOVING RIGHT
    if (date_COMPARE(root->node.date, date) == -1)
        return BSTsearch_R(root->right, date, z);
    
    //MOVING LEFT
    else if (date_COMPARE(root->node.date, date) == 1)
        return BSTsearch_R(root->left, date, z);
    
    //NODE FOUND
    else
        return root;
}
link insert (link h, quote_t exchange, link z) {
    
    //TERMINAL CONDITION
    if (h == z)
        return NEW(exchange, z, z, z);
    
    //MOVING LEFT
    if (date_COMPARE(exchange.date, h->node.date) == -1) {
        h->left = insert(h->left, exchange, z);
        h->left->father = h;
    }
    
    //MOVING RIGHT
    else {
        h->right = insert(h->right, exchange, z);
        h->right->father = h;
    }
    
    return h;
}
link NEW (quote_t exchange, link father, link left, link right) {
    
    //DYNAMIC ALLOCATION FOR THE NEW NODE
    link x = (link) malloc(sizeof(struct BSTnode));
    if (x == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //COPYING INFORMATIONS
    date_COPY(&x->node.date, exchange.date);
    x->node.numerator = exchange.numerator;
    x->node.denominator = exchange.denominator;
    x->left = left;
    x->right = right;
    x->father = father;
    
    return x;
}
int count (BST bst) {
    return count_R(bst->root, bst->z);
}
int count_R (link x, link z) {
    
    //TERMINAL CONDITION
    if (x == z)
        return 0;
    
    //COUNTING LEFT AND RIGHT TREE RECURSIVELY
    return count_R(x->left, z) + count_R(x->right, z) + 1;
}
void maxmin_SEARCH_byTREE (BST bst, date_t date_A, date_t date_B) {
    
    //VARIABLES USED
    quote_t max, min;
    
    //INITIALIZING VARIABLES
    max.numerator = 0;
    max.denominator = 1;
    min.numerator = INT_MAX;
    min.denominator = 1;
    
    //SEARCH FOR VALUES
    maxmin_SEARCH_byTREE_R (bst->root, bst->z, date_A, date_B, &max, &min);
    
    //SHOW VALUES
    fprintf(stdout, "\n\t[MAX VALUE]: ");
    date_SHOW(max.date);
    fprintf(stdout, "%f\n", max.numerator/max.denominator);
    
    fprintf(stdout, "\n\t[MIN VALUE]: ");
    date_SHOW(min.date);
    fprintf(stdout, "%f\n", min.numerator/min.denominator);
}
void maxmin_SEARCH_byTREE_R (link x, link z, date_t date_A, date_t date_B, quote_t *max, quote_t *min) {
    
    //IN_ORDER ALGORITHM USED TO RECURSIVELY SEARCH FOR VALID NODES IN THE TREE
    //PRUNING: MOVING LEFT OR RIGHT DEPENDS ON THE INTERVAL (THE ALGORITHM DOES NOT VISIT EVERY NODES)
    
    //TERMINAL CONDITION
    if (x == z)
        return;
    
    //MOVING LEFT
    if (date_COMPARE(x->node.date, date_A) == 1)
        maxmin_SEARCH_byTREE_R(x->left, z, date_A, date_B, max, min);
    
    //VISITING NODE
    if (date_COMPARE(x->node.date, date_A) == 1 && date_COMPARE(x->node.date, date_B) == -1) {
        
        //COMPUTING MAX
        if ((x->node.numerator/x->node.denominator) > (max->numerator/max->denominator)) {
            max->denominator = x->node.denominator;
            max->numerator = x->node.numerator;
            date_COPY(&max->date, x->node.date);
        }
        
        //COMPUTING MIN
        if ((x->node.numerator/x->node.denominator) < (min->numerator/min->denominator)) {
            min->denominator = x->node.denominator;
            min->numerator = x->node.numerator;
            date_COPY(&min->date, x->node.date);
        }
    }
    
    //MOVING RIGHT
    if (date_COMPARE(x->node.date, date_B) == -1)
        maxmin_SEARCH_byTREE_R(x->right, z, date_A, date_B, max, min);
}
float balanceValue_COMPUTE_byTREE (BST bst) {
    
    //COMPUTING HEIGHT (MAX PATH TO NODES)
    float height = height_COMPUTE (bst->root, bst->z);
    
    //COMPUTING SHORTEST PATH TO NODES
    float short_path = short_COMPUTE (bst->root, bst->z);
    
    return height/short_path;
}
float height_COMPUTE (link x, link z) {
    
    //TERMINAL CONDITION
    if (x == z)
        return 0;
    
    //COMPUTING LEFT AND RIGHT TREE HEIGHT RECURSIVELY
    float height_LEFT = height_COMPUTE(x->left, z);
    float height_RIGHT = height_COMPUTE(x->right, z);
    
    //RETURNING THE LARGEST VALUE (+1 FOR THE NODE VISITED)
    if (height_LEFT > height_RIGHT)
        return height_LEFT + 1;
    return height_RIGHT + 1;
        
}
float short_COMPUTE (link x, link z) {
    
    //TERMINAL CONDITION
    if (x == z)
        return 0;
    
    //COMPUTING LEFT AND RIGHT SHORTEST TREE PATH FOROM ROOT TO LEAFS
    float short_LEFT = short_COMPUTE(x->left, z);
    float short_RIGHT = short_COMPUTE(x->right, z);
    
    //RETURNING SHORTEST VALUE
    if (short_LEFT < short_RIGHT)
        return short_LEFT + 1;
    return short_RIGHT + 1;
}
void balance_byTREE (BST bst) {
    bst->root = balance_byTREE_R(bst->root, bst->z);
}
link balance_byTREE_R (link x, link z) {
    int medium_position;
    
    //TERMINAL CONDITION
    if (x == z)
        return z;
    
    //COMPUTING MEDIUM KEY POSITION
    medium_position = (count_R(x, z) + 1)/2 - 1;
    
    //PARTITIONING THE TREE BY THE MEDIUM KEY
    x = partR(x, z, medium_position);
    
    //BALANCING NEW LEFT AND RIGHT TREE RECURSUVELY
    x->left = balance_byTREE_R(x->left, z);
    x->right = balance_byTREE_R(x->right, z);
    
    return x;
    
}
link partR (link x, link z, int value) {
    
    //COUNTING NODES IN THE LEFT SUBTREE
    int t = count_R(x->left, z);
    
    //PARTITIONING LEFT SUBTREE PERFORMING ROTATION TO THE RIGHT
    if (t > value) {
        x->left = partR(x->left, z, value);
        x = rotR(x);
    }
    
    //PARTITIONING RIGHT SUBTREE PERFORMING PARTITION TO THE LEFT
    if (t < value) {
        x->right = partR(x->right, z, value-t-1);
        x = rotL(x);
    }
    
    return x;
}
link rotR (link x) {
    link tmp = x->left;
    x->left = tmp->right;
    tmp->right->father = x;
    tmp->right = x;
    tmp->father = x->father;
    x->father = tmp;
    return tmp;
}
link rotL (link x) {
    link tmp = x->right;
    x->right = tmp->left;
    tmp->left->father = x;
    tmp->left = x;
    tmp->father = x->father;
    x->father = tmp;
    return tmp;
}
