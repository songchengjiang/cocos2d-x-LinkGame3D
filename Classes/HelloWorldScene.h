#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DrawNode3D.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:

    void setCamera();
    void setHandleEvent();
    void resetScene();
    bool isTouchElement(cocos2d::Touch *touch);
    bool checkNeedEliminate();
    bool canReached(int location);
    bool BFSearch(int start, int end, std::vector<int> &path, int searchCount);

private:

#define  MAX_ELEMENT_NUM_IN_LINE 10
#define  MAX_CAPACITY_NUM_IN_LINE (MAX_ELEMENT_NUM_IN_LINE + 2)
#define  MAX_CAPACITY_NUM (MAX_CAPACITY_NUM_IN_LINE * MAX_CAPACITY_NUM_IN_LINE)
    cocos2d::Sprite3D *_elements[MAX_CAPACITY_NUM];
    cocos2d::Layer *_elementContainer;
    cocos2d::Camera *_camera;
    int _touchElements[2];
    cocos2d::Size _elementSize;

    cocos2d::DrawNode3D *_drawNode;
};

#endif // __HELLOWORLD_SCENE_H__
