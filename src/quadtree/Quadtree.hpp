/*
 * Quadtree.hpp
 *
 *  Created on: 23/11/2012
 *      Author: bruno
 */

#ifndef QUADTREE_HPP_
#define QUADTREE_HPP_

#include "mfw/geometry/Box2D.hpp"
#include "QuadtreeNode.hpp"

class Quadtree {
	QuadtreeNode *root;

	Box2D area;	// limites do quadrante

	void print(QuadtreeNode *node, int n = 0);
	void insert(void *node, QuadtreeNode *init);

	void calculateQuadtreeSize(QuadtreeNode *node, int &size);

protected:
	vector<void *> *objects;

	Quadtree(Box2D area);
	virtual ~Quadtree();

	void reallocate();

	/*
	 *	Testa se o objeto est� dentro do quadrante
	 */
	virtual bool isInsideQuadrant(void* object, Box2D quadrant);

	void printQuadtree();

	void clear();
	void clear2(QuadtreeNode *node);

	void generateQuadtree(vector<void *> *objects);

public:
	QuadtreeNode *getRoot();

	int getQuadtreeSize();
};

#endif /* QUADTREE_HPP_ */
