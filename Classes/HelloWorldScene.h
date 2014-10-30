#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DrawNode3D.h"

struct GameData
{
    std::vector<std::string> textureList;
    struct Level
    {
        std::string background;
        std::vector< std::pair<int, int> > locations;
    };
    std::map< int,  Level> levelList;
};

class GameStart : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameStart);
    GameStart(){}
    ~GameStart(){}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    virtual bool init() override;
};

class GameOver : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameOver);
    GameOver(){}
    ~GameOver(){}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    virtual bool init() override;

};

class GameWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameWorld);
    GameWorld();

private:

    void setCameras(float width, float height);
    void setHandleEvent();
    void setTimeLine();
    void setScore();
    void resetLevel(int level);
    bool isTouchElement(cocos2d::Touch *touch);
    bool checkNeedEliminate();
    bool canReached(int location);
   // bool BFSearch(int start, int end, std::vector<int> &path, int searchCount);
    bool BFSearch(int start, int end, std::vector<int> &path);
    void eliminateElements(float dt);
    void updateTimeLine(float dt);
    void updateScore();
    void gameOver();
    void gamePass();

    bool readLevels();

private:

#define  MAX_ELEMENT_NUM_IN_LINE 10
#define  MAX_CAPACITY_NUM_IN_LINE (MAX_ELEMENT_NUM_IN_LINE + 2)
#define  MAX_CAPACITY_NUM (MAX_CAPACITY_NUM_IN_LINE * MAX_CAPACITY_NUM_IN_LINE)
    cocos2d::Sprite3D *_elements[MAX_CAPACITY_NUM];
    cocos2d::Layer *_elementContainer;
    cocos2d::Camera *_foreCamera;
    cocos2d::Camera *_backCamera;
    int _touchElements[2];
    cocos2d::Size _elementSize;

    cocos2d::DrawNode3D *_drawNode;

    cocos2d::ProgressTimer *_progress;
    cocos2d::Label *_timeLabel;
    cocos2d::Label *_scoreLabel;
    int _score;

    int _level;
    cocos2d::Label *_levelLabel;

    GameData _gameData;
};

#endif // __HELLOWORLD_SCENE_H__
