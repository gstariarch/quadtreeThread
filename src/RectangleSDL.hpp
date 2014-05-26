/*
 * RectangleSDL.hpp
 *
 *  Created on: 25/08/2013
 *      Author: bruno
 */

#ifndef RECTANGLESDL_HPP_
#define RECTANGLESDL_HPP_

#include "mfw/geometry/Box2D.hpp"
#include "mfw/resource/Color.hpp"
#include <SDL/SDL.h>

class RectangleSDL : public Box2D {
	bool colliding;

	int speed;
	bool hor;
	bool vert;

	SDL_Color color;

public:
	RectangleSDL(Color::enColor color, int x, int y, int size, bool hor, bool vert, int speed);
	virtual ~RectangleSDL();

	void setColliding(bool value);
	bool isColliding();
	bool isColliding(RectangleSDL *r2);

	void move(Box2D *limit);

	void draw(SDL_Surface *screen);
	static void draw(Box2D &area, Color::enColor color, SDL_Surface *screen);
};

#endif
