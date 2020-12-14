#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    double distanceFrom(double X1, double Y1, double X2, double Y2);
    Socrates* getSoc();
    int max(int x, int y);
    void randRim(double& x, double& y);
    bool overlapSoc(Actor* c);
    void addActor(Actor* c);
    bool projOverlap(Actor* c, int hp);
    bool positionfree(double x, double y, bool dirtexempt);
    bool overlapFood(Actor* c);
    bool isblockedbyDirt(Actor* c, double& x, double& y, int amt);
    bool isFoodNearby(Actor* c, int& dir);
    

private:
    std::vector <Actor*> ActorsinGame;
    Socrates* Socratesptr;
    
};

#endif // STUDENTWORLD_H_
