/*
 * Quadtree.cpp
 *
 *  Created on: 23/11/2012
 *      Author: bruno
 */

#include "Quadtree.hpp"
#include <iostream>

using std::cout;
using std::endl;

/*
 *	Inicializa a Quadtree com a �rea total do n� raiz
 */
Quadtree::Quadtree(Box2D area) {
	this->area = area;
	root = new QuadtreeNode(area, 0);

	objects = new vector<void *>;
}

Quadtree::~Quadtree() {
	clear();
}

/*
 * Se o objeto estiver dentro do quadrante
 */
bool Quadtree::isInsideQuadrant(void *object, Box2D quadrant) {
	Box2D *rect = (Box2D *)object;

	return rect->isCollision(quadrant);
}

void Quadtree::printQuadtree() {
	cout << "=== Tree ===" << endl << endl;
	print(root);
}

void Quadtree::print(QuadtreeNode *node, int n) {
	if ( !node )
		return;

	for (int i=0; i < n; i++)
		cout << "  ";

	cout << "Quadrant " << node->getArea().toString() << " - " << node->toString() << endl;

	if ( node->hasChildren() ) {
		QuadtreeNode *aux = 0;

		for (int i=0; i < QuadtreeNode::SIZE; i++) {
			aux = node->getNode(i);

			if ( aux ) {
				print(aux, n+1);
			}
		}
	}
}

/*
 * Insere um nó na Quadtree
 */
void Quadtree::insert(void *object, QuadtreeNode *node) {
	// Se pertencer ao quadrante
	if ( isInsideQuadrant(object, node->getArea()) ) {
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

/*
 * Gera a quadtree a partir de um vetor de objetos
 */
void Quadtree::generateQuadtree(vector<void*> *objects) {
	for (unsigned int i=0; i < objects->size(); i++) {
		insert(objects->at(i), root);
	}
}

QuadtreeNode *Quadtree::getRoot() {
	return root;
}

/*
 * Realoca o nó raiz
 */
void Quadtree::reallocate() {
	if ( !root )
		root = new QuadtreeNode(area, 0);
}

/*
 * Remove todos os n�s da Quadtree
 */
void Quadtree::clear() {
	if ( root ) {
		clear2(root);
		delete root;

		root = NULL;
	}
}

void Quadtree::clear2(QuadtreeNode *node) {
	if ( !node )
		return;

	// Se possuir filhos continua a itera��o
	if ( node->hasChildren() ) {
		if ( node->getObjects() ) {
			for (unsigned int i=0; i < node->getObjects()->size(); i++ ) {
				clear2(node->getNode(i));
				delete node->getNode(i);
			}
		}
	}
}

int Quadtree::getQuadtreeSize() {
	int n = 0;
	calculateQuadtreeSize(root, n);

	return n;
}

void Quadtree::calculateQuadtreeSize(QuadtreeNode *node, int &size) {
	if ( node ) {
		size++;

		calculateQuadtreeSize(node->getNode(0), size);
		calculateQuadtreeSize(node->getNode(1), size);
		calculateQuadtreeSize(node->getNode(2), size);
		calculateQuadtreeSize(node->getNode(3), size);
	}
}




