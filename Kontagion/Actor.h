#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor:public GraphObject{
  public:
    Actor(StudentWorld* sw,int imageID, double startX, double startY, int dir, int depth);
    bool alive();
    virtual void doSomething() = 0;
    virtual bool damageable();
    virtual ~Actor();
    StudentWorld* returnWorld();
    double returnX();
    double returnY();
    void setDead();
    virtual bool isDirt();
    virtual bool killbyproj();
    virtual bool needtowin();
    virtual bool isedible();
    virtual void decrementHealth(int hp);

  private:
    bool m_alive;
    StudentWorld *m_sw;
};

class Socrates :public Actor {
  public:
    Socrates(StudentWorld* sw, double startX, double startY);
    virtual ~Socrates();
    virtual void doSomething();
    virtual void decrementHealth(int hp);
    void addflamecharge(int f);
    void fullhealth();
    int gethealth();
    virtual bool killbyproj();
    int getspray();
    int getflame();
  private:
    int m_flamecharges;
    int m_spraycharges;
    int m_health;
};


class Food: public Actor {
  public:
    Food(StudentWorld* sw, double startX, double startY);
    virtual ~Food();
    virtual void doSomething();
    virtual bool damageable();
    virtual bool killbyproj();
    virtual bool isedible();
};

class BacterialPits: public Actor {
  public:
    BacterialPits(StudentWorld* sw, double startX, double startY);
    virtual ~BacterialPits();
    virtual void doSomething();
    virtual bool killbyproj();
    virtual bool damageable();
    virtual bool needtowin();
    
  private:
    int bacarray[3];
    
    
};

class Bacteria:public Actor {
public:
    Bacteria(StudentWorld* sw, int imageID, double startX, double startY, int hp, int dmg);
    virtual ~Bacteria();
    virtual void doSomething() = 0;
    //health accessor and mutator
    int gethealth();
    virtual void decrementHealth(int hp);
    //bool if needed
    virtual bool needtowin();
    //mpd accessor and mutator
    int getMPD();
    void subtractMPD();
    void resetMPD();
    //access dmg
    int getDMG();
    
    //2
    bool ifOverlapSoc();
    bool ifEatThreeFood(double& x, double& y);
    bool ifOverlapFood();
    
private:
    int m_health;
    int m_mpd;
    int m_dmg;
    int m_food;
    
};


class Salmonella:public Bacteria {
  public:
    Salmonella(StudentWorld* sw, double startX, double startY, int hp, int dmg);
    virtual ~Salmonella();
    virtual void doSomething() = 0;
    bool followplan();
    void dirFood();
    virtual void decrementHealth(int hp);
    
    
};

class RegularSalmonella:public Salmonella {
  public:
    RegularSalmonella(StudentWorld* sw, double startX, double startY);
    virtual ~RegularSalmonella();
    virtual void doSomething();
    
};

class AggressiveSalmonella:public Salmonella {
  public:
    AggressiveSalmonella(StudentWorld* sw, double startX, double startY);
    virtual ~AggressiveSalmonella();
    virtual void doSomething();
    bool movetoSoc();
    
};

class EColi:public Bacteria {
  public:
    EColi(StudentWorld* sw, double startX, double startY);
    virtual ~EColi();
    virtual void doSomething();
    void movetoSoc();
    virtual void decrementHealth(int hp);
};


//done
class Projectiles: public Actor {
  public:
    Projectiles(StudentWorld* sw, int imageID, double startX, double startY, int dir, int maxD, int dmg);
    virtual ~Projectiles();
    virtual void doSomething();
    int getDistanceLeft();
    void decreaseDistance();
    virtual bool killbyproj();
    
  private:
    int m_maxD;
    int m_dmg;
    
};

class FlameProjectiles: public Projectiles {
  public:
    FlameProjectiles(StudentWorld* sw, double startX, double startY, int dir);
    virtual ~FlameProjectiles();
    
};

class SprayProjectiles: public Projectiles {
  public:
    SprayProjectiles(StudentWorld* sw, double startX, double startY, int dir);
    virtual ~SprayProjectiles();
};



//done
class DirtPile:public Actor {
  public:
    DirtPile(StudentWorld* sw,double startX, double startY);
    virtual ~DirtPile();
    virtual void doSomething();
    virtual bool isDirt();
};



//done
class Goodies: public Actor {
  public:
    Goodies(StudentWorld* sw, int imageID, double startX, double startY);
    virtual ~Goodies();
    virtual void doSomething() = 0;
    int getTime();
    void tickTime();
    
  private:
    int m_maxT;
};

class RestoreHealthGoodies: public Goodies {
  public:
    RestoreHealthGoodies(StudentWorld* sw,double startX, double startY);
    virtual ~RestoreHealthGoodies();
    virtual void doSomething();
};

class FlameThrowerGoodies: public Goodies {
  public:
    FlameThrowerGoodies(StudentWorld* sw,double startX, double startY);
    virtual ~FlameThrowerGoodies();
    virtual void doSomething();
    
};

class ExtraLifeGoodies: public Goodies {
  public:
    ExtraLifeGoodies(StudentWorld* sw,double startX, double startY);
    virtual ~ExtraLifeGoodies();
    virtual void doSomething();
    
};

class Fungi: public Goodies {
  public:
    Fungi(StudentWorld* sw,double startX, double startY);
    virtual ~Fungi();
    virtual void doSomething();
};

#endif // ACTOR_H_
