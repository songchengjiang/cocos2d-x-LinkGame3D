#include "HelloWorldScene.h"

USING_NS_CC;

#define TOUCH_NONE -1
#define IMAGE_NUM 10
const char *images[IMAGE_NUM] = {"models/bear.png",  "models/beaver.png", "models/cat.png",  "models/cow.png", "models/dog.png"
                                  ,  "models/elephant.png", "models/elk.png",  "models/giraffe.png", "models/mouse.png",  "models/owl.png"};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    //// 3. add your codes below...

    //// add a label shows "Hello World"
    //// create and initialize a label
    //
    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //
    //// position the label on the center of the screen
    //label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - label->getContentSize().height));

    //// add the label as a child to this layer
    //this->addChild(label, 1);

    //// add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //// add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    
    _elementContainer = Layer::create();
    this->addChild(_elementContainer);
    _drawNode = DrawNode3D::create();
    _drawNode->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(_drawNode);
    resetScene();
    setCamera();
    setHandleEvent();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::resetScene()
{
    memset(_elements, 0, sizeof(_elements));
    _elementContainer->removeAllChildren();
    _touchElements[0] = _touchElements[1] = TOUCH_NONE;

    std::vector<std::pair<int, int> > emptySpace;
    for (int i = 1; i < MAX_CAPACITY_NUM_IN_LINE - 1; ++i)
        for (int j = 1; j < MAX_CAPACITY_NUM_IN_LINE - 1; ++j)
            emptySpace.push_back(std::pair<int, int>(i, j));


    while (!emptySpace.empty())
    {
        std::string tex = images[(unsigned int)(CCRANDOM_0_1() * (IMAGE_NUM - 1))];
        //first element
        {
            unsigned int index = (unsigned int)(CCRANDOM_0_1() * (emptySpace.size() - 1));
            auto location = emptySpace.at(index);
            auto sprite = Sprite3D::create("models/box.c3b");
            auto size = sprite->getBoundingBox().size;
            _elementSize = size;
            sprite->setTexture(tex);
            sprite->setName(tex);
            sprite->setPosition3D(Vec3((-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.width / 2.0f) + location.first - 1) * size.width, 0.0f, (-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.height / 2.0f) + location.second - 1) * size.height));
            _elementContainer->addChild(sprite);
            _elements[location.first * MAX_CAPACITY_NUM_IN_LINE + location.second] = sprite;
            emptySpace.erase(emptySpace.begin() + index);
        }
        //second element
        {
            unsigned int index = (unsigned int)(CCRANDOM_0_1() * (emptySpace.size() - 1));
            auto location = emptySpace.at(index);
            auto sprite = Sprite3D::create("models/box.c3b");
            auto size = sprite->getBoundingBox().size;
            _elementSize = size;
            sprite->setTexture(tex);
            sprite->setName(tex);
            sprite->setPosition3D(Vec3((-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.width / 2.0f) + location.first - 1) * size.width, 0.0f, (-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.height / 2.0f) + location.second - 1) * size.height));
            _elementContainer->addChild(sprite);
            _elements[location.first * MAX_CAPACITY_NUM_IN_LINE + location.second] = sprite;
            emptySpace.erase(emptySpace.begin() + index);
        }

    }

    //auto sprite = Sprite3D::create("D:/Develop/cocos2d-x/tests/cpp-tests/Resources/Sprite3DTest/orc.c3b");
    //sprite->setPosition3D(Vec3(0.0f, 1.0f, 0.0f));
    //sprite->setScale(0.1f);
    //_elementContainer->addChild(sprite);
    _elementContainer->setCameraMask((unsigned short)CameraFlag::USER1);
}

void HelloWorld::setCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _camera=Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
    _camera->setCameraFlag(CameraFlag::USER1);
    _camera->setPosition3D(Vec3(0.0f, MAX_ELEMENT_NUM_IN_LINE * 1.2 , MAX_ELEMENT_NUM_IN_LINE * 1.2 ));
    _camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    this->addChild(_camera);
}

void HelloWorld::setHandleEvent()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch *touch, Event *event){
        return !isTouchElement(touch);
    };
    listener->onTouchMoved = [&](Touch *touch, Event *event){
        Vec2 location = touch->getLocationInView();
        Vec2 preLocation = touch->getPreviousLocationInView();
        float delta = (location - preLocation).dot(Vec2(-1.0f, 0.0f));
        Mat4 mat;
        mat.rotateY(delta * 0.01f);
        mat.translate(_camera->getPosition3D());
        Vec3 currentPos;
        mat.getTranslation(&currentPos);
        _camera->setPosition3D(currentPos);
        _camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HelloWorld::isTouchElement( Touch *touch )
{
#if 0
    _drawNode->clear();
#endif
    std::vector<std::pair<int, Sprite3D *>> sortedElements;
    for (int i = 0; i < MAX_CAPACITY_NUM; ++i)
    {
        if (_elements[i])
        {
            Vec3 posInView;
            _camera->getViewMatrix().transformPoint(_elements[i]->getPosition3D(), &posInView);
            _elements[i]->setGlobalZOrder(-posInView.z);
            sortedElements.push_back(std::pair<int, Sprite3D *>(i, _elements[i]));
        }
    }

    std::sort(sortedElements.begin(), sortedElements.end(), [](std::pair<int, Sprite3D *> left, std::pair<int, Sprite3D *> right){
        return left.second->getGlobalZOrder() < right.second->getGlobalZOrder();
    });

    bool isTouch = false;
    for (auto iter : sortedElements)
    {
#if 0
        const AABB &degaabb = iter.second->getAABB();
        Vec3 degcorners[8];
        degaabb.getCorners(degcorners);
        _drawNode->drawCube(degcorners, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
#endif

        auto location = touch->getLocationInView();
        Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);
        auto size = Director::getInstance()->getWinSize();
        _camera->unproject(size, &nearP, &nearP);
        _camera->unproject(size, &farP, &farP);
        auto ray = Ray(nearP, (farP - nearP).getNormalized());
        const AABB &aabb = iter.second->getAABB();
        if (ray.intersects(aabb))
        {
            if (iter.first != _touchElements[0])
            {
                _elements[iter.first]->setColor(Color3B(255, 0, 0));
                _touchElements[0] == TOUCH_NONE? _touchElements[0] = iter.first: _touchElements[1] = iter.first;
                isTouch = true;
            }
            break;
        }
    }

    if (_touchElements[0] != TOUCH_NONE && _touchElements[1] != TOUCH_NONE)
    {
        if (checkNeedEliminate())
        {
            _elements[_touchElements[0]]->getParent()->removeChild(_elements[_touchElements[0]]);
            _elements[_touchElements[1]]->getParent()->removeChild(_elements[_touchElements[1]]);
            _elements[_touchElements[0]] = _elements[_touchElements[1]] = nullptr;
            //_drawNode->clear();
        }
        else
        {
            _elements[_touchElements[0]]->setColor(Color3B(255, 255, 255));
            _elements[_touchElements[1]]->setColor(Color3B(255, 255, 255));
        }
        _touchElements[0] = _touchElements[1] = TOUCH_NONE;
    }

    return isTouch;
}

bool HelloWorld::checkNeedEliminate()
{
    if (_elements[_touchElements[0]]->getName() == _elements[_touchElements[1]]->getName())
    {
        std::vector<int> path;
        if (BFSearch(_touchElements[0], _touchElements[1], path, 0)){
            for (unsigned int i = 0; i < path.size() - 1; ++i)
            {
                int rowi = path[i] / MAX_CAPACITY_NUM_IN_LINE;
                int coli = path[i] - (rowi * MAX_CAPACITY_NUM_IN_LINE);
                int rowi1 = path[i + 1] / MAX_CAPACITY_NUM_IN_LINE;
                int coli1 = path[i + 1] - (rowi1 * MAX_CAPACITY_NUM_IN_LINE);
                Vec3 posi = Vec3((-(MAX_CAPACITY_NUM_IN_LINE / 2 - _elementSize.width / 2.0f) + rowi) * _elementSize.width, 0.0f, (-(MAX_CAPACITY_NUM_IN_LINE / 2 - _elementSize.height / 2.0f) + coli) * _elementSize.height);
                Vec3 posi1 = Vec3((-(MAX_CAPACITY_NUM_IN_LINE / 2 - _elementSize.width / 2.0f) + rowi1) * _elementSize.width, 0.0f, (-(MAX_CAPACITY_NUM_IN_LINE / 2 - _elementSize.height / 2.0f) + coli1) * _elementSize.height);
                _drawNode->drawLine(posi, posi1, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
            }
            return true;
        }
    }
    return false;
}

bool HelloWorld::canReached( int location )
{
    return _elements[location] == nullptr? true: false;
}

bool HelloWorld::BFSearch( int start, int end, std::vector<int> &path, int searchCount)
{
    if (searchCount > 2) 
        return false;
    bool searchState = false;
    do 
    {
        path.push_back(start);
        int row = start / MAX_CAPACITY_NUM_IN_LINE;
        //int col = start - (row * MAX_CAPACITY_NUM_IN_LINE);
        //left row
        for (int i = start - 1; i >= (row * MAX_CAPACITY_NUM_IN_LINE); --i)
        {
            if (!canReached(i)){
                if (i == end) {
                    path.push_back(i);
                    searchState = true;
                }
                break;
            }
            if (BFSearch(i, end, path, searchCount + 1)){
                searchState = true;
                break;
            }
        }
        if (searchState) break;
        //right row
        for (int i = start + 1; i < ((row + 1) * MAX_CAPACITY_NUM_IN_LINE); ++i)
        {
            if (!canReached(i)){
                if (i == end) {
                    path.push_back(i);
                    searchState = true;
                }
                break;
            }
            if (BFSearch(i, end, path, searchCount + 1)){
                searchState = true;
                break;
            }
        }
        if (searchState) break;
        //top col
        for (int i = start - MAX_CAPACITY_NUM_IN_LINE; i >= 0; i -= MAX_CAPACITY_NUM_IN_LINE)
        {
            if (!canReached(i)){
                if (i == end) {
                    path.push_back(i);
                    searchState = true;
                }
                break;
            }
            if (BFSearch(i, end, path, searchCount + 1)){
                searchState = true;
                break;
            }
        }
        if (searchState) break;
        //button row
        for (int i = start + MAX_CAPACITY_NUM_IN_LINE; i < MAX_CAPACITY_NUM; i += MAX_CAPACITY_NUM_IN_LINE)
        {
            if (!canReached(i)){
                if (i == end) {
                    path.push_back(i);
                    searchState = true;
                }
                break;
            }
            if (BFSearch(i, end, path, searchCount + 1)){
                searchState = true;
                break;
            }
        }
        if (!searchState)
            path.pop_back();
    } while (false);

    return searchState;
}