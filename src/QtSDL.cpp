/*
 * QtSDL.cpp
 *
 *  Created on: 24/11/2012
 *      Author: bruno
 */

#include "QtSDL.hpp"
#include <sstream>
#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

QtSDL::QtSDL(int w, int h) : Quadtree(Box2D(0, 120, w, h-120)), Kernel(40, w, h) {
	srand(time(NULL));

	text = NULL;

	timeQt = new TimeEvaluate();
	timeQtThread = new TimeEvaluate();
	timeBruteForce = new TimeEvaluate();

	this->setDisplay(Kernel::DISPLAY_FULLSCREEN);
	this->setScreen(Kernel::SCREEN_OPENGL);

	showDivision = true;
	useQuadtree = false;
	moving = true;

	initVideo();

	collisionMethod = BRUTE_FORCE;

	initialSquares = 40;
}

void QtSDL::resetQuadtreeTimer() {
	if ( collisionMethod == QUADTREE ) {
		timeQt->reset();
	} else if ( collisionMethod == QUADTREE_THREADS ) {
		timeQtThread->reset();
	}
}

QtSDL::~QtSDL() {
}

bool QtSDL::loadResource() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	generateObjects(initialSquares);

	text = new OGLFT::Monochrome("font/calibri.ttf", 12);

	text->setForegroundColor(0.0, 0.0, 0.0, 1.0);

	if ( text == NULL ) {
		std::cerr << "TEXT" << endl;
	}

	return true;
}

bool QtSDL::keyPressed(SDLKey keyPressed) {
	switch ( keyPressed ) {
		case SDLK_1:
		case SDLK_KP1:
			timeBruteForce->reset();
			collisionMethod = BRUTE_FORCE;

			break;
		case SDLK_2:
		case SDLK_KP2:
			timeQt->reset();
			collisionMethod = QUADTREE;

			break;
		case SDLK_3:
		case SDLK_KP3:
			timeQtThread->reset();
			collisionMethod = QUADTREE_THREADS;

			break;
		case SDLK_ESCAPE:
			quitGame();
			break;
		case SDLK_s:
			showDivision = !showDivision;
			break;
		case SDLK_m:
			moving = !moving;
			break;
//		case SDLK_RETURN:
//			fps->switchFps();
//			break;
		case SDLK_PLUS:
		case SDLK_KP_PLUS:
			if ( initialSquares < 1000000 ) {
				initialSquares *= 2;
				generateObjects(initialSquares);

				resetQuadtreeTimer();
			}

			break;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			if ( initialSquares >= 2 ) {
				initialSquares /= 2;
				generateObjects(initialSquares);

				resetQuadtreeTimer();
			}
			break;
		case SDLK_LEFT:
			QuadtreeNode::changeCapacity(-1);
			resetQuadtreeTimer();
			break;
		case SDLK_RIGHT:
			QuadtreeNode::changeCapacity(1);
			resetQuadtreeTimer();
			break;
		case SDLK_UP:
			QuadtreeNode::changeMaxLevel(1);
			resetQuadtreeTimer();
			break;
		case SDLK_DOWN:
			QuadtreeNode::changeMaxLevel(-1);
			resetQuadtreeTimer();
			break;
		default:
			break;
	}

	return true;
}

/*
 * Métodos para colisão OBB
 */

//bool Game::gameLogic() {
//	collisionTime.begin();
//
//	polygon->updateAABB();
//	polygon->setCollision(false);
//
//	for (unsigned int i=0; i < box->size(); i++) {
//		box->at(i)->updateAABB();
//
//		if ( polygon->isColliding(box->at(i)) ) {
//			polygon->setCollision(true);
//			box->at(i)->setCollision(true);
//		} else {
//			box->at(i)->setCollision(false);
//		}
//	}
//
//	collisionTime.end();
//
//	return true;
//}
//
//void Game::generateBox(int numberBoxes) {
//	int x;
//	int y;
//	int width;
//	int height;
//
//	float rotation;
//
//	Polygon *polygon;
//
//	int size = 16;
//
//	PointI res = getResolution();
//
//	for (int i=0; i < numberBoxes; i++) {
//		x = random(10, res.getX() - 80);
//		y = random(10, res.getY() - 80);
//		width = random(8, size);
//		height = random(8, size);
//
//		rotation = random(0, 170);
//
//		polygon = new Polygon(x, y, width, height);
//
//		polygon->addVertex(x, y);
//		polygon->addVertex(x+width, y);
//		polygon->addVertex(x+width, y+height);
//		polygon->addVertex(x, y+height);
//
//		polygon->setColor(Color::getColor(Color::Blue));
//		polygon->rotate(rotation);
//
//		box->push_back(polygon);
//	}
//}

bool QtSDL::gameLogic() {
	QuadtreeNode *rootNode = NULL;
	QuadtreeNode *nodeQuadrant;

	if ( objects ) {
		Box2D rootArea = getRoot()->getArea();
		int size = (int)objects->size();

		RectangleSDL *rect;

		// Move objetos
		if ( moving ) {
			for (int i=0; i < size; i++) {
				rect = (RectangleSDL *)objects->at(i);

				rect->move(&rootArea);
			}
		}

		switch ( collisionMethod ) {
			case BRUTE_FORCE:
				// Colisão Bruteforce
				timeBruteForce->begin();
				QtSDLThread::collisionBruteForce(objects);
				timeBruteForce->end();

				break;
			case QUADTREE:
				timeQt->begin();

				clear();
				reallocate();
				generateQuadtree(objects);
				QtSDLThread::collisionQuadtree(getRoot());

				timeQt->end();

				break;
			case QUADTREE_THREADS:
				timeQtThread->begin();

				clear();
				reallocate();

				rootNode = getRoot();

				// Cria 4 nós iniciais
				rootNode->split();

				// Criar árvore com threads
				if ( rootNode && rootNode->hasChildren() ) {
					for (int i=0; i < 4 && i < QuadtreeNode::SIZE; i++) {
						nodeQuadrant = rootNode->getNode(i);

						if ( nodeQuadrant )
							thread[i] = new QtSDLThread(nodeQuadrant, objects);
						else
							break;
					}

					if ( nodeQuadrant ) {
						for (int i=0; i < MAX_THREAD; i++)
							thread[i]->start();

						for (int i=0; i < MAX_THREAD; i++)
							thread[i]->join();

						for (int i=0; i < MAX_THREAD; i++) {
							delete thread[i];
							thread[i] = NULL;
						}
					}
				}

				timeQtThread->end();

				break;
			default:
				break;
		}
	}

	return true;
}

/*
 * Gera valor 0 ou 1
 */
bool QtSDL::randomBool() {
	return rand() % 2;
}


/*
 * Gera um número aleatório entre min e max
 */
int QtSDL::randomNumber(int min, int max) {
	return min + (rand() % (max-min+1));
}

/*
 * Gera n objetos aleatórios
 */
void QtSDL::generateObjects(int n) {
	int x, y, size;
	bool horizontal;
	bool vertical;
	int speed;

	objects->clear();

	Box2D limits = getRoot()->getArea();

	for (int i=0; i < n; i++) {
		x = randomNumber(limits.getLeftSide() + 5, limits.getRightSide() - 5);
		y = randomNumber(limits.getTopSide() + 5, limits.getBottomSide() - 5);

		size = randomNumber(2, 6);
		horizontal = randomBool();
		vertical = randomBool();
		speed = randomNumber(1, 2);

		objects->push_back(new RectangleSDL(Color::Blue, x, y, size, horizontal, vertical, speed));
	}
}

/*
 * Desenha quadrantes da quadtree
 */
void QtSDL::drawQtQuadrants(QuadtreeNode *node) {
	if ( node ) {
		RectangleSDL::draw(node->getArea(), Color::Gray, screen);

		if ( node->hasChildren() ) {
			for (int i=0; i < QuadtreeNode::SIZE; i++) {
				drawQtQuadrants(node->getNode(i));
			}
		}
	}
}

bool QtSDL::draw(SDL_Surface *screen) {
	ostringstream stream;

	RectangleSDL *rect;

//	 Desenha objetos
	if ( objects ) {
		for (int i=0; i < (int)objects->size(); i++) {
			rect = (RectangleSDL *)objects->at(i);

			rect->draw(screen);
		}
	}

	if ( showDivision && collisionMethod != BRUTE_FORCE )
		drawQtQuadrants(getRoot());

	stream << "Squares: " << initialSquares << ", MaxLevels: " << QuadtreeNode::getMaxLevel() <<
			", MaxCapacity: " << QuadtreeNode::getCapacity();

	stream << " | Commands - S: qt quadrants; 1,2,3: bruteforce / qt /qt threads; M: move/stop squares;";

	text->draw(20, 30, fps->toString().c_str());
	text->draw(20, 50, stream.str().c_str());
	stream.str("");

	if ( collisionMethod == BRUTE_FORCE )
		stream << ">> ";

	stream << "BruteForce=> " << timeBruteForce->toString();
	text->draw(20, 70, stream.str().c_str());
	stream.str("");

	if ( collisionMethod == QUADTREE )
		stream << ">> ";

	stream << "Quadtree=> " << timeQt->toString();
	text->draw(20, 90, stream.str().c_str());
	stream.str("");

	if ( collisionMethod == QUADTREE_THREADS )
		stream << ">> ";

	stream << "QuadtreeThreads=> " << timeQtThread->toString();
	text->draw(20, 110, stream.str().c_str());

	return true;
}



