#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = malloc(sizeof(list_t)); // Allocate memory for the list
    if (!list) return NULL; // Return NULL if memory allocation fails

    node_t *sentinel = malloc(sizeof(node_t)); // Allocate memory for the sentinel node
    if (!sentinel) {
        free(list); // Free the list if sentinel allocation fails
        return NULL;
    }

    sentinel->data = NULL; // Sentinel node does not hold data
    sentinel->next = sentinel; // Sentinel's next points to itself
    sentinel->prev = sentinel; // Sentinel's prev points to itself

    list->size = 0; // Initialize list size to 0
    list->head = sentinel; // Set sentinel as the head of the list
    list->destroy_data = destroy_data; // Set the destroy_data function
    list->compare_to = compare_to; // Set the compare_to function

    return list; // Return the initialized list
}

void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) return; // Return if list is NULL

    node_t *current = (*list)->head->next; // Start from the first element
    while (current != (*list)->head) { // Iterate through the list
        node_t *next = current->next; // Store the next node
        (*list)->destroy_data(current->data); // Destroy the data
        free(current); // Free the current node
        current = next; // Move to the next node
    }

    free((*list)->head); // Free the sentinel node
    free(*list); // Free the list structure
    *list = NULL; // Set the list pointer to NULL
}

list_t *list_add(list_t *list, void *data) {
    node_t *new_node = malloc(sizeof(node_t)); // Allocate memory for the new node
    if (new_node == NULL) return NULL; // Return NULL if allocation fails

    new_node->data = data; // Set the data for the new node
    new_node->next = list->head->next; // New node points to the first element
    new_node->prev = list->head; // New node's prev points to the sentinel
    list->head->next->prev = new_node; // First element's prev points to the new node
    list->head->next = new_node; // Sentinel's next points to the new node

    list->size++; // Increment the list size
    return list; // Return the list
}

void *list_remove_index(list_t *list, size_t index) {
    if (index >= list->size) return NULL; // Return NULL if index is out of bounds

    node_t *current = list->head->next; // Start from the first element
    for (size_t i = 0; i < index; i++) { // Iterate to the specified index
        current = current->next;
    }

    current->prev->next = current->next; // Bypass the current node
    current->next->prev = current->prev; // Bypass the current node
    void *data = current->data; // Store the data to return

    free(current); // Free the current node
    list->size--; // Decrement the list size
    return data; // Return the removed data
}

int list_indexof(list_t *list, void *data) {
    node_t *current = list->head->next; // Start from the first element
    size_t index = 0; // Initialize index

    while (current != list->head) { // Iterate through the list
        if (list->compare_to(current->data, data) == 0) { // Compare data
            return (int)index; // Return the index if data matches
        }
        current = current->next; // Move to the next node
        index++; // Increment the index
    }

    return -1; // Return -1 if data is not found
}

// NO changes were given from the code review given, one suggestion was to stay more consistent with the use of ! or using null, to keep it the same,
//but that would change the dynamic of my program. I'll keep that in mind next project I work on. 