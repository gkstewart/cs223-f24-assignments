#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static struct tree_node* create_node(const char* name) {
    struct tree_node* new_node = (struct tree_node*)malloc(sizeof(struct tree_node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed for new node\n");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->data.name, name, sizeof(new_node->data.name) - 1);
    new_node->data.name[sizeof(new_node->data.name) - 1] = '\0'; // Ensure null termination
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

struct tree_node* find(const char* name, struct tree_node* root) {
    if (root == NULL) {
        return NULL; // Not found
    }
    int cmp = strcmp(name, root->data.name);
    if (cmp == 0) {
        return root; // Found
    } else if (cmp < 0) {
        return find(name, root->left); // Search left subtree
    } else {
        return find(name, root->right); // Search right subtree
    }
}

struct tree_node* insert(const char* name, struct tree_node* root) {
    if (root == NULL) {
        return create_node(name); // Create and return a new node if tree is empty
    }
    int cmp = strcmp(name, root->data.name);
    if (cmp < 0) {
        root->left = insert(name, root->left); // Insert into left subtree
    } else if (cmp > 0) {
        root->right = insert(name, root->right); // Insert into right subtree
    }
    return root; 
}

void clear(struct tree_node* root) {
    if (root != NULL) {
        clear(root->left); // Clear left subtree
        clear(root->right); // Clear right subtree
        free(root); // Free current node
    }
}

void print(struct tree_node* root) {
    if (root != NULL) {
        printf("%s\n", root->data.name); // Print the current node
        print(root->left); // Print left subtree
        print(root->right); // Print right subtree
    }
}

void printSorted(struct tree_node* root) {
    if (root != NULL) {
        printSorted(root->left); // Print left subtree
        printf("%s\n", root->data.name); // Print the current node
        printSorted(root->right); // Print right subtree
    }
}
