/** @file LBinaryTree.cpp
 * @brief Concrete link listed based implementation of BinaryTree ADT.
 *
 * @author Derek Harter
 * @note   class: COSC 2336, Summer 2021
 * @note   ide  : VSCode Server 3.9.3, Gnu Development Tools
 * @note   assg : Assignment Binarytrees
 * @date   June 1, 2021
 *
 * Implement the Binarytree abstraction using a pointer based set of
 * nodes of dynamically allocated node items.  The dynamic node based
 * implementation creates new nodes dynamically when items are inserted
 * into the tree, and deletes these nodes when the key/value pair is deleted
 * from the tree.  We use a binary search tree.  We support nodes with
 * key/value pairs.  The binary tree is organized by key values.  The
 * general rule is that keys must have an order defined by operator<().
 * When a key/value pair is inserted in tree, it will go to the left subtree
 * of any existing node where the key is less than ordering, and will got
 * to the right subtree where the key is greater than ordering.  Because
 * of the tree structure, assuming the tree does not become too balanced,
 * all operations such as search, insertion and removal should be O(n log_2 n)
 * in performance.
 */
#include "LBinaryTree.hpp"
#include "BinaryTreeException.hpp"
#include "Employee.hpp"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

/**
 * @brief Default constructor
 *
 * Construct an initially empty binary tree.
 */
template<class Key, class Value>
LBinaryTree<Key, Value>::LBinaryTree()
{
  root = nullptr;
  BinaryTree<Key, Value>::size = 0;
}

/**
 * @brief Array based constructor
 *
 * Construct a tree from parallel lists of given keys and values.  We
 * simply reuse the insert method to iterate through and insert all of
 * the key/value pairs.  Key/value pairs are inserted into the tree in
 * the order they are passed in the arrays.  This constructor assumes
 * the arrays are parallel arrays, e.g. they are of the same size and
 * each indexed element corresponds to a key/value pair.
 *
 * @param size The size of the parallel input arrays of keys and values
 * @param keys An array of Key types to be inserted into newly constructed tree.
 * @param values An array of Value types to be inserted into newly
 *   constructed tree.
 */
template<class Key, class Value>
LBinaryTree<Key, Value>::LBinaryTree(int size, const Key keys[], const Value values[])
{
  // initialze the tree so it is initially empty
  root = nullptr;
  BinaryTree<Key, Value>::size = 0;

  // iterate through all of the key/value pairs in parallel arrays, inserting
  // them into the tree.
  for (int idx = 0; idx < size; idx++)
  {
    // need to uncomment this after implementing insert so that array
    // based constructor will now work
    insert(keys[idx], values[idx]);
  }
}

/**
 * @brief Class destructor
 *
 * Destroy the dynamically allocated nodes of this tree when the
 * instance object is going out of scope.  We (re)use the clear()
 * method here.
 */
template<class Key, class Value>
LBinaryTree<Key, Value>::~LBinaryTree()
{
  clear();
}

/**
 * @brief LBinaryTree to string
 *
 * Create a string representation of this tree.  This is the
 * public function, it calls the private recursive overloaded
 * str() to perform most of the work, though it does add in
 * the preamble and size of the tree.
 *
 * @returns string Returns the constructed string of the BinaryTree
 *   contents in ascending sorted order.
 */
template<class Key, class Value>
string LBinaryTree<Key, Value>::str() const
{
  ostringstream out;

  out << "<BinaryTree> size: " << BinaryTree<Key, Value>::size << " values: [ " << str(root) << "]";

  return out.str();
}

/**
 * @brief Clear the tree
 *
 * Clear the tree and return to an empty state.  Make sure we are
 * good managers of memory, and traverse the tree to delete all
 * dynamically allocated nodes currently in this tree.
 */
template<class Key, class Value>
void LBinaryTree<Key, Value>::clear()
{
  // actual work done in overloaded private clear() recursive function
  clear(root);

  // when we are done clearing, ensure tree is back to empty state
  root = nullptr;
  BinaryTree<Key, Value>::size = 0;
}

/**
 * @brief Private LBinaryTree to string
 *
 * This is the recursive private function that does the actual
 * work of creating a string representation of the LBinaryTree.
 * We perform a (recursive) inorder traversal, constructing a
 * string object, to be returned as a result of this function.
 *
 * @param node The BinaryTreeNode we are currently processing.
 *
 * @returns string Returns the constructed string of the BinaryTree
 *   contents in ascending sorted order.
 */
template<class Key, class Value>
string LBinaryTree<Key, Value>::str(BinaryTreeNode<Key, Value>* node) const
{
  // base case, if node is null, just return empty string, which
  // stops the recursing
  if (node == NULL)
  {
    return "";
  }
  // general case, do an inorder traversal and build string
  else
  {
    ostringstream out;

    // do an inorder traversal
    out << str(node->getLeft()) << node->getValue() << " " << str(node->getRight());

    return out.str();
  }
}

/**
 * @brief Private recursive clear tree
 *
 * Clear the tree and return to an empty state.  Make sure we are
 * good managers of memory, and traverse the tree to delete all
 * dynamically allocated nodes currently in this tree.
 *
 * This is the private recursive function that performs the actual
 * work of clearing the tree.  The public clear function simply calls
 * this function for the root of the tree to perform the clear operation
 * recursively.
 */
template<class Key, class Value>
void LBinaryTree<Key, Value>::clear(BinaryTreeNode<Key, Value>* node)
{
  // base case, when node is nullptr we are done clearing in this branch,
  // in which case we have nothing to doo
  if (node == nullptr)
  {
    return;
  }

  // otherwise first recursively clear our left and right subchildren
  clear(node->getLeft());
  clear(node->getRight());

  // now we can free up this node safely
  delete node;
}
/** @brief insert a new key / value pair into the binary tree
 *
 * Insert the given key and corresponding value into the tree data structure.
 * This is the public implementation of the tree insert method.
 *
 * @param key The key of the key/value pair to insert into this tree.
 * @param value The value of the key/value pair to insert into this tree.
 *
 */
template<class Key, class Value>
void LBinaryTree<Key, Value>::insert(const Key& key, const Value& value)
{
  root = insert(root, key, value);
}

/** @brief insert a new key / value pair into the binary tree
 *
 * Insert the given key and corresponding value into the tree data structure.
 * This is the private implementation of the tree insert method. It recursively
 * searches the tree to find the correct location to insert.
 *
 * @param key The key of the key/value pair to insert into this tree.
 * @param value The value of the key/value pair to insert into this tree.
 *
 * @return BinaryTreeNode* Returns the node we are called with/ the new
 * node we create.
 */
template<class Key, class Value>
BinaryTreeNode<Key, Value>* LBinaryTree<Key, Value>::insert(BinaryTreeNode<Key, Value>* node, const Key& key, const Value& value)
{
  if (node == nullptr)
  {
    BinaryTreeNode<Key, Value>* newNode = new BinaryTreeNode<Key, Value>(key, value);
    BinaryTree<Key, Value>::size += 1;
    return newNode;
  }
  else
  {

    BinaryTreeNode<Key, Value>* returnedNode;
    if (key <= node->getKey())
    {
      returnedNode = insert(node->getLeft(), key, value);
      node->setLeft(returnedNode);
    }
    else
    {
      returnedNode = insert(node->getRight(), key, value);
      node->setRight(returnedNode);
    }
    return node;
  }
}

/** @brief Search of the binary tree container.
 *
 * Find the given key in the tree data structure.
 * This is the public implementation of the tree find method.
 *
 * @param key The key that is being searched.
 *
 */
template<class Key, class Value>
Value LBinaryTree<Key, Value>::find(const Key& key) const
{
  return find(root, key);
}

/** @brief Search of the binary tree container.
 *
 * Find the given key in the tree data structure.
 * This is the private implementation of the tree find method. It recursively
 * searches the tree to find the correct location that the key is at.
 *
 * @param key The key that is being searched.
 * @param node The Node that is being searched.
 *
 * @return returnedVal Returns the location of the key.
 */
template<class Key, class Value>
Value LBinaryTree<Key, Value>::find(BinaryTreeNode<Key, Value>* node, const Key& key) const
{
  if (node == nullptr)
  {
    throw BinaryTreeKeyNotFoundException("The key you were expecting to find was not in the tree.");
  }
  else
  {
    if (node->getKey() == key)
    {
      return node->getValue();
    }
    else
    {
      Value returnedVal;
      if (key < node->getKey())
      {
        returnedVal = find(node->getLeft(), key);
        return returnedVal;
      }
      else
      {
        returnedVal = find(node->getRight(), key);
        cout << returnedVal;
        return returnedVal;
      }
    }
  }
}
/** @brief Gets minimum node in tree.
 *
 * This function searches for and returns a pointer to the node with the minimum
 * value from some beginning subtree/root node of the tree.
 *
 * @param node Pointer to node to be searched.
 *
 * @return getMinimum() the minimum node in the tree.
 */
template<class Key, class Value>
BinaryTreeNode<Key, Value>* LBinaryTree<Key, Value>::getMinimum(BinaryTreeNode<Key, Value>* node)
{
  if (node->getLeft() == nullptr)
  {
    return node;
  }
  return getMinimum(node->getLeft());
}
/** @brief Deletes minimum node in tree.
 *
 * This function deletes and returns a pointer to the node with the minimum
 * value from some beginning subtree/root node of the tree.
 *
 * @param node Pointer to node to be deleted.
 *
 * @return node the minimum node in the tree.
 */
template<class Key, class Value>
BinaryTreeNode<Key, Value>* LBinaryTree<Key, Value>::deleteMinimum(BinaryTreeNode<Key, Value>* node)
{
  if (node->getLeft() == nullptr)
  {
    return node->getRight();
  }
  BinaryTreeNode<Key, Value>* newNode = deleteMinimum(node->getLeft());
  node->setLeft(newNode);
  return node;
}

/** @brief Removes a node in tree.
 *
 * Removes a node with the value of "key" in the tree data structure.
 * This is the public implementation of the tree remove method. It recursively
 * searches the tree to find the correct node to remove.
 *
 * @param key value to be removed.
 *
 * @return node The tree with the node removed.
 */
template<class Key, class Value>
Value LBinaryTree<Key, Value>::remove(const Key& key)
{
  Value node = find(root, key);
  root = remove(root, key);
  this->size -= 1;
  return node;
}

/** @brief Removes a node in tree.
 *
 * Removes a node with the value of "key" in the tree data structure.
 * This is the private implementation of the tree remove method. It recursively
 * searches the tree to find the correct node to remove.
 *
 * @param node node to be deleted.
 * @param key value to be removed.
 *
 * @return node The tree with the node removed.
 */
template<class Key, class Value>
BinaryTreeNode<Key, Value>* LBinaryTree<Key, Value>::remove(BinaryTreeNode<Key, Value>* node, const Key& key)
{
  if (node == nullptr)
  {
    throw BinaryTreeKeyNotFoundException("The key you were expecting to find was not in the tree.");
  }
  if (key < node->getKey())
  {
    node->setLeft(remove(node->getLeft(), key));
  }
  else if (key > node->getKey())
  {
    node->setRight(remove(node->getRight(), key));
  }
  else
  {
    BinaryTreeNode<Key, Value>* temp = node;
    BinaryTreeNode<Key, Value>* temptwo;
    if (node->getLeft() == NULL)
    {
      node = node->getRight();
      delete temp;
    }
    else if (node->getRight() == NULL)
    {
      node = node->getLeft();
      delete temp;
    }
    else if (node->getRight() != NULL && node->getLeft() != NULL)
    {
      temptwo = getMinimum(node->getRight());
      node->setValue(temptwo->getValue());
      node->setKey(temptwo->getKey());
      node->setRight(deleteMinimum(node->getRight()));
      delete temptwo;
    }
  }
  return node;
}

/**
 * @brief Cause specific instance compilations
 *
 * This is a bit of a kludge, but we can use normal make dependencies
 * and separate compilation by declaring template class
 * LBinaryTree<needed_key_type, needed_value_type> here of any types
 * we are going to be instantianting with the template.
 *
 * https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
 * https://isocpp.org/wiki/faq/templates#separate-template-class-defn-from-decl
 */
template class LBinaryTree<int, int>;
template class LBinaryTree<string, double>;
