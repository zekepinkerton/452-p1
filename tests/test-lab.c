#include "harness/unity.h"
#include "../src/lab.h"

static list_t *lst_ = NULL; // Global pointer to the list

static int *alloc_data(int i) {
  int *rval = (int *)malloc(sizeof(int)); // Allocate memory for an integer
  *rval = i; // Assign the value to the allocated memory
  return rval; // Return the pointer to the allocated memory
}

static void destroy_data(void *data) {
  free(data); // Free the allocated memory
}

static int compare_to(const void *a, const void *b) {
  int fst = *(int *)a; // Dereference and cast the first pointer to int
  int snd = *(int *)b; // Dereference and cast the second pointer to int
  return fst - snd; // Return the difference
}

static void populate_list(void) {
  for (int i = 0; i < 5; i++) {
    list_add(lst_, alloc_data(i)); // Add elements 0 to 4 to the list
  }
}

void setUp(void) {
  lst_ = list_init(destroy_data, compare_to); // Initialize the list with destroy and compare functions
}

void tearDown(void) {
  list_destroy(&lst_); // Destroy the list and free memory
}

void test_create_destroy(void) {
  list_t *lst = NULL;
  lst = list_init(destroy_data, compare_to); // Initialize a new list
  TEST_ASSERT_FALSE(lst == NULL); // Check if the list is not NULL
  TEST_ASSERT_FALSE(lst->head == NULL); // Check if the head is not NULL
  TEST_ASSERT_TRUE(lst->size == 0); // Check if the list size is 0
  TEST_ASSERT_TRUE(lst->head->data == NULL); // Check if the head data is NULL
  TEST_ASSERT_TRUE(lst->destroy_data == destroy_data); // Check if destroy function is set correctly
  TEST_ASSERT_TRUE(lst->compare_to == compare_to); // Check if compare function is set correctly
  TEST_ASSERT_FALSE(lst->head->next == NULL); // Check if the next pointer is not NULL
  TEST_ASSERT_FALSE(lst->head->prev == NULL); // Check if the prev pointer is not NULL
  TEST_ASSERT_TRUE(lst->head->next == lst->head->prev); // Check if next and prev point to the same node
  list_destroy(&lst); // Destroy the list
  TEST_ASSERT_TRUE(lst == NULL); // Check if the list is NULL after destruction
}

void test_add1(void) {
  list_add(lst_, alloc_data(1)); // Add element 1 to the list
  TEST_ASSERT_TRUE(lst_->size == 1); // Check if the list size is 1
  TEST_ASSERT_TRUE(lst_->head->next == lst_->head->prev); // Check if next and prev point to the same node
  TEST_ASSERT_FALSE(lst_->head == lst_->head->next); // Check if head is not equal to next
  TEST_ASSERT_FALSE(lst_->head == lst_->head->prev); // Check if head is not equal to prev
  TEST_ASSERT_TRUE(lst_->head->data == NULL); // Check if head data is NULL
  TEST_ASSERT_TRUE(*((int *)lst_->head->next->data) == 1); // Check if next node data is 1
  TEST_ASSERT_TRUE(*((int *)lst_->head->prev->data) == 1); // Check if prev node data is 1
}

void test_add2(void) {
  list_add(lst_, alloc_data(1)); // Add element 1 to the list
  TEST_ASSERT_TRUE(lst_->size == 1); // Check if the list size is 1
  list_add(lst_, alloc_data(2)); // Add element 2 to the list
  TEST_ASSERT_TRUE(lst_->size == 2); // Check if the list size is 2
  TEST_ASSERT_FALSE(lst_->head->next == lst_->head->prev); // Check if next and prev are not equal
  TEST_ASSERT_FALSE(lst_->head == lst_->head->next); // Check if head is not equal to next
  TEST_ASSERT_FALSE(lst_->head == lst_->head->prev); // Check if head is not equal to prev
  TEST_ASSERT_TRUE(lst_->head->data == NULL); // Check if head data is NULL
  TEST_ASSERT_TRUE(*((int *)lst_->head->next->data) == 2); // Check if next node data is 2
  TEST_ASSERT_TRUE(*((int *)lst_->head->prev->data) == 1); // Check if prev node data is 1
}

void test_removeIndex0(void) {
  populate_list(); // Populate the list with elements 0 to 4
  int *rval = (int *)list_remove_index(lst_, 0); // Remove the element at index 0
  TEST_ASSERT_TRUE(lst_->size == 4); // Check if the list size is 4
  TEST_ASSERT_TRUE(*rval == 4); // Check if the removed value is 4
  free(rval); // Free the removed value
  node_t *curr = lst_->head->next;
  for (int i = 3; i >= 0; i--) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i); // Check if the list is 3->2->1->0
    curr = curr->next;
  }
  curr = lst_->head->prev;
  for (int i = 0; i <= 3; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i); // Check if the list is 0->1->2->3
    curr = curr->prev;
  }
}

void test_removeIndex3(void) {
  populate_list(); // Populate the list with elements 0 to 4
  int *rval = (int *)list_remove_index(lst_, 3); // Remove the element at index 3
  TEST_ASSERT_TRUE(lst_->size == 4); // Check if the list size is 4
  TEST_ASSERT_TRUE(*rval == 1); // Check if the removed value is 1
  free(rval); // Free the removed value
  node_t *curr = lst_->head->next;
  for (int i = 3; i >= 1; i--) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1); // Check if the list is 4->3->2->0
    curr = curr->next;
  }
  TEST_ASSERT_TRUE(*((int *)curr->data) == 0); // Check the last element
  curr = curr->prev;
  for (int i = 1; i <= 3; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1); // Check if the list is 0->2->3->4
    curr = curr->prev;
  }
}

void test_removeIndex4(void) {
  populate_list(); // Populate the list with elements 0 to 4
  int *rval = (int *)list_remove_index(lst_, 4); // Remove the element at index 4
  TEST_ASSERT_TRUE(lst_->size == 4); // Check if the list size is 4
  TEST_ASSERT_TRUE(*rval == 0); // Check if the removed value is 0
  free(rval); // Free the removed value
  node_t *curr = lst_->head->next;
  for (int i = 3; i >= 0; i--) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1); // Check if the list is 4->3->2->1
    curr = curr->next;
  }
  curr = lst_->head->prev;
  for (int i = 0; i <= 3; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1); // Check if the list is 1->2->3->4
    curr = curr->prev;
  }
}

void test_invaidIndex(void) {
  populate_list(); // Populate the list with elements 0 to 4
  void *rval = list_remove_index(lst_, 666); // Attempt to remove an element at an invalid index
  TEST_ASSERT_TRUE(rval == NULL); // Check if the return value is NULL
  TEST_ASSERT_TRUE(lst_->size == 5); // Check if the list size is still 5
  node_t *curr = lst_->head->next;
  for (int i = 4; i >= 0; i--) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i); // Check if the list is 4->3->2->1->0
    curr = curr->next;
  }
  for (int i = 0; i >= 4; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i); // Check if the list is 0->1->2->3->4
    curr = curr->prev;
  }
}

void test_removeAll(void) {
  populate_list(); // Populate the list with elements 0 to 4
  for (int i = 4; i >= 0; i--) {
    int *rval = (int *)list_remove_index(lst_, 0); // Remove elements one by one
    TEST_ASSERT_TRUE(*rval == i); // Check if the removed value is correct
    free(rval); // Free the removed value
  }
  TEST_ASSERT_FALSE(lst_->head->next == NULL); // Check if the next pointer is not NULL
  TEST_ASSERT_FALSE(lst_->head->prev == NULL); // Check if the prev pointer is not NULL
  TEST_ASSERT_TRUE(lst_->head->next == lst_->head->prev); // Check if next and prev point to the same node
  TEST_ASSERT_TRUE(lst_->size == 0); // Check if the list size is 0
}

void test_indexOf0(void) {
  populate_list(); // Populate the list with elements 0 to 4
  void *data = lst_->head->next->data; // Get the data of the first element
  size_t idx = list_indexof(lst_, data); // Get the index of the data
  TEST_ASSERT_TRUE(idx == 0); // Check if the index is 0
}

void test_indexOf3(void) {
  populate_list(); // Populate the list with elements 0 to 4
  void *data = alloc_data(1); // Allocate data with value 1
  size_t idx = list_indexof(lst_, data); // Get the index of the data
  TEST_ASSERT_TRUE(idx == 3); // Check if the index is 3
  free(data); // Free the allocated data
}

void test_notInList(void) {
  populate_list(); // Populate the list with elements 0 to 4
  void *data = alloc_data(22); // Allocate data with value 22
  int idx = list_indexof(lst_, data); // Get the index of the data
  TEST_ASSERT_EQUAL_INT64(-1, idx); // Check if the index is -1 (not found)
  free(data); // Free the allocated data
}

int main(void) {
  UNITY_BEGIN(); // Begin Unity test framework
  RUN_TEST(test_create_destroy); // Run test_create_destroy
  RUN_TEST(test_add1); // Run test_add1
  RUN_TEST(test_add2); // Run test_add2
  RUN_TEST(test_removeIndex0); // Run test_removeIndex0
  RUN_TEST(test_removeIndex3); // Run test_removeIndex3
  RUN_TEST(test_removeIndex4); // Run test_removeIndex4
  RUN_TEST(test_invaidIndex); // Run test_invaidIndex
  RUN_TEST(test_removeAll); // Run test_removeAll
  RUN_TEST(test_indexOf0); // Run test_indexOf0
  RUN_TEST(test_indexOf3); // Run test_indexOf3
  RUN_TEST(test_notInList); // Run test_notInList
  return UNITY_END(); // End Unity test framework
}
