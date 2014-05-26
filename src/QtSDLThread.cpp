/*
 * QtSDLThread.cpp
 *
 *  Created on: 24/11/2012
 *      Author: bruno
 */

#include "QtSDLThread.hpp"
#include "quadtree/Quadtree.hpp"
#include <iostream>

using std::cout;
using std::endl;

/*
 *
 */
QtSDLThread::QtSDLThread() {
	node = NULL;
}

/*
 * Colisão
 */
QtSDLThread::QtSDLThread(QuadtreeNode *node) {
	this->node = node;
	objects = 0;

	action = COLLISION;
}

/*
 * Movimento
 */
QtSDLThread::QtSDLThread(vector<void *> *objects, int first, int finish, Box2D *limit) {
	this->node = 0;
	this->objects = objects;

	this->first = first;
	this->last = finish;
	this->limit = limit;

	action = MOVE;
}

/*
 * Criação da árvore
 */
QtSDLThread::QtSDLThread(QuadtreeNode *root, vector<void *> *objects) {
	this->objects = objects;

	this->node = root;

	action = GENERATE_QUADTREE;
}

QtSDLThread::~QtSDLThread() {

}

/*
 * Gera a quadtree a partir de uma lista de objetos
 */
void QtSDLThread::generateQuadtree() {
	if ( node ) {
		for (unsigned int i=0; i < objects->size(); i++) {
			insert(objects->at(i), this->node);
		}
	} else {
		cout << "Problem" << endl;
	}
}

/*
 * Verifica se um objeto está dentro do quadrante
 */
bool QtSDLThread::isInsideQuadrant(void *object, Box2D quadrant) {
	Box2D *rect = (Box2D *)object;

	return rect->isCollision(quadrant);
}

/*
 * Insere um objeto na quadtree
 */
void QtSDLThread::insert(void *object, QuadtreeNode *node) {
	// Se pertencer ao quadrante
	if ( QtSDLThread::isInsideQuadrant(object, node->getArea()) ) {
		// Se não for um nó folha
		if ( node->hasChildren() ) {
			for (int i=0; i < QuadtreeNode::SIZE; i++) {
				insert(object, node->getNode(i));
			}
		} else {
			// Tenta dividir, se retornar false é porque
			// já foi alocado o nível máximo

			if ( !node->insertObject(object) ) {
				if ( node->split() ) {
					vector<void *> *list = node->getObjects();

					while ( !list->empty() ) {
						for (int i=0; i < QuadtreeNode::SIZE; i++) {
							insert(list->front(), node->getNode(i));
						}

						list->erase(list->begin());
					}
				}
			}
		}
	}
}

void *QtSDLThread::run() {
	switch ( action ) {
		case GENERATE_QUADTREE:
			generateQuadtree();
			collisionQuadtree(node);
			break;
		case MOVE:
			move();
			break;
		case COLLISION:
			collisionQuadtree(node);
			break;
		default:
			break;
	}

	return NULL;
}

void QtSDLThread::move() {
	RectangleSDL *rect;

	for (int i=first; i < last; i++) {
		rect = (RectangleSDL *)objects->at(i);
		rect->move(limit);
	}
}

void QtSDLThread::collisionQuadtree(QuadtreeNode *node) {
	if ( !node )
		return;

	if ( node->getObjects() )
		collisionBruteForce(node->getObjects());

	if ( node->hasChildren() ) {
		for (int i=0; i < QuadtreeNode::SIZE; i++)
			collisionQuadtree(node->getNode(i));
	}
}

void QtSDLThread::collisionBruteForce(vector<void *> *objectList) {
	int size = (int)objectList->size();

	RectangleSDL *r1, *r2;

	// Define todos os retângulos como não estando colidindo
	for (int i=0; i < size; i++) {
		r1 = (RectangleSDL *)objectList->at(i);

		r1->setColliding(false);
	}

	// Testa colisão entre a lista de objetos
	for (int i=0; i < size; i++) {
		r1 = (RectangleSDL *)objectList->at(i);

		for (int j=i+1; j < size; j++) {
			r2 = (RectangleSDL *)objectList->at(j);

			if ( r1->isCollision(*r2) ) {
				r1->setColliding(true);
				r2->setColliding(true);
			}
		}
	}
}
