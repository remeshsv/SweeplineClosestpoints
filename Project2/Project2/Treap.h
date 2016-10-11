#ifndef TREAP_H
#define TREAP_H


#include <climits>
#include "UniformRandom.h"
#include "dsexceptions.h"
#include <iostream>
#include <utility>


using namespace std;


int countrlc = 0;
int countrrc = 0;
int x1, x2, y3, y2;
int maxsize=0;


// Treap class
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class Treap
{
public:
	int distance = 0;
	int size;

	Treap()
	{
		nullNode = new TreapNode;
		nullNode->left = nullNode->right = nullNode;
		nullNode->priority = INT_MAX;
		root = nullNode;
	}

	Treap(const Treap & rhs)
	{
		nullNode = new TreapNode;
		nullNode->left = nullNode->right = nullNode;
		nullNode->priority = INT_MAX;
		root = clone(rhs.root);
	}

	~Treap()
	{
		makeEmpty();
		delete nullNode;
	}


	Treap(Treap && rhs) : root{ rhs.root }, nullNode{ rhs.nullNode }
	{
		rhs.root = nullptr;
		rhs.nullNode = nullptr;
	}


	/**
	* Deep copy.
	*/
	Treap & operator=(const Treap & rhs)
	{
		Treap copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	/**
	* Move.
	*/
	Treap & operator=(Treap && rhs)
	{
		std::swap(root, rhs.root);
		std::swap(nullNode, rhs.nullNode);

		return *this;
	}

	const Comparable & findMin() const
	{
		if (isEmpty())
			throw UnderflowException{};

		TreapNode *ptr = root;
		while (ptr->left != nullNode)
			ptr = ptr->left;

		return ptr->element1;
	}

	const Comparable & findMax() const
	{
		if (isEmpty())
			throw UnderflowException{};

		TreapNode *ptr = root;
		while (ptr->right != nullNode)
			ptr = ptr->right;

		return ptr->element1;
	}

	bool contains(const Comparable & x) const
	{
		TreapNode *current = root;
		nullNode->element1 = x;

		for (; ; )
		{
			if (x < current->element1)
				current = current->left;
			else if (current->element1 < x)
				current = current->right;
			else
				return current != nullNode;
		}
	}

	bool isEmpty() const
	{
		return root == nullNode;
	}

	void sizeTree() 
	{
			size = 0;
			sizeTree(root);
			if (maxsize < size)
				maxsize = size;
		
	}

	void printTree() const
	{
		if (isEmpty())
			cout << "Empty tree" << endl;

		else
			printTree(root);
	}

	void makeEmpty()
	{
		makeEmpty(root);
	}

	void insert(const Comparable & x, const Comparable & y)
	{
		insert(x, y, root);
	}

	void insert(Comparable && x, Comparable && y)
	{
		insert(std::move(x), std::move(x), root);
	}

	void remove(const Comparable & x)
	{
		remove(x, root);
	}

	int calcDist(const Comparable & x, const Comparable & y, const Comparable & dist)
	{
		distance = dist;
		size = 0;
		calcDist(x, y, dist, root);
		if (maxsize < size)
			maxsize = size;
		return distance;

	}

private:
	struct TreapNode
	{
		Comparable element1;
                Comparable element2;
		TreapNode *left;
		TreapNode *right;
		int        priority;

		TreapNode() : left{ nullptr }, right{ nullptr }, priority{ INT_MAX } { }

		TreapNode(const Comparable & e, const Comparable & f, TreapNode *lt, TreapNode *rt, int pr)
			: element1{ e }, element2{ f }, left{ lt }, right{ rt }, priority{ pr }
		{ }

		TreapNode(Comparable && e, Comparable && f, TreapNode *lt, TreapNode *rt, int pr)
			: element1{ std::move(e) }, element2{ std::move(f) },left{ lt }, right{ rt }, priority{ pr }
		{ }
	};

	TreapNode *root;
	TreapNode *nullNode;
	UniformRandom randomNums;

	// Recursive routines
	/**
	* Internal method to insert into a subtree.
	* x is the item to insert.
	* t is the node that roots the tree.
	* Set the new root of the subtree.
	* (randomNums is a UniformRandom object that is a data member of Treap.)
	*/
	void insert(const Comparable & x, const Comparable & y, TreapNode* & t)
	{
		if (t == nullNode)
			t = new TreapNode{ x, y, nullNode, nullNode, randomNums.nextInt() };
		else if (x < t->element1)
		{
			insert(x, y, t->left);
			if (t->left->priority < t->priority)
				rotateWithLeftChild(t);
		}
		else if (t->element1 < x)
		{
			insert(x, y, t->right);
			if (t->right->priority < t->priority)
				rotateWithRightChild(t);
		}
		// else duplicate; do nothing
	}

	/**
	* Internal method to insert into a subtree.
	* x is the item to insert.
	* t is the node that roots the tree.
	* Set the new root of the subtree.
	* (randomNums is a UniformRandom object that is a data member of Treap.)
	*/
	void insert(Comparable && x, Comparable && y, TreapNode* & t)
	{
		if (t == nullNode)
			t = new TreapNode{ std::move(x), std::move(y), nullNode, nullNode, randomNums.nextInt() };
		else if (x < t->element1)
		{
			insert(std::move(x), t->left);
			if (t->left->priority < t->priority)
				rotateWithLeftChild(t);
		}
		else if (t->element1 < x)
		{
			insert(std::move(x), t->right);
			if (t->right->priority < t->priority)
				rotateWithRightChild(t);
		}
		// else duplicate; do nothing
	}

	/**
	* Internal method to remove from a subtree.
	* x is the item to remove.
	* t is the node that roots the tree.
	* Set the new root of the subtree.
	*/
	void remove(const Comparable & x, TreapNode * & t)
	{
		if (t != nullNode)
		{
			if (x < t->element1)
				remove(x, t->left);
			else if (t->element1 < x)
				remove(x, t->right);
			else
			{
				// Match found
				if (t->left->priority < t->right->priority)
					rotateWithLeftChild(t);
				else
					rotateWithRightChild(t);

				if (t != nullNode)      // Continue on down
					remove(x, t);
				else
				{
					delete t->left;
					t->left = nullNode;  // At a leaf
				}
			}
		}
	}

	void makeEmpty(TreapNode * & t)
	{
		if (t != nullNode)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullNode;
	}

	void sizeTree(TreapNode *t) 
	{
		if (t != nullNode)
		{
			sizeTree(t->left);
			size++;
			sizeTree(t->right);
		}
	}

	void printTree(TreapNode *t) const
	{
		if (t != nullNode)
		{
			printTree(t->left);
			cout << t->element1 << endl;
			printTree(t->right);
		}
	}

	/*calcDist() Implemented by Remesh S V*/
	void calcDist(const Comparable & x, const Comparable & y, const Comparable & dist, TreapNode * & t)
	{
		if (t != nullNode)
		{
			calcDist(x, y, dist, t->left);
			if (((x - t->element1)*(x - t->element1)) < dist) //Check if y-values are within limit
			{
				int newdist = ((x - t->element1)*(x - t->element1) + (y - t->element2)*(y - t->element2));
				if (newdist < dist)
				{
					distance = newdist;
					x1 = y;
					y3 = x;
					x2 = t->element2;
					y2 = t->element1;
				}
			}
			size++;
			calcDist(x, y, distance, t->right);
		}

	}


	// Rotations
	void rotateWithLeftChild(TreapNode * & k2)
	{
		TreapNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2 = k1;
		countrlc++;
	}

	void rotateWithRightChild(TreapNode * & k1)
	{
		TreapNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1 = k2;
		countrrc++;
	}

	TreapNode * clone(TreapNode * t) const
	{
		if (t == t->left)  // Cannot test against nullNode!!!
			return nullNode;
		else
			return new TreapNode{ t->element1, t->element2, clone(t->left), clone(t->right), t->priority };
	}
};

#endif
#pragma once
