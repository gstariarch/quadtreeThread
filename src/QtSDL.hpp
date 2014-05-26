/*
 * QtSDL.hpp
 *
 *  Created on: 24/11/2012
 *      Author: bruno
 */

#ifndef QTSDL_HPP_
#define QTSDL_HPP_

#include "quadtree/Quadtree.hpp"
#include "mfw/timer/TimeEvaluate.hpp"
#include "mfw/Kernel.hpp"
#include "mfw/resource/Color.hpp"
#include "mfw/font/FontFW.hpp"
#include "QtSDLThread.hpp"
#include <time.h>

class QtSDL : public Quadtree, public Kernel {
	enum {MAX_THREAD = 4};
	enum CollisionMethod {BRUTE_FORCE, QUADTREE, QUADTREE_THREADS, MAX_METHODS};

	QtSDLThread *thread[MAX_THREAD];

//	Font *text;

	OGLFT::Monochrome *text;

	CollisionMethod collisionMethod;

	TimeEvaluate *timeQt;
	TimeEvaluate *timeQtThread;
	TimeEvaluate *timeBruteForce;

	bool showDivision;
	bool useQuadtree;
	bool moving;

	int initialSquares;	// quantidade inicial de quadrados

	void resetQuadtreeTimer();

	// Métodos
	bool loadResource();
	bool keyPressed(SDLKey keyPressed);
	bool gameLogic();
	bool draw(SDL_Surface *screen);

	bool randomBool();
	int randomNumber(int min, int max);
	void generateObjects(int n);

	void drawQtQuadrants(QuadtreeNode *node);

public:
	QtSDL(int w, int h);
	virtual ~QtSDL();
};

#endif /* QTSDL_HPP_ */
