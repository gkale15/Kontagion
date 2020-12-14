#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <cmath>
#include <sstream>



using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

StudentWorld::~StudentWorld() {
    if (ActorsinGame.empty())
        cleanUp();
}

Socrates* StudentWorld::getSoc(){
    return Socratesptr;
}

bool StudentWorld::overlapFood(Actor* c) {
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        double dist = distanceFrom(c->returnX(), c->returnY(), (*it)->returnX(), (*it)->returnY());
        if (dist <= SPRITE_WIDTH && (*it)->isedible()) {
            
            (*it)->setDead();
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::isblockedbyDirt(Actor* c, double& x, double& y, int amt) {
    c->getPositionInThisDirection(c->getDirection(), amt, x, y);
    if(distanceFrom(VIEW_WIDTH/2, VIEW_HEIGHT/2, x, y) >= VIEW_RADIUS)
        return true;
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        double dist = distanceFrom(x, y, (*it)->returnX(), (*it)->returnY());
        if (dist <= SPRITE_WIDTH/2 && (*it)->isDirt()) {
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::isFoodNearby(Actor* c, int& dir) {
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        double dist = distanceFrom(c->getX(), c->getY(), (*it)->returnX(), (*it)->returnY());
        if (dist <= 128 && (*it)->isedible()) {
            dir = atan2((*it)->getY() - c->getY(), (*it)->getX() - c->getX()) * (180/3.14);
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::projOverlap(Actor* c, int hp) {
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        double dist = distanceFrom(c->returnX(), c->returnY(), (*it)->returnX(), (*it)->returnY());
        if (dist <= SPRITE_WIDTH && (*it)->damageable() && (*it)->killbyproj() && (*it)->needtowin()) {
            (*it)->decrementHealth(hp);
            return true;
        }
        if (dist <= SPRITE_WIDTH && (*it)->damageable() && (*it)->killbyproj() == true) {
            (*it)->setDead();
            return true;
        }
        it++;
    }
    return false;
}

int StudentWorld::max(int x, int y){
    if(x>=y) return x;
    return y;
}

void StudentWorld::randRim(double& x, double& y) {
    const double PI = 4 * atan(1);
    int angle = randInt(0, 359);
    x = (VIEW_WIDTH/2 + VIEW_RADIUS * cos(angle*1.0 / 360 * 2 * PI));
    y = (VIEW_HEIGHT/2 + VIEW_RADIUS * sin(angle*1.0 / 360 * 2 * PI));
}

double StudentWorld::distanceFrom(double X1, double Y1, double X2, double Y2){
    double x = X2-X1;
    double y = Y2-Y1;
    double dist = pow(x, 2) + pow(y, 2);
    dist = sqrt(dist);
    return dist;
}

bool StudentWorld::overlapSoc(Actor* c) {
    if (distanceFrom(c->getX(), c->getY(), Socratesptr->returnX(), Socratesptr->returnY()) <= SPRITE_WIDTH) {
        return true;
    }
    return false;
}

bool StudentWorld::positionfree(double x, double y, bool dirtexempt) {
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        if (distanceFrom(x, y, (*it)->returnX(), (*it)->returnY()) <= SPRITE_WIDTH) {
            if (!dirtexempt) return false;
            if(dirtexempt && !(*it)->isDirt()) return false;
        }
        it++;
    }
    return true;
}

void StudentWorld::addActor(Actor* c) {
    ActorsinGame.push_back(c);
}

int StudentWorld::init()
{
    Socratesptr = new Socrates(this, 0, VIEW_HEIGHT/2);
    
    ostringstream os;
    os<<"Score: "<<getScore()<<" Level: "<<getLevel()<<" Lives: "<<getLives()<<" health: "<<Socratesptr->gethealth()<<" Sprays: "<<Socratesptr->getspray()<<" Flames: "<<Socratesptr->getflame();
    string s = os.str();
    setGameStatText(s);
    
    Actor* a;
    for (int i = 0; i < getLevel(); i++) {
        int randX = 3000, randY = 3000;
        
        while((distanceFrom(randX, randY, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120) || !positionfree(randX, randY, false)) {
            randX = randInt(0, 256);
            randY = randInt(0, 256);
        }
        a = new BacterialPits(this, randX, randY);
        ActorsinGame.push_back(a);
    }
    
    int numfood = 25;
    if(max(5*getLevel(), 25) == 25)
        numfood = 5*getLevel();
    for (int i = 0; i< numfood; i++) {
        int randX = 3000, randY = 3000;
        
        while((distanceFrom(randX, randY, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120) || !positionfree(randX, randY, false)) {
            randX = randInt(0, 256);
            randY = randInt(0, 256);
        }
        
        a = new Food(this, randX, randY);
        ActorsinGame.push_back(a);
    }
  
    for(int i = 0; i< max(20,180-getLevel()*20); i++) {
        int randX = 3000, randY = 3000;
        
        while(distanceFrom(randX, randY, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120 || (!positionfree(randX, randY, true))) {
            randX = randInt(0, 256);
            randY = randInt(0, 256);
        }
        
        a = new DirtPile(this, randX, randY);
        ActorsinGame.push_back(a);
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ostringstream os;
    os<<"Score: "<<getScore()<<" Level: "<<getLevel()<<" Lives: "<<getLives()<<" health: "<<Socratesptr->gethealth()<<" Sprays: "<<Socratesptr->getspray()<<" Flames: "<<Socratesptr->getflame();
    string s = os.str();
    setGameStatText(s);
    
    //make sure soc not dead
    if(Socratesptr->alive() == false) {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    else Socratesptr->doSomething();
    
    //give each a chance to do something
    vector<Actor*>::iterator it;
    it = ActorsinGame.begin();
    while(it != ActorsinGame.end()) {
        (*it)->doSomething();
        it++;
    }
    //rand chance of adding goodies
    Actor* a;
    int chanceF = max(510-getLevel()*10, 200);
    if(randInt(0, chanceF-1) == 0) {
        double x,y;
        randRim(x, y);
        a = new Fungi(this, x, y);
        ActorsinGame.push_back(a);
    }
    int chanceG = max(510-getLevel()*10, 200);
    if(randInt(0, chanceG-1) == 0) {
        int whichG = randInt(1, 10);
        double x,y;
        randRim(x, y);
        if(whichG == 1) {
            a = new ExtraLifeGoodies(this, x, y);
            ActorsinGame.push_back(a);
        }
        if(whichG <= 7) {
            a = new RestoreHealthGoodies(this, x, y);
            ActorsinGame.push_back(a);
        }
        if(whichG > 7) {
            a = new FlameThrowerGoodies(this, x, y);
            ActorsinGame.push_back(a);
        }
    }
    
    //remove dead bois
    for(it = ActorsinGame.begin(); it != ActorsinGame.end();) {
        if ((*it)->alive() == false) {
            delete *it;
            it = ActorsinGame.erase(it);
        }
        else it++;
    }
    
    bool lvlwon = true;
    for(it = ActorsinGame.begin(); it != ActorsinGame.end();it++) {
        if((*it)->needtowin()) lvlwon = false;
    }
    if (lvlwon) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete Socratesptr;
    Socratesptr = nullptr;
    vector<Actor*>::iterator it;
    for(it = ActorsinGame.begin(); it != ActorsinGame.end(); it++)
        delete *it;
    ActorsinGame.clear();
}

