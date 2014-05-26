/*
 * QtSdlThread.hpp
 *
 *  Created on: 24/11/2012
 *      Author: bruno
 */

#ifndef QTSDLTHREAD_HPP_
#define QTSDLTHREAD_HPP_

#include "mfw/thread/Thread.hpp"
#include "quadtree/QuadtreeNode.hpp"
#include "RectangleSDL.hpp"
#include <vector>

using std::vector;

class QtSDLThread : public Thread {
public:
	enum Action {MOVE, COLLISION, GENERATE_QUADTREE};

private:
	QuadtreeNode *node;
	vector<void *> *objects;

	int first;
	int last;

	Box2D *limit;

	Action action;

public:
	QtSDLThread();
	QtSDLThread(QuadtreeNode *node);
	QtSDLThread(vector<void *> *objects, int first, int last, Box2D *limit);
	QtSDLThread(QuadtreeNode *root, vector<void *> *objects);
	virtual ~QtSDLThread();

	void move();

	void generateQuadtree();

	static bool isInsideQuadrant(void *object, Box2D quadrant);
	void insert(void *object, QuadtreeNode *node);
	static void collisionQuadtree(QuadtreeNode *node);
	static void collisionBruteForce(vector<void *> *objectList);

	void *run();
};

#endif /* QTSDLTHREAD_HPP_ */
