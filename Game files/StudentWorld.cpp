#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    p = nullptr;
    g_actor.clear();
    lvl_over = false;
    g_over = false;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    string level_file = "level0" + to_string(getLevel()) + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find " << level_file << " data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << level_file << " is improperly formatted" << endl;
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        for (int x = 0; x < 32; x++) {
            for (int y = 0; y < 32; y++) {
                Level::GridEntry ge = lev.getContentsOf(x, y);

                switch (ge)
                {
                case Level::empty:
                    cout << "Location " << x << "," << y << " is empty" << endl;
                    break;
                case Level::koopa:
                    cout << "Location " << x << "," << y << " starts with a koopa" << endl;
                    g_actor.push_back(new Koopa(this, x, y));
                    break;
                case Level::goomba:
                    cout << "Location " << x << "," << y << " starts with a goomba" << endl;
                    g_actor.push_back(new Goomba(this, x, y));
                    break;
                case Level::piranha:
                    cout << "Location " << x << "," << y << " starts with a piranha" << endl;
                    g_actor.push_back(new Piranha(this, x, y));
                    break;
                case Level::peach:
                    cout << "Location " << x << "," << y << " is where Peach starts" << endl;
                    p = new Peach(this, x, y);
                    break;
                case Level::flag:
                    cout << "Location " << x << "," << y << " is where a flag is" << endl;
                    g_actor.push_back(new Flag(this, x, y));
                    break;
                case Level::block:
                    cout << "Location " << x << "," << y << " holds a regular block" << endl;
                    g_actor.push_back(new Block(this, x, y));
                    break;
                case Level::pipe:
                    cout << "Location " << x << "," << y << " holds a pipe" << endl;
                    g_actor.push_back(new Pipes(this, x, y));
                    break;
                case Level::star_goodie_block:
                    g_actor.push_back(new s_Block(this, x, y));
                    cout << "Location " << x << "," << y << " has a star goodie block" << endl;
                    break;
                case Level::flower_goodie_block:
                    cout << "Location " << x << "," << y << " has a flower goodie block" << endl;
                    g_actor.push_back(new f_Block(this, x, y));
                    cout << g_actor.back()->getX() << " " << g_actor.back()->getY() << endl;
                    break;
                case Level::mushroom_goodie_block:
                    cout << "Location " << x << "," << y << " has a mushroom goodie block" << endl;
                    g_actor.push_back(new m_Block(this, x, y));
                    break;
                case Level::mario:
                    cout << "Location " << x << "," << y << " has the MARIOOOOOOOOOOOOO" << endl;
                    g_actor.push_back(new Mario(this, x, y));
                    break;
                }
            }
        }
    }
    //g_actor.push_back(new Flag(this, 5, 10)); //DO NOT FORGET TO REMOVE THIS
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{    
    p->doSomething();               //peach moves first
    for (auto act = g_actor.begin(); act != g_actor.end(); act++) {    //for the rest of the actors
        if ((*act)->isAlive()) {
            (*act)->doSomething();

            if (!p->isAlive()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }

            if (lvl_over) {
                playSound(SOUND_FINISHED_LEVEL);
                lvl_over = false;
                return GWSTATUS_FINISHED_LEVEL;
            }
            if (g_over) {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
        }
    }

    for (auto act = g_actor.begin(); act != g_actor.end(); act++) {
        if (!(*act)->isAlive()) {
            delete* act;
            g_actor.erase(act);
            act = g_actor.begin();
        }
    }
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();

    ostringstream oss;
    oss.fill('0');
    oss << "Lives: ";
    oss << getLives() << " ";
    oss << "Level: ";
    oss << setw(2) << getLevel() << " ";
    oss << "Score: ";
    oss << setw(6) << getScore() << " ";
    if (p->isinvincible()) {
        oss << "StarPower! ";
    }
    if (p->canShoot()) {
        oss << "ShootPower! ";
    }
    if (p->canJump()) {
        oss << "JumpPower! ";
    }
    setGameStatText(oss.str());

    //scoring system
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* act : g_actor)
        delete act;
    g_actor.clear();

    delete p;
    p = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::isBlockingAt(double x, double y) {
    for (auto a : g_actor) {
        if (a->BlocksMovement()) {
            if (x + SPRITE_WIDTH > a->getX() && x < a->getX() + SPRITE_WIDTH) {
                if (y + SPRITE_HEIGHT > a->getY() && y < a->getY() + SPRITE_HEIGHT)
                    return true;
            }
        }
    }
    return false;
}

bool StudentWorld::intersection(double x1, double y1, double x2, double y2) {
    if (x1 + SPRITE_WIDTH > x2 && x1 < x2 + SPRITE_WIDTH) {
        if (y1 + SPRITE_HEIGHT > y2 && y1 < y2 + SPRITE_HEIGHT)
            return true;
    }
    return false;
}

bool StudentWorld::intersection(Actor* a, Actor* b) {
    return intersection(a->getX(), a->getY(), b->getX(), b->getY());
}


bool StudentWorld::overlapsPeach(Actor* ac) {
    if (p->isAlive() && intersection(ac, p))
        return true;
    return false;
}

int StudentWorld::PeachY() {
    return p->getY();
}

int StudentWorld::PeachX() {
    return p->getX();
}

void StudentWorld::p_yesJump() {
    p->yesJump();
}
void StudentWorld::p_yesShoot() {
    p->yesShoot();
}
void StudentWorld::p_yesStar() {
    p->yesStar();
}
void StudentWorld::p_setInvinc(int n) {
    p->setInvinciblity(n);
}
void StudentWorld::p_incH() {
    p->incH();
}

void StudentWorld::p_villain_bonk() {
    p->villain_bonk();
}

bool StudentWorld::overlapVillain(Actor* ac) {
    for (auto a : g_actor) {
        if (a != ac) {
            if (a->isAlive() && a->isDamagable() && intersection(ac, a)) {
                return true;
            }
        }
    }
    return false;
}


void StudentWorld::checkGoodie(int x, int y) {
    for (auto a : g_actor) {
        if (a->f_block()) {
            if (a->getX() == x && a->getY() == y) {
                if (a->hasGoodie()) {
                    g_actor.push_back(new Flower(this, a->getX() / 8, (a->getY() / 8) + 1));
                    playSound(SOUND_POWERUP_APPEARS);
                    a->noMoreGood();
                }
            }
        }
        else if (a->m_block()) {
            if (a->getX() == x && a->getY() == y) {
                if (a->hasGoodie()) {
                    g_actor.push_back(new Mushroom(this, a->getX() / 8, (a->getY() / 8) + 1));
                    playSound(SOUND_POWERUP_APPEARS);
                    a->noMoreGood();
                }
            }
        }
        else if (a->s_block()) {
            if (a->getX() == x && a->getY() == y) {
                if (a->hasGoodie()) {
                    g_actor.push_back(new Star(this, a->getX() / 8, (a->getY() / 8) + 1));
                    playSound(SOUND_POWERUP_APPEARS);
                    a->noMoreGood();
                }
            }
        }
    }
}

void StudentWorld::makepFire() {
    g_actor.push_back(new Peach_ball(this, p->getX() / 8, p->getY() / 8, p->getDirection()));
}

void StudentWorld::makeShell(int x, int y, int dir) {
    g_actor.push_back(new Shell(this, x, y, dir));
}

void StudentWorld::makePiranhaFire(int x, int y, int dir) {
    g_actor.push_back(new Piranha_ball(this, x, y, dir));
}

void StudentWorld::killPeachball(int x, int y) {
    for (auto a : g_actor) {
        if (a->isPeachFire() && a->getX() == x && a->getY() == y) {
            a->setDead();
        }
    }
}

void StudentWorld::killPiranhaBall(int x, int y) {
    for (auto a : g_actor) {
        if (a->isPiranhaFire() && a->isAlive() && a->getX() == x && a->getY() == y) {
            a->setDead();
        }
    }
}

void StudentWorld::killPower(int x, int y) {
    for (auto a : g_actor) {
        if (a->isMovingInteraction() && a->isAlive() && a->getX() == x && a->getY() == y) {
            a->setDead();
        }
    }
}

void StudentWorld::killVillain() {
    for (auto a : g_actor) {
        if (a->isDamagable() && a->isAlive() && intersection(a,p)) {
            if (a->canShell()) {
                a->setDead();
                increaseScore(100);
                makeShell(a->getX() / 8, a->getY()/8, a->getDirection());
            }
            else {
                a->setDead();
                increaseScore(100);
            }
        }
    }
}

void StudentWorld::killVillain(Actor* ac) {
    for (auto a : g_actor) {
        if (a->isDamagable() && a->isAlive() && intersection(a, ac)) {
            if (a->canShell()) {
                a->setDead();
                increaseScore(100);
                makeShell(a->getX() / 8, a->getY()/8, a->getDirection());
            }
            else {
                a->setDead();
                increaseScore(100);
            }
        }
    }
}