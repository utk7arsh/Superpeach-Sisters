#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	~StudentWorld();
	void level_over() { lvl_over = true; }
	void game_over() { g_over = true; }

	bool isBlockingAt(double x, double y);
	bool overlapsPeach(Actor* ac);
	int PeachY();
	int PeachX();
	void p_yesShoot();
	void p_yesJump();
	void p_yesStar();
	void p_setInvinc(int n);
	void p_incH();
	void p_villain_bonk();

	bool intersection(double x1, double y1, double x2, double y2);
	bool intersection(Actor* a, Actor* b);
	bool overlapVillain(Actor* ac);

	void checkGoodie(int x, int y);
	void makepFire();
	void makePiranhaFire(int x, int y, int dir);
	void makeShell(int x, int y, int dir);
	void killPeachball(int x, int y);
	void killPiranhaBall(int x, int y);
	void killPower(int x, int y);
	void killVillain();
	void killVillain(Actor* ac);
private:

	bool lvl_over;
	bool g_over;
	Peach* p;
	std::vector<Actor*> g_actor;
	std::vector<Actor*> temp;
};

#endif // STUDENTWORLD_H_