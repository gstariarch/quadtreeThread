/*
 * OBB.hpp
 *
 *  Created on: 17/12/2012
 *      Author: bruno
 */

#ifndef OBB_HPP_
#define OBB_HPP_

#include <mfw/geometry/Box2D.hpp>
#include <mfw/geometry/Point.hpp>
#include <mfw/geometry/Vector2D.hpp>
#include <SDL/SDL.h>
#include <vector>

using std::vector;

class OBB {
	struct Projection {
		float minor;
		float major;
	};

	enum {SIZE = 4};

	PointF *p[SIZE];

	Projection projectOBB(Vector2D *v);

public:
	OBB(int x, int y, int width, int height);
	virtual ~OBB();

	PointF *getPoint(int index);
	PointF **getPoints();

	void facesNormal(vector<Vector2D *> *normal);
	bool isColliding(OBB *box);

	void rotate(float degrees, float transX, float transY);
	void translate(float x, float y);
	void translate(PointI point);
};

#endif /* OBB_HPP_ */
