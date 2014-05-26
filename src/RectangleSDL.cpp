/*
 * RectangleSDL.cpp
 *
 *  Created on: 25/08/2013
 *      Author: bruno
 */

#include "RectangleSDL.hpp"
#include <SDL_gfx/SDL_gfxPrimitives.h>
#include "GL/gl.h"

RectangleSDL::RectangleSDL(Color::enColor color, int x, int y, int size, bool hor, bool vert, int speed)
																			: Box2D(x, y, size) {
	this->speed = speed;
	this->hor = hor;
	this->vert = vert;
	this->color = Color::getColor(color);
}

RectangleSDL::~RectangleSDL() {
}

void RectangleSDL::setColliding(bool value) {
	this->colliding = value;
}


bool RectangleSDL::isColliding() {
	return colliding;
}

void RectangleSDL::move(Box2D *limit) {
	colliding = false;

	if ( hor ) {
		if ( getRightSide() + speed >= limit->getRightSide() ) {
			hor = false;

			colliding = true;
		} else {
			addX(speed);
		}
	} else {
		if ( getLeftSide() - speed <= limit->getLeftSide() ) {
			hor = true;

			colliding = true;
		} else {
			addX(-speed);
		}
	}

	if ( vert ) {
		if ( getBottomSide() + speed >= limit->getBottomSide() ) {
			vert = false;
			colliding = true;
		} else {
			addY(speed);
		}
	} else {
		if ( getTopSide() - speed <= limit->getTopSide() ) {
			vert = true;
			colliding = true;
		} else {
			addY(-speed);
		}
	}
}

void RectangleSDL::draw(SDL_Surface *screen) {
	static GLfloat collisionColor[4] = {1.0, 0.0, 0.0, 0.7};
	static GLfloat normalColor[4] = {0.0, 0.0, 1.0, 0.7};

	glBegin(GL_QUADS);
		if ( colliding )
			glColor4fv(collisionColor);
		else
			glColor4fv(normalColor);

		glVertex2i(this->getLeftSide(), this->getTopSide());

		glVertex2i(this->getRightSide(), this->getTopSide());

		glVertex2i(this->getRightSide(), this->getBottomSide());

		glVertex2i(this->getLeftSide(), this->getBottomSide());
	glEnd();

	/*if ( colliding )
		boxRGBA(screen, this->getLeftSide(), this->getTopSide(), this->getRightSide(), this->getBottomSide(),
				255, 0, 0, 180);
	else
		boxRGBA(screen, this->getLeftSide(), this->getTopSide(), this->getRightSide(), this->getBottomSide(),
				color.r, color.g, color.b, 180);*/
}

void RectangleSDL::draw(Box2D & area, Color::enColor color, SDL_Surface *screen) {
	GLfloat c = 0.5;

	glBegin(GL_LINE_LOOP);
		glColor3f(c, c, c);

		glVertex2i(area.getLeftSide(), area.getTopSide());

		glVertex2i(area.getRightSide(), area.getTopSide());

		glVertex2i(area.getRightSide(), area.getBottomSide());

		glVertex2i(area.getLeftSide(), area.getBottomSide());
	glEnd();

	/*
	SDL_Color c = Color::getColor(color);

	boxRGBA(screen, area.getLeftSide(), area.getTopSide(), area.getRightSide(), area.getBottomSide(), c.r, c. g, c. b, 255);
	*/
}



