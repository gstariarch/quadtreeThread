/*
 * Polygon.hpp
 *
 *  Created on: 17/12/2012
 *      Author: bruno
 */

#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include "OBB.hpp"
#include <mfw/geometry/Point.hpp>
#include <vector>

using std::vector;

class Polygon {
	struct AABB {
		// Pontos da AABB usado envolta da OBB
		float x;
		float y;
		float width;
		float height;
	};

	static const float PI = 3.14159265;

	vector<PointF> vertex;
	PointF *centroid;

	OBB *obbox;		// Caixa orientada que envolve o objeto
	AABB *aabbox;	// AABB utilizada por fora da OBB, para reduzir cálculos de colisão da OBB

	bool collision;
	bool colAABB;
	float rotation;

	SDL_Color color;

	bool isCollidingAABB(AABB *box);

public:
	Polygon(int x, int y, int width, int height, PointF *centroid = NULL);
	virtual ~Polygon();

	OBB *getBoundingBox();

	bool isColliding(Polygon *polygon);

	AABB *getAABB();

	void setCollision(bool collision);
	void setColor(SDL_Color color);

	void addVertex(float x, float y);

	void updateAABB();

	void move(int speed);
	void rotate(float degrees);

	void draw(SDL_Surface *buffer, bool debug = false);
};

#endif /* POLYGON_HPP_ */
