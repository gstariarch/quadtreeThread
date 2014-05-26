/*
 * QuadtreeNode.hpp
 *
 *  Created on: 23/11/2012
 *      Author: bruno
 */

#ifndef QUADTREENODE_HPP_
#define QUADTREENODE_HPP_

#include "mfw/geometry/Box2D.hpp"
#include <vector>
#include <string>

using std::vector;
using std::string;

/*
 * 	  Quadrantes
 * 		4   1
 * 		3   2
 */
class QuadtreeNode {
	QuadtreeNode **nodes;	// Quadrantes

	Box2D area; 		// Posição e dimens�o do quadrante

	int level;
	vector<void *> *objectVector;

	static int capacity;
	static int max_level;

public:
	enum {SIZE = 4, MAX_CAPACITY = 200, MAX_LEVEL=20};

	QuadtreeNode(Box2D area, int level);
	virtual ~QuadtreeNode();

	bool split();

	bool isFull();
	bool hasChildren();

	bool insertObject(void *object);

	QuadtreeNode *getNode(int position);
	vector<void *> *getObjects();
	Box2D &getArea();

    static int getCapacity();
    static int getMaxLevel();

    static void changeCapacity(int cap);
    static void changeMaxLevel(int mlevel);

    string toString();
};

#endif /* QUADTREENODE_HPP_ */
