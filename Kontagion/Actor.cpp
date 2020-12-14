#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//base actor class
Actor::Actor(StudentWorld* sw,int imageID, double startX, double startY, int dir, int depth):GraphObject(imageID, startX, startY,dir,depth,1.0)
{
    this->setDirection(dir);
    this->moveTo(startX, startY);
    m_alive = true;
    m_sw = sw;
}

bool Actor::alive(){
    return m_alive;
}

bool Actor::damageable(){
    return true;
}

Actor::~Actor(){
}

StudentWorld* Actor::returnWorld(){
    return m_sw;
}

double Actor::returnX(){
    return this->getX();
}
double Actor::returnY() {
    return this->getY();
}

void Actor::setDead() {
    m_alive = false;
}

bool Actor::isDirt() {
    return false;
}

bool Actor::killbyproj() {
    return true;
}

bool Actor::needtowin() {
    return false;
}

bool Actor::isedible() {
    return false;
}

void Actor::decrementHealth(int hp) {}
///////////////////////////////////////////////////////////////////////////////////////////////



//socrates class///////////////////////////////////////////////////////////////////////////////////////////////
Socrates::Socrates(StudentWorld* sw, double startX, double startY):Actor(sw, IID_PLAYER, startX,startY, 0, 0)
{
    m_health = 100;
    m_spraycharges = 20;
    m_flamecharges = 5;
}

Socrates::~Socrates() {
    
}

void Socrates::doSomething(){
    //do something
      int ch;
    if(!this->alive())
        return;
      if (this->returnWorld()->getKey(ch))
      {
      // user hit a key during this tick!
      switch (ch)
      {
      case KEY_PRESS_LEFT:
      //... move Socrates counterclockwise ...;
          {
              double newX, newY;
              GraphObject::moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2);
              GraphObject::getPositionInThisDirection(getDirection()+185, VIEW_RADIUS, newX, newY);
              GraphObject::moveTo(newX, newY);
              setDirection(getDirection()+5);
              break;
          }
      case KEY_PRESS_RIGHT:
      //... move Socrates clockwise...;
          {
              double newX, newY;
              GraphObject::moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2);
              GraphObject::getPositionInThisDirection(getDirection()+175, VIEW_RADIUS, newX, newY);
              GraphObject::moveTo(newX, newY);
              setDirection(getDirection()-5);
              break;
          }
      case KEY_PRESS_SPACE:
          {
      //... add spray in front of Socrates...;
              if (m_spraycharges <= 0) break;
              double x = 0, y = 0;
              getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);
              Actor* c = new SprayProjectiles(returnWorld(), x, y, getDirection());
              returnWorld()->addActor(c);
              m_spraycharges--;
              returnWorld()->playSound(SOUND_PLAYER_SPRAY);
              break;
          }
      // etc…
      case KEY_PRESS_ENTER:
          {
              if (m_flamecharges <= 0) break;
              double x = 0, y = 0;
              Actor *c;
              for (int i = 0; i <16; i++) {
                  getPositionInThisDirection(getDirection()+(22*i), SPRITE_WIDTH, x, y);
                  c = new FlameProjectiles(returnWorld(), x, y, getDirection()+(22*i));
                  returnWorld()->addActor(c);
              }
              m_flamecharges--;
              returnWorld()->playSound(SOUND_PLAYER_FIRE);
              break;
          }
      }
      }
    else if (getspray() < 20) m_spraycharges++;
}

void Socrates::decrementHealth(int hp){
    m_health-=hp;
    if (m_health > 0)
        returnWorld()->playSound(SOUND_PLAYER_HURT);
    if(m_health<=0) {
        this->setDead();
        returnWorld()->playSound(SOUND_PLAYER_DIE);
    }
}


void Socrates::addflamecharge(int f){
    m_flamecharges+=f;
}

void Socrates::fullhealth(){
    m_health = 100;
}

int Socrates::gethealth(){
    return m_health;
}

bool Socrates::killbyproj() {return false;}

int Socrates::getspray() {return m_spraycharges;}
int Socrates::getflame() {return m_flamecharges;}
///////////////////////////////////////////////////////////////////////////////////////////////



//dirtpile///////////////////////////////////////////////////////////////////////////////////////////////
DirtPile::DirtPile(StudentWorld* sw,double startX, double startY):Actor(sw, IID_DIRT, startX, startY, 0, 1)
{
    
}
DirtPile:: ~DirtPile(){
    
}
void DirtPile::doSomething(){}

bool DirtPile::isDirt() {
    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////




//projectiles///////////////////////////////////////////////////////////////////////////////////////////////
Projectiles::Projectiles(StudentWorld* sw, int imageID, double startX, double startY, int dir, int maxD, int dmg):Actor(sw, imageID, startX, startY, dir, 1)
{
    m_maxD = maxD;
    m_dmg = dmg;
}

Projectiles::~Projectiles() {}

void Projectiles::doSomething() {
    //check if overlap, if it is deal damage and set dead
    if (returnWorld()->projOverlap(this, m_dmg)) {
        setDead();
    }
    //otherwise move forward in direction with sprwid pixels
    double x,y;
    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);
    moveTo(x, y);
    decreaseDistance();
    
}

int Projectiles::getDistanceLeft() {
    return m_maxD;
}
void Projectiles::decreaseDistance() {
    m_maxD-=SPRITE_WIDTH;
    if((m_maxD) <= 0) setDead();
}

bool Projectiles::killbyproj() {return false;}

///////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////flame
FlameProjectiles::FlameProjectiles(StudentWorld* sw, double startX, double startY, int dir)
:Projectiles(sw, IID_FLAME, startX, startY, dir, 32,5)
{}
  
FlameProjectiles::~FlameProjectiles() {};
/////////////////////////////////////////////////

//spray/////////////////////////////
SprayProjectiles::SprayProjectiles(StudentWorld* sw, double startX, double startY, int dir)
:Projectiles(sw, IID_SPRAY, startX, startY, dir, 112, 2)
{}
    
SprayProjectiles::~SprayProjectiles() {};
///////////////////////////////////////////////



//goodies///////////////////////////////////////////////////////////////////////////////////////////////
Goodies::Goodies(StudentWorld* sw, int imageID, double startX, double startY):Actor(sw, imageID, startX, startY, 0, 1)
{
    m_maxT = sw->max((randInt(0, 300-10*sw->getLevel()-1)), 50);
}
Goodies:: ~Goodies(){}

int Goodies::getTime(){
    return m_maxT;
}
void::Goodies::tickTime() {
    m_maxT--;
    if (m_maxT <= 0)
        setDead();
}
///////////////////////////////////////////////////////////////////////////////////////////////


//health////////////////////
RestoreHealthGoodies::RestoreHealthGoodies(StudentWorld* sw,double startX, double startY):Goodies(sw, IID_RESTORE_HEALTH_GOODIE, startX, startY)
{}
RestoreHealthGoodies::~RestoreHealthGoodies(){}

void RestoreHealthGoodies::doSomething(){
    if(!alive()) return;
    if(returnWorld()->overlapSoc(this)) {
        returnWorld()->increaseScore(250);
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        returnWorld()->getSoc()->fullhealth();
        setDead();
        return;
       }
    tickTime();
}
//////////////////////


//flemmenwerfer////////////////////
FlameThrowerGoodies::FlameThrowerGoodies(StudentWorld* sw,double startX, double startY):Goodies(sw, IID_FLAME_THROWER_GOODIE, startX, startY)
{}

FlameThrowerGoodies:: ~FlameThrowerGoodies(){}

void FlameThrowerGoodies::doSomething(){
    if(!alive()) return;
    if(returnWorld()->overlapSoc(this)) {
        returnWorld()->increaseScore(300);
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        returnWorld()->getSoc()->addflamecharge(5);
        setDead();
        return;
    }
    tickTime();
}
//////////////////////////////////


//extralyfe///////////////////////
ExtraLifeGoodies::ExtraLifeGoodies(StudentWorld* sw,double startX, double startY):Goodies(sw, IID_EXTRA_LIFE_GOODIE, startX, startY)
{}

ExtraLifeGoodies:: ~ExtraLifeGoodies(){}

void ExtraLifeGoodies::doSomething(){
    if(!alive()) return;
    if(returnWorld()->overlapSoc(this)) {
        returnWorld()->increaseScore(500);
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        returnWorld()->incLives();
        setDead();
        return;
    }
    tickTime();
}
////////////////////////////////


//fungi///////////////////////
Fungi::Fungi(StudentWorld* sw,double startX, double startY):Goodies(sw, IID_FUNGUS, startX, startY)
{}

Fungi::~Fungi(){}

void Fungi::doSomething(){
    if(!alive()) return;
    if(returnWorld()->overlapSoc(this)) {
        returnWorld()->increaseScore(-50);
        returnWorld()->getSoc()->decrementHealth(20);
        setDead();
        return;
    }
    tickTime();
}
/////////////////////////////////



//food///////////////////////
Food::Food(StudentWorld* sw, double startX, double startY):Actor(sw, IID_FOOD, startX, startY, 90, 1)
{}

Food::~Food(){}

void Food::doSomething() {}


bool Food::damageable() {
    return false;
}

bool Food::killbyproj() {return false;}

bool Food::isedible() {return true;}

//bacterialpits//////////////////////////////////////
BacterialPits::BacterialPits(StudentWorld* sw, double startX, double startY)
:Actor(sw, IID_PIT, startX, startY, 0, 1)
{
    bacarray[0] = 5;
    bacarray[1] = 3;
    bacarray[2] = 2;
}

BacterialPits:: ~BacterialPits() {}

void BacterialPits::doSomething() {
    if (bacarray[0] == 0 && bacarray[0] == 0 && bacarray[0] == 0) {
        setDead();
        return;
    }
    int chanceB = randInt(1, 50);
    if (chanceB == 1) {
        int chooseBac = randInt(1, 3);
        if (chooseBac == 1) {
            
            if (bacarray[0] <= 0) chooseBac = randInt(1, 3);
            else {
                Bacteria *b = new RegularSalmonella(returnWorld(), returnX(), returnY());
                returnWorld()->addActor(b);
                returnWorld()->playSound(SOUND_BACTERIUM_BORN);
                bacarray[0]--;
                return;
            }
            
        }
        if (chooseBac == 2) {
            if (bacarray[1] <= 0) chooseBac = randInt(1, 3);
            else {
                Bacteria *b = new AggressiveSalmonella(returnWorld(), returnX(), returnY());
                returnWorld()->addActor(b);
                returnWorld()->playSound(SOUND_BACTERIUM_BORN);
                bacarray[1]--;
                return;
            }
        }
        if (chooseBac == 3) {
            if (bacarray[2] <= 0) chooseBac = randInt(1, 3);
            else {
                Bacteria *b = new EColi(returnWorld(), returnX(), returnY());
                returnWorld()->addActor(b);
                returnWorld()->playSound(SOUND_BACTERIUM_BORN);
                bacarray[2]--;
                return;
            }
        }
    }
     
}

bool BacterialPits::damageable() {return false;}
bool BacterialPits::killbyproj() {return false;}
bool BacterialPits::needtowin() {return true;}
///////////////////////////////////////////////////////////////////////////////////////

//bacteria base class////////////////////////////
Bacteria::Bacteria(StudentWorld* sw, int imageID, double startX, double startY, int hp, int dmg)
:Actor(sw, imageID, startX, startY, 90, 0), m_health(hp), m_dmg(dmg), m_mpd(0), m_food(0)
{}

Bacteria:: ~Bacteria(){}


int Bacteria::gethealth(){
    return m_health;
}

void Bacteria::decrementHealth(int hp) {
//    m_health-=hp;
//    if(m_health<=0) {
//        int x = randInt(0, 1);
//        if(x==0) {
//            Actor* a = new Food(returnWorld(), getX(),getY());
//            returnWorld()->addActor(a);
//        }
//        this->setDead();
//        returnWorld()->increaseScore(100);
    m_health -= hp;
        
}
bool Bacteria::needtowin() {return true;}

int Bacteria::getMPD() {return m_mpd;}
void Bacteria::subtractMPD() {m_mpd--;}
void Bacteria::resetMPD() {m_mpd = 10;}

int Bacteria::getDMG() {return m_dmg;}


bool Bacteria::ifOverlapSoc() {
    if (returnWorld()->overlapSoc(this)) {
        returnWorld()->getSoc()->decrementHealth(m_dmg);
        return true;
    }
    return false;
}

bool Bacteria::ifEatThreeFood(double& x, double& y) {
    if (m_food != 3) return false;
    if (returnX() == VIEW_WIDTH/2) x = returnX();
    if (returnX() < VIEW_WIDTH/2) x = returnX() + SPRITE_WIDTH/2;
    if (returnX() > VIEW_WIDTH/2) x = returnX() - SPRITE_WIDTH/2;
    
    if (returnY() == VIEW_HEIGHT/2) y = returnY();
    if (returnY() < VIEW_HEIGHT/2) y = returnY() + SPRITE_WIDTH/2;
    if (returnY() > VIEW_HEIGHT/2) y = returnY() - SPRITE_WIDTH/2;
    
    m_food = 0;
    
    return true;
    
}

bool Bacteria::ifOverlapFood() {
    if (returnWorld()->overlapFood(this)) {
        m_food++;
        return true;
    }
    return false;
}



////////////////////////////////////////////////////

//////////////////////////////////////////
Salmonella::Salmonella(StudentWorld* sw, double startX, double startY, int hp, int dmg)
:Bacteria(sw, IID_SALMONELLA, startX, startY, hp, dmg)
{}

Salmonella::~Salmonella(){}

bool Salmonella::followplan() {
    subtractMPD();
    double x,y;
    if (!returnWorld()->isblockedbyDirt(this,x,y,3)) {
        moveTo(x, y);
        return true;
    }
    else if (returnWorld()->isblockedbyDirt(this,x,y,3)) {
        setDirection(randInt(0, 359));
        resetMPD();
    }
    return true;
}

void Salmonella::dirFood() {
    int dir;

   
    
    if(returnWorld()->isFoodNearby(this, dir) == false) {
        setDirection(randInt(0, 359));
        resetMPD();
        return;
    }

    if (returnWorld()->isFoodNearby(this, dir)) {
           double x,y;
           setDirection(dir);
           resetMPD();
           if(returnWorld()->isblockedbyDirt(this, x, y,3)) {
               resetMPD();
               setDirection(randInt(0, 359));
           }
           return;
    }

}

void Salmonella::decrementHealth(int hp) {
    Bacteria::decrementHealth(hp);
    if(gethealth() > 0) returnWorld()->playSound(SOUND_SALMONELLA_HURT);
    if(gethealth()<=0) {
        int x = randInt(0, 1);
        if(x==0) {
            Actor* a = new Food(returnWorld(), getX(),getY());
            returnWorld()->addActor(a);
        }
        returnWorld()->playSound(SOUND_SALMONELLA_DIE);
        returnWorld()->increaseScore(100);
        this->setDead();
    }
}
///////////////////////////////////////////////


//regular salmon////////////////////////////////////////
RegularSalmonella::RegularSalmonella(StudentWorld* sw, double startX, double startY)
:Salmonella(sw, startX, startY, 4, 1)
{}

RegularSalmonella::~RegularSalmonella() {}

void RegularSalmonella::doSomething() {
    if(!alive()) return;
    
    if(ifOverlapSoc()) {
        if (getMPD() <= 0)  {
            dirFood();
            return;
        }
        followplan();
        return;
    }
    
    double x,y;
    if(ifEatThreeFood(x, y)) {
        Bacteria *a = new RegularSalmonella(returnWorld(),x,y);
        returnWorld()->addActor(a);
        if (getMPD() <= 0)  {
            dirFood();
            return;
        }
        followplan();
        return;
    }
    
    ifOverlapFood();
    if (getMPD() <= 0)  {
        dirFood();
        return;
    }
    followplan();
}
///////////////

//aggressive////////////
AggressiveSalmonella::AggressiveSalmonella(StudentWorld* sw, double startX, double startY)
:Salmonella(sw, startX, startY, 10, 2)
{}
AggressiveSalmonella:: ~AggressiveSalmonella() {}

void AggressiveSalmonella::doSomething() {
    if(!alive()) return;
    
    if (movetoSoc()) {
        ifOverlapSoc();
        double x,y;
        if (ifEatThreeFood(x, y)) {
            Bacteria *a = new RegularSalmonella(returnWorld(),x,y);
            returnWorld()->addActor(a);
        }
        ifOverlapFood();
        return;
    }
    if(ifOverlapSoc()) {
        if (getMPD() <= 0)  {
            dirFood();
            return;
        }
        followplan();
        return;
    }
    
    double x,y;
    if(ifEatThreeFood(x, y)) {
        Bacteria *a = new RegularSalmonella(returnWorld(),x,y);
        returnWorld()->addActor(a);
        if (getMPD() <= 0)  {
            dirFood();
            return;
        }
        followplan();
        return;
    }
    
    ifOverlapFood();
    if (getMPD() <= 0)  {
        dirFood();
        return;
    }
    followplan();
}

bool AggressiveSalmonella::movetoSoc() {
    if(returnWorld()->distanceFrom(getX(), getY(), returnWorld()->getSoc()->getX(), returnWorld()->getSoc()->getY()) <= 72) {
        int dir = atan2(returnWorld()->getSoc()->getY() - getY(), returnWorld()->getSoc()->getX() - getX()) * (180/3.14);
        setDirection(dir);
        double x,y;
        getPositionInThisDirection(getDirection(), 3, x, y);
        if(returnWorld()->distanceFrom(VIEW_WIDTH/2, VIEW_HEIGHT/2, x, y) >= VIEW_RADIUS) return false;
        moveTo(x, y);
        return true;
    }
    return false;
}
/////////////////////////////

///ecoli////////////////////////////
EColi::EColi(StudentWorld* sw, double startX, double startY)
:Bacteria(sw, IID_ECOLI, startX, startY, 5, 4)
{}
EColi:: ~EColi(){}

void EColi::doSomething() {
    if(!alive()) return;
    
    if(ifOverlapSoc()) {
        if (getMPD() <= 0)  {
            movetoSoc();
            return;
        }
    }
    
    double x,y;
    if(ifEatThreeFood(x, y)) {
        Bacteria *a = new RegularSalmonella(returnWorld(),x,y);
        returnWorld()->addActor(a);
        movetoSoc();
        return;
        }
    
    ifOverlapFood();
    movetoSoc();
    return;
    
}

void EColi::movetoSoc() {
    if(returnWorld()->distanceFrom(getX(), getY(), returnWorld()->getSoc()->getX(), returnWorld()->getSoc()->getY()) <= 256) {
        int dir = atan2(returnWorld()->getSoc()->getY() - getY(), returnWorld()->getSoc()->getX() - getX()) * (180/3.14);
        setDirection(dir);
        double x,y;
        if(!returnWorld()->isblockedbyDirt(this, x, y, 2)) {
            moveTo(x, y);
            return;
        }
        else {
            for (int i = 0; i < 10; i++) {
                setDirection(getDirection()+10);
                if(!returnWorld()->isblockedbyDirt(this, x, y, 2)) {
                    moveTo(x, y);
                    return;
                }
            }
        }
    }
}

void EColi::decrementHealth(int hp) {
   Bacteria::decrementHealth(hp);
    if (gethealth()>0) returnWorld()->playSound(SOUND_ECOLI_HURT);
    if(gethealth()<=0) {
        int x = randInt(0, 1);
        if(x==0) {
            Actor* a = new Food(returnWorld(), getX(),getY());
            returnWorld()->addActor(a);
        }
        returnWorld()->playSound(SOUND_ECOLI_DIE);
        returnWorld()->increaseScore(100);
        this->setDead();
    }
}
