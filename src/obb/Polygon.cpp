/*
 * Polygon.cpp
 *
 *  Created on: 17/12/2012
 *      Author: bruno
 */

#include "Polygon.hpp"
#include <SDL_gfx/SDL_gfxPrimitives.h>
#include <mfw/resource/Color.hpp>
#include <iostream>

using std::cout;
using std::endl;

Polygon::Polygon(int x, int y, int width, int height, PointF *centroid) {
	obbox = new OBB(x, y, width, height);

	color = Color::getColor(Color::Gray);

	collision = false;

	if ( centroid )
		this->centroid = centroid;
	else
		this->centroid = new PointF(x + width/2, y + height/2);

	aabbox = new AABB;
}

Polygon::~Polygon() {
	if ( obbox )
		delete obbox;

	if ( aabbox )
		delete aabbox;
}

bool Polygon::isCollidingAABB(AABB *box) {
	if ( aabbox->x + aabbox->width < box->x )
		return false;

	if ( aabbox->x > box->x + box->width )
		return false;

	if ( aabbox->y + aabbox->height < box->y )
		return false;

	if ( aabbox->y > box->y + box->height )
		return false;

	return true;
}

Polygon::AABB *Polygon::getAABB() {
	return aabbox;
}

OBB *Polygon::getBoundingBox() {
	return obbox;
}

bool Polygon::isColliding(Polygon *polygon) {
//	if ( isCollidingAABB(polygon->getAABB()) ) {
//		this->colAABB = true;
		return obbox->isColliding(polygon->getBoundingBox());
//	}

//	return false;
}

void Polygon::addVertex(float x, float y) {
	vertex.push_back(PointF(x, y));
}

void Polygon::move(int speed) {
	Vector2D front(obbox->getPoint(0), obbox->getPoint(1));

	front.normal(&front);
	front.versor(&front);

	front.multiply(speed);

	obbox->translate(front.getX(), front.getY());
	centroid->translate(front.getX(), front.getY());

	for (unsigned int i=0; i < vertex.size(); i++) {
		vertex.at(i).addX(front.getX());
		vertex.at(i).addY(front.getY());
	}
}

void Polygon::rotate(float angle) {
	static float RAD2DEG = PI/180.0;

	rotation += angle;

	PointF inverse(-1, -1);

	float degree = angle * RAD2DEG;

	float cx = centroid->getX();
	float cy = centroid->getY();

	obbox->rotate(degree, cx, cy);

	for (unsigned int i=0; i < vertex.size(); i++) {
		vertex[i].translate(-cx, -cy);
		vertex[i].rotation(degree);
		vertex[i].translate(cx, cy);
	}
}

void Polygon::setCollision(bool collision) {
	this->collision = collision;
}

void Polygon::setColor(SDL_Color color) {
	this->color = color;
}

void Polygon::updateAABB() {
	PointF *aux = obbox->getPoint(0);

	colAABB = false;

	float x = aux->getX();
	float y = aux->getY();
	float x2 = aux->getX();
	float y2 = aux->getY();

	for (int i=1; i < 4; i++) {
		aux = obbox->getPoint(i);

		if ( aux->getX() < x )
			x = aux->getX();
		else
			if ( aux->getX() > x2 )
				x2 = aux->getX();

		if ( aux->getY() < y )
			y = aux->getY();
		else
			if ( aux->getY() > y2 )
				y2 = aux->getY();
	}

	aabbox->x = x;
	aabbox->y = y;
	aabbox->width = x2 - x;
	aabbox->height = y2 - y;
}

void Polygon::draw(SDL_Surface *buffer, bool debug) {
//	SDL_Color c, c2;
//	int size = vertex.size();

//	Sint16 vx[size];
//	Sint16 vy[size];

//	for (int i=0; i < size; i++) {
//		vx[i] = vertex[i].getX();
//		vy[i] = vertex[i].getY();
//	}

//	if ( collision )
//		c = Color::getColor(Color::Red);
//	else
//		c = color;

//	filledPolygonRGBA(buffer, vx, vy, size, c.r, c.g, c.b, 255);

//	if ( debug ) {
//		Sint16 boxX[4];
//		Sint16 boxY[4];

//		for (int i=0; i < 4; i++) {
//			boxX[i] = obbox->getPoint(i)->getX();
//			boxY[i] = obbox->getPoint(i)->getY();
//		}

//		if ( this->colAABB )
//			c2 = Color::getColor(Color::Yellow);
//		else
//			c2 = Color::getColor(Color::Cian);

//		circleRGBA(buffer, centroid->getX(), centroid->getY(), 4, 255, 255, 255, 255);
//		polygonRGBA(buffer, boxX, boxY, 4, 200, 200, 200, 255);
//		rectangleRGBA(buffer, aabbox->x, aabbox->y, aabbox->x + aabbox->width, aabbox->y + aabbox->height,
//					  c2.r, c2.g, c2.b, 255);
//	}
}

