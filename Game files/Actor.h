#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* w, int imageID, int start_x, int start_y, int startDirection, int depth) : GraphObject(imageID, SPRITE_WIDTH* start_x, SPRITE_HEIGHT* start_y, startDirection, depth), world(w),
		alive(true), good(true) {}
	virtual void doSomething() = 0;
	virtual bool BlocksMovement() { return false; }
	bool isAlive() { return alive; }
	void setDead() { alive = 0; }
	StudentWorld* getworld() { return world; }
	virtual bool isDamagable() { return false; }
	virtual bool isFlower() { return false; }
	virtual bool isMushroom() { return false; }
	virtual bool isStar() { return false; }
	virtual bool f_block() { return false; }
	virtual bool m_block() { return false; }
	virtual bool s_block() { return false; }
	virtual bool isMovingInteraction() { return false; }
	virtual bool isPeachFire() { return false; }
	virtual bool isPiranhaFire() { return false; }
	virtual bool hasGoodie() { return good; }
	virtual bool isShell() { return false; }
	virtual bool canShell() { return false; }
	void noMoreGood() { good = 0; }
private:
	StudentWorld* world;
	bool alive;
	bool good;
};



//-------------------------------------------------------------------------------------------------------------------------------------------
class BlocksMove : public Actor {
public:
	BlocksMove(StudentWorld* m_world, int ID, int lx, int ly) : Actor(m_world, ID, lx, ly, 0, 2) {}
	virtual void doSomething() { return; }
	virtual bool BlocksMovement() { return true; }
};
class Block : public BlocksMove {
public:
	Block(StudentWorld* m_world, int lx, int ly) : BlocksMove(m_world, IID_BLOCK, lx, ly) {}
};

class f_Block : public Block {
public:
	f_Block(StudentWorld* m_world, int lx, int ly) : Block(m_world, lx, ly) {}
	virtual bool f_block() { return true; }
};

class m_Block : public Block {
public:
	m_Block(StudentWorld* m_world, int lx, int ly) : Block(m_world, lx, ly) {}
	virtual bool m_block() { return true; }
};

class s_Block : public Block {
public:
	s_Block(StudentWorld* m_world, int lx, int ly) : Block(m_world, lx, ly) {}
	virtual bool s_block() { return true; }
};

class Pipes : public BlocksMove {
public:
	Pipes(StudentWorld* m_world, int lx, int ly) : BlocksMove(m_world, IID_PIPE, lx, ly) {}
};

//-------------------------------------------------------------------------------------------------------------------------------------------

class Interaction : public Actor {	//stores powerups and flags and firballs
public:
	Interaction(StudentWorld* m_world, int ID, int lx, int ly, int depth, int dir = right) : Actor(m_world, ID, lx, ly, dir, depth) {}
};

class Flag : public Interaction {
public:
	Flag(StudentWorld* m_world, int lx, int ly) : Interaction(m_world, IID_FLAG, lx, ly, 1) {}
	virtual void doSomething();
};

class Mario : public Interaction {
public:
	Mario(StudentWorld* m_world, int lx, int ly) : Interaction(m_world, IID_MARIO, lx, ly, 1) {}
	virtual void doSomething();
};

//---------------------------------------------------------------------------------------------------
class movingInteraction : public Interaction {
public:
	movingInteraction(StudentWorld* m_world, int ID, int lx, int ly) : Interaction(m_world, ID, lx, ly, 1) {}
	void motion();
	void switch_dir();
	void common_doSomething();
	virtual bool isMovingInteraction() { return true; }
	virtual bool hasGoodie() { return true; }
};

class Flower : public movingInteraction {
public:
	Flower(StudentWorld* m_world, int lx, int ly) : movingInteraction(m_world, IID_FLOWER, lx, ly) {}
	virtual void doSomething();
	virtual bool isFlower() { return true; }
};

class Mushroom : public movingInteraction {
public:
	Mushroom(StudentWorld* m_world, int lx, int ly) : movingInteraction(m_world, IID_MUSHROOM, lx, ly) {}
	virtual void doSomething();
	virtual bool isMushroom() { return true; }
};

class Star : public movingInteraction {
public:
	Star(StudentWorld* m_world, int lx, int ly) : movingInteraction(m_world, IID_STAR, lx, ly) {}
	virtual void doSomething();
	virtual bool isStar() { return true; }
};

//-----------------------------------------------------------------------------------------------------
class Fireball : public Interaction {
public:
	Fireball(StudentWorld* m_world, int ID, int lx, int ly, int dir) : Interaction(m_world, ID, lx, ly, 1, dir) {}
	virtual void f_motion();
};
class Piranha_ball : public Fireball {
public:
	Piranha_ball(StudentWorld* m_world, int lx, int ly, int dir) : Fireball(m_world, IID_PIRANHA_FIRE, lx, ly, dir) {}
	virtual void doSomething();
	virtual bool isPiranhaFire() { return true; }
};

class Peach_ball : public Fireball {
public:
	Peach_ball(StudentWorld* m_world, int lx, int ly, int dir) : Fireball(m_world, IID_PEACH_FIRE, lx, ly, dir) {}
	virtual void doSomething();
	virtual bool isPeachFire() { return true; }
};

class Shell : public Fireball {
public:
	Shell(StudentWorld* m_world, int lx, int ly, int dir) : Fireball(m_world, IID_SHELL, lx, ly, dir) {}
	virtual void doSomething();
	virtual bool isShell() { return true; }
	virtual void f_motion();

};
//--------------------------------------------------------------------------------------------------------

class Peach : public Actor {
public:
	Peach(StudentWorld* m_world, int lx, int ly) : Actor(m_world, IID_PEACH, lx, ly, right, 1),
		hitpoint(1), invinciblity(false), jumpPower(false), shootPower(false), hasstar(false), temp_invinc(0), time_to_recharge_before_next_fire(0), remaining_jump_distance(0) {}
	virtual void doSomething();
	void block_bonk();
	void villain_bonk();

	virtual bool isDamagable() { return true; }
	bool isinvincible() { return invinciblity > 0; }
	bool istempinv() { return temp_invinc > 0; }
	bool canShoot() { return shootPower; }
	void yesShoot() { shootPower = true; }
	bool canJump() { return jumpPower; }
	void yesJump() { jumpPower = true; }
	void setInvinciblity(int n) { invinciblity = n; }
	bool Starpower() { return hasstar; }
	void yesStar() { hasstar = true; }
	int howManyHits() { return hitpoint; }
	void incH() { hitpoint = 2; }

private:
	int hitpoint, remaining_jump_distance;
	int invinciblity, temp_invinc, time_to_recharge_before_next_fire;
	bool shootPower;
	bool jumpPower;
	bool hasstar;
};

//------------------------------------------------------------------------------------------------------------------------------------------
class Goomba : public Actor {
public:
	Goomba(StudentWorld* m_world, int lx, int ly) : Actor(m_world, IID_GOOMBA, lx, ly, randInt(0, 1) * 180, 0) {}
	virtual bool isDamagable() { return true; }
	virtual void motion();
	virtual void switch_dir();
	virtual void doSomething();
};

class Koopa : public Actor {
public:
	Koopa(StudentWorld* m_world, int lx, int ly) : Actor(m_world, IID_KOOPA, lx, ly, randInt(0, 1) * 180, 0) {}
	virtual bool isDamagable() { return true; }
	virtual void motion();
	virtual void switch_dir();
	virtual void doSomething();
	virtual bool canShell() { return true; }
};

class Piranha : public Actor {
public:
	Piranha(StudentWorld* m_world, int lx, int ly) : Actor(m_world, IID_PIRANHA, lx, ly, randInt(0, 1) * 180, 0), firing_delay(0) {}
	virtual bool isDamagable() { return true; }
	virtual void doSomething();
	int givefireDelay() { return firing_delay; }
private:
	int firing_delay;
};
#endif // ACTOR_H_