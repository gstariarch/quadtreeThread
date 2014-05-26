/*
 * OBB.cpp
 *
 *  Created on: 17/12/2012
 *      Author: bruno
 */

#include "OBB.hpp"
#include <cmath>

OBB::OBB(int x, int y, int width, int height) {
	p[0] = new PointF(x, y);
	p[1] = new PointF(x+width, y);
	p[2] = new PointF(x+width, y+height);
	p[3] = new PointF(x, y+height);
}

OBB::~OBB() {
	if ( p ) {
		delete [] p;
	}
}

PointF *OBB::getPoint(int index) {
	if ( index >= 0 && index < 4 ) {
		return p[index];
	}

	return NULL;
}

PointF **OBB::getPoints() {
	return p;
}

void OBB::facesNormal(vector<Vector2D *> *normal) {
	Vector2D *normalAux;

	int next;

	for (int i=0; i < SIZE; i++) {
		next = (i + 1) % SIZE;

		normalAux = new Vector2D(p[i], p[next]);
		normalAux->normal(normalAux);
		normalAux->versor(normalAux);

		normal->push_back(normalAux);
	}
}

bool OBB::isColliding(OBB *box) {
	vector<Vector2D *> normalVersor;

	// Versores normais a cada face das boundings boxes
	facesNormal(&normalVersor);
	box->facesNormal(&normalVersor);

	// Projeção de cada vértice
	Projection projA;
	Projection projB;

	for (unsigned int i=0; i < normalVersor.size(); i++) {
		projA = projectOBB(normalVersor[i]);
		projB = box->projectOBB(normalVersor[i]);

		if ( projA.minor < projB.minor ) {
			// A est� antes
			if ( projA.major < projB.minor ) {
				return false;
			}
		} else {
			// B est� antes

			if ( projB.major < projA.minor ) {
				return false;
			}
		}
	}

	return true;
}

void OBB::rotate(float degrees, float transX, float transY) {
	for (int i=0; i < SIZE; i++) {
		p[i]->translate(-transX, -transY);
		p[i]->rotation(degrees);
		p[i]->translate(transX, transY);
	}
}

void OBB::translate(float x, float y) {
	for (int i=0; i < SIZE; i++) {
		p[i]->addX(x);
		p[i]->addY(y);
	}
}

void OBB::translate(PointI point) {
	for (int i=0; i < SIZE; i++) {
		p[i]->addX(point.getX());
		p[i]->addY(point.getY());
	}
}

OBB::Projection OBB::projectOBB(Vector2D *v) {
	float projection;

	Projection proj;

	projection = v->scalarProduct(p[0]);

	proj.minor = projection;
	proj.major = projection;

	for (int i=1; i < SIZE; i++) {
		projection = v->scalarProduct(p[i]);

		if ( projection > proj.major )
			proj.major = projection;
		else
			if ( projection < proj.minor )
				proj.minor = projection;
	}

	return proj;
}



