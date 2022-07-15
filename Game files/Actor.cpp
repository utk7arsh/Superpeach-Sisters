#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Flag::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		setDead();
		getworld()->increaseScore(1000);
		getworld()->level_over();
	}
}

void Mario::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		setDead();
		getworld()->increaseScore(1000);
		getworld()->game_over();
	}
}

//-----------------------------------------------------------------------------------------------------
void movingInteraction::switch_dir() {
	if (getDirection() == 0) {
		setDirection(180);
	}
	else
		setDirection(0);
}

void movingInteraction::motion() {
	if (getDirection() == 0 && getworld()->isBlockingAt(getX() + 2, getY()))
		switch_dir();
	if (getDirection() == 0 && !(getworld()->isBlockingAt(getX() + 2, getY())))
		moveTo(getX() + 2, getY());
	if (getDirection() == 180 && getworld()->isBlockingAt(getX() - 2, getY())) {
		switch_dir();
	}
	if (getDirection() == 180 && !(getworld()->isBlockingAt(getX() - 2, getY()))) {
		moveTo(getX() - 2, getY());
	}
	if (!getworld()->isBlockingAt(getX(), getY() - 2) && getDirection() == 0 && getworld()->isBlockingAt(getX() + 2, getY())) {
		moveTo(getX(), getY() - 2);
		switch_dir();
	}
	if (!getworld()->isBlockingAt(getX(), getY() - 2) && getDirection() == 180 && getworld()->isBlockingAt(getX() - 2, getY())) {
		moveTo(getX(), getY() - 2);
		switch_dir();
	}
	if (!getworld()->isBlockingAt(getX(), getY() - 2)) {
		moveTo(getX(), getY() - 2);
	}
}

void movingInteraction::common_doSomething() {
	getworld()->playSound(SOUND_PLAYER_POWERUP);
}

void Flower::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		getworld()->increaseScore(50);
		common_doSomething();
		setDead();
		getworld()->p_yesShoot();
		getworld()->p_incH();
		return;
	}
	else
		motion();
}

void Mushroom::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		getworld()->increaseScore(75);
		common_doSomething();
		setDead();
		getworld()->p_yesJump();
		getworld()->p_incH();
		return;
	}
	else
		motion();
}

void Star::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		getworld()->increaseScore(100);
		common_doSomething();
		setDead();
		getworld()->p_setInvinc(150);
		getworld()->p_yesStar();
		return;
	}
	else
		motion();
}

void Fireball::f_motion() {
	if (!getworld()->isBlockingAt(getX(), getY() - 2)) {
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0 && getworld()->isBlockingAt(getX() + 2, getY()))
		setDead();
	if (getDirection() == 0 && !(getworld()->isBlockingAt(getX() + 2, getY())))
		moveTo(getX() + 2, getY());
	if (getDirection() == 180 && getworld()->isBlockingAt(getX() - 2, getY())) {
		setDead();
	}
	if (getDirection() == 180 && !(getworld()->isBlockingAt(getX() - 2, getY()))) {
		moveTo(getX() - 2, getY());
	}
}

void Shell::f_motion() {
	if (!getworld()->isBlockingAt(getX(), getY() - 2) && !(getworld()->isBlockingAt(getX() + 4, getY() - 2)) && !(getworld()->isBlockingAt(getX() - 4, getY() - 2))) {
			moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0 && getworld()->isBlockingAt(getX() + 2, getY()))
			setDead();
	if (getDirection() == 0 && !(getworld()->isBlockingAt(getX() + 2, getY())))
			moveTo(getX() + 2, getY());
	if (getDirection() == 180 && getworld()->isBlockingAt(getX() - 2, getY())) {
		setDead();
	}
	if (getDirection() == 180 && !(getworld()->isBlockingAt(getX() - 2, getY()))) {
		moveTo(getX() - 2, getY());
	}
}
void Peach_ball::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapVillain(this)) {
		getworld()->killVillain(this);
		setDead();
	}
	else
		f_motion();
}

void Piranha_ball::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		getworld()->p_villain_bonk();
		setDead();
		return;
	}
	else
		f_motion();
}

void Shell::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (getworld()->overlapVillain(this)) {
		getworld()->killVillain(this);
		setDead();
	}
	else
		f_motion();
}

//----------------------------------------------------------------------------------------------------

void Peach::doSomething() {
	//1
	if (!isAlive()) {
		return;
	}
	//2,3,4
	if (invinciblity > 0) {		//invinciblity is the number of ticks left
		invinciblity--;
	}
	if (invinciblity == 0) {
		hasstar = false;
	}
	if (temp_invinc > 0) {
		temp_invinc--;
	}
	if (time_to_recharge_before_next_fire > 0) {
		time_to_recharge_before_next_fire--;
	}
	//5
	if (getworld()->isBlockingAt(getX(), getY())) {
		block_bonk();				
	}
	if (getworld()->overlapVillain(this)) {
		villain_bonk();
	}

	if (remaining_jump_distance > 0) {
		if (getworld()->isBlockingAt(getX(), getY() + 4)) {
			block_bonk();
			remaining_jump_distance = 0;
		}
		else {
			moveTo(getX(), getY() + 4);
			remaining_jump_distance--;
		}
	}
	//7
	if (remaining_jump_distance == 0) {
		bool val = true;
		for (int i = 0; i < 4; i++) {
			if (getworld()->isBlockingAt(getX(), getY() - i)) {
				val = false;
			}
		}
		if (val != 0) {
			moveTo(getX(), getY() - 4);
		}
	}
	//8
	int input;
	if (getworld()->getKey(input)) {
		switch (input) {
		case KEY_PRESS_RIGHT:
			setDirection(0);
			if (!getworld()->isBlockingAt(getX() + 4, getY())) {
				moveTo(getX() + 4, getY());
			}
			else
				block_bonk();	
			break;
		case KEY_PRESS_LEFT:
			setDirection(180);
			if (!getworld()->isBlockingAt(getX() - 4, getY())) {
				moveTo(getX() - 4, getY());
			}
			else
				block_bonk();
			break;
		case KEY_PRESS_UP:
			if (getworld()->isBlockingAt(getX(), getY() - 1)) {
				if (jumpPower)
					remaining_jump_distance = 12;
				else
					remaining_jump_distance = 8;
				getworld()->playSound(SOUND_PLAYER_JUMP);
			}
			break;
		case KEY_PRESS_SPACE:
			if (canShoot() && time_to_recharge_before_next_fire == 0) {
				getworld()->makepFire();
				getworld()->playSound(SOUND_PLAYER_FIRE);
				time_to_recharge_before_next_fire = 8;
			}
			break;
		default:
			break;
		}
	}
	return;
}
//--------------------------------------------------------------------------------------
void Peach::block_bonk() {			
	getworld()->playSound(SOUND_PLAYER_BONK);
	getworld()->checkGoodie(getX(), getY() + 8);
	getworld()->checkGoodie(getX() + 4, getY() + 8);
	getworld()->checkGoodie(getX() - 4, getY() + 8);
	if (getDirection() == 0) {
		getworld()->checkGoodie(getX() + 8, getY());
	}
	else
		getworld()->checkGoodie(getX() - 8, getY());
}

void Peach::villain_bonk() {
	if (istempinv()) {
		return;
	}
	if (isinvincible()) {
		getworld()->killVillain();
		getworld()->playSound(SOUND_PLAYER_KICK);
		return;
	}
	hitpoint--;
	temp_invinc = 10;
	jumpPower = false;
	shootPower = false;
	if (hitpoint == 1) {
		getworld()->playSound(SOUND_PLAYER_HURT);
		return;
	}
	if (hitpoint == 0) {
		setDead();
		getworld()->playSound(SOUND_PLAYER_DIE);
	}
}
//-----------------------------------------------------------------------------------------
void Goomba::switch_dir() {
	if (getDirection() == 0) {
		setDirection(180);
	}
	else
		setDirection(0);
}
void Goomba::motion() {
	if (getDirection() == 0 && getworld()->isBlockingAt(getX() + 1, getY()))
		switch_dir();
	else if (getDirection() == 0 && !getworld()->isBlockingAt(getX() + 8, getY() - 1))
		switch_dir();
	else if (getDirection() == 0 && !(getworld()->isBlockingAt(getX() + 1, getY())))
		moveTo(getX() + 1, getY());
	else if (getDirection() == 180 && getworld()->isBlockingAt(getX() - 1, getY())) {
		switch_dir();
	}
	else if (getDirection() == 180 && !getworld()->isBlockingAt(getX() - 8, getY() - 1))
		switch_dir();
	else if (getDirection() == 180 && !(getworld()->isBlockingAt(getX() - 1, getY()))) {
		moveTo(getX() - 1, getY());
	}
}
void Goomba::doSomething() {
	if (!isAlive())
		return;
	if (getworld()->overlapsPeach(this)) {
		return;
	}
	else
		motion();
}

void Koopa::switch_dir() {
	if (getDirection() == 0) {
		setDirection(180);
	}
	else
		setDirection(0);
}
void Koopa::motion() {
	if (getDirection() == 0 && getworld()->isBlockingAt(getX() + 1, getY()))
		switch_dir();
	else if (getDirection() == 0 && !getworld()->isBlockingAt(getX() + 8, getY() - 1))
		switch_dir();
	else if (getDirection() == 0 && !(getworld()->isBlockingAt(getX() + 1, getY())))
		moveTo(getX() + 1, getY());
	else if (getDirection() == 180 && getworld()->isBlockingAt(getX() - 1, getY())) {
		switch_dir();
	}
	else if (getDirection() == 180 && !getworld()->isBlockingAt(getX() - 8, getY() - 1))
		switch_dir();
	else if (getDirection() == 180 && !(getworld()->isBlockingAt(getX() - 1, getY()))) {
		moveTo(getX() - 1, getY());
	}
}
void Koopa::doSomething() {
	if (!isAlive())
		return;

	if (getworld()->overlapsPeach(this)) {
		return;
	}
	else
		motion();
}

void Piranha::doSomething() {
	if (!isAlive())
		return;
	increaseAnimationNumber();
	if (getworld()->overlapsPeach(this)) {
		return;
	}
	bool check = false;
	for (int i = 0; i < 1.5 * SPRITE_HEIGHT; i++) {
		if (getworld()->PeachY() == getY() + i || getworld()->PeachY() == getY() - i)
			check = true;
	}

	if (!check)
		return;

	if (getX() > getworld()->PeachX()) {
		setDirection(180);
	}
	else
		setDirection(0);

	if (firing_delay > 0) {
		firing_delay--;
		return;
	}

	int diffx = getX() - getworld()->PeachX();
	if (abs(diffx) < 8 * SPRITE_WIDTH) {
		getworld()->makePiranhaFire(getX() / 8, getY() / 8, getDirection());
		getworld()->playSound(SOUND_PIRANHA_FIRE);
		firing_delay = 40;
	}
}