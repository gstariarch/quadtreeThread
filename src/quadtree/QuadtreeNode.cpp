/*
 * QuadtreeNode.cpp
 *
 *  Created on: 23/11/2012
 *      Author: bruno
 */

#include "QuadtreeNode.hpp"
#include <sstream>
#include <iostream>

using std::ostringstream;
using std::cout;
using std::endl;

int QuadtreeNode::capacity = 30;
int QuadtreeNode::max_level = 4;

QuadtreeNode::QuadtreeNode(Box2D area, int level) {
	objectVector = NULL;
	nodes = NULL;

	this->level = level;
	this->area = area;
}

QuadtreeNode::~QuadtreeNode() {
	if ( objectVector ) {
		delete objectVector;
		objectVector = NULL;
	}

	if ( nodes ) {
		delete [] nodes;
		nodes = NULL;
	}
}

QuadtreeNode *QuadtreeNode::getNode(int position) {
	if ( position < 0 || position > 3 )
		return 0;

	return nodes ? nodes[position] : NULL;
}

/*
 * Divide um quadrante em 4 sub-quadrantes
 */
bool QuadtreeNode::split() {
	if ( level == max_level )
		return false;

	int x = area.getLeftSide();
	int y = area.getTopSide();
	int w = area.getWidth() / 2;
	int h = area.getHeight() / 2;

	int nextLevel = level+1;

	Box2D quadrant(0, 0, w, h);

	this->nodes = new QuadtreeNode*[SIZE];

	quadrant.setX(x+w);
	quadrant.setY(y);
	this->nodes[0] = new QuadtreeNode(quadrant, nextLevel);

	quadrant.setY(y+h);
	this->nodes[1] = new QuadtreeNode(quadrant, nextLevel);

	quadrant.setX(x);
	this->nodes[2] = new QuadtreeNode(quadrant, nextLevel);

	quadrant.setX(x);
	quadrant.setY(y);
	this->nodes[3] = new QuadtreeNode(quadrant, nextLevel);

	return true;
}

/*
 * Se for verdadeiro o n� possui n�s filhos, caso contr�rio �
 * um n� folha
 */
bool QuadtreeNode::hasChildren() {
	if ( nodes )
		return true;

	return false;
}

/*
 * Retorna o vetor de objetos do n�
 */
vector<void*> *QuadtreeNode::getObjects() {
	return objectVector;
}

/*
 * Se alcan�ou a capacidade m�xima de objetos
 */
bool QuadtreeNode::isFull() {
	if ( objectVector && (int)objectVector->size() >= QuadtreeNode::capacity ) {
		return true;
	}

	return false;
}

/*
 * Adiciona um objeto ao vetor do nó
 */
bool QuadtreeNode::insertObject(void *object) {
	if ( !objectVector )
		objectVector = new vector<void *>;

	objectVector->push_back(object);

	return !isFull();
}

Box2D &QuadtreeNode::getArea() {
	return area;
}

int QuadtreeNode::getCapacity() {
    return capacity;
}

int QuadtreeNode::getMaxLevel() {
    return max_level;
}

void QuadtreeNode::changeCapacity(int cap) {
    if ( capacity + cap > 0 && capacity + cap < MAX_CAPACITY ) {
    	capacity += cap;
    }
}

void QuadtreeNode::changeMaxLevel(int mlevel) {
	if ( max_level + mlevel >= 0 && max_level + mlevel < MAX_LEVEL )
    	max_level += mlevel;
}


string QuadtreeNode::toString() {
	ostringstream stream;

	Box2D *rect;

	int size = 0;

	if ( !objectVector || (size = (int)objectVector->size()) > 0 ) {
		stream << "";

		stream << "Obj.: ";

		for (int i=0; i < size; i++) {
			rect = (Box2D *)objectVector->at(i);
			stream << rect->toString();

			if ( i < size - 1 )
				stream << ", ";
		}
	} else {
		stream << "Empty";
	}

	return stream.str();
}
