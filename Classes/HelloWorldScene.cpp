#include "HelloWorldScene.h"
#include "tinyxml2/tinyxml2.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define TOUCH_NONE -1
#define BACKGROUND_TAG 0x00010001


Scene* GameWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() || !readLevels())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

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
    _levelLabel = Label::createWithTTF("Level 0", "fonts/Marker Felt.ttf", 24);
    _levelLabel->setPosition(10.0f + _levelLabel->getContentSize().width, visibleSize.height - 20.0f);
    this->addChild(_levelLabel);
    auto iter = _gameData.levelList.begin();
    resetLevel(iter->first);
    setTimeLine();
    setScore();
    setHandleEvent();
    return true;
}

void GameWorld::resetLevel(int level)
{
    _level = level;
    char str[32];
    sprintf(str, "Level %d", _level);
    _levelLabel->setString(str);

    memset(_elements, 0, sizeof(_elements));
    _elementContainer->removeAllChildren();
    _touchElements[0] = _touchElements[1] = TOUCH_NONE;

    std::vector<std::pair<int, int> > emptySpace = _gameData.levelList[_level].locations;

    //std::vector<std::pair<int, int> > emptySpace;
    //for (int i = 1; i < MAX_CAPACITY_NUM_IN_LINE - 1; ++i)
    //	for (int j = 1; j < MAX_CAPACITY_NUM_IN_LINE - 1; ++j)
    //		emptySpace.push_back(std::pair<int, int>(i, j));


    while (!emptySpace.empty())
    {
        std::string tex = _gameData.textureList[(unsigned int)(CCRANDOM_0_1() * (_gameData.textureList.size() - 1))];
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
            sprite->setCameraMask((unsigned short)CameraFlag::USER1);
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
            sprite->setCameraMask((unsigned short)CameraFlag::USER1);
            _elementContainer->addChild(sprite);
            _elements[location.first * MAX_CAPACITY_NUM_IN_LINE + location.second] = sprite;
            emptySpace.erase(emptySpace.begin() + index);
        }

    }

    this->removeChildByTag(BACKGROUND_TAG);
    auto background = Sprite::create(_gameData.levelList[_level].background);
    background->setCameraMask((unsigned short)CameraFlag::USER2);
    background->setAnchorPoint(Vec2(0.0f, 0.0f));
    this->addChild(background, 0, BACKGROUND_TAG);

    setCameras(background->getContentSize().width, background->getContentSize().height);
}

void GameWorld::setCameras(float width, float height)
{
    this->removeChild(_backCamera);
    _backCamera = Camera::createOrthographic(width, height, 1.0f, 1000.0f);
    _backCamera->setCameraFlag(CameraFlag::USER2);
    _backCamera->setPosition3D(Vec3(0.0f, 0.0f, 1.0f ));
    _backCamera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    this->addChild(_backCamera);

    this->removeChild(_foreCamera);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _foreCamera=Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
    _foreCamera->setCameraFlag(CameraFlag::USER1);
    _foreCamera->setPosition3D(Vec3(0.0f, MAX_ELEMENT_NUM_IN_LINE * 1.2 , MAX_ELEMENT_NUM_IN_LINE * 1.2 ));
    _foreCamera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    this->addChild(_foreCamera);
}

void GameWorld::setHandleEvent()
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
        mat.translate(_foreCamera->getPosition3D());
        Vec3 currentPos;
        mat.getTranslation(&currentPos);
        _foreCamera->setPosition3D(currentPos);
        _foreCamera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameWorld::setTimeLine()
{
    auto size = Director::getInstance()->getWinSize();
    auto sprite = Sprite::create("timeline.png");
    _progress  = ProgressTimer::create(sprite);
    _progress->setType(ProgressTimer::Type::BAR);
    _progress->setPosition(Vec2(size.width / 2.0f, size.height - 20.0f));
    _progress->setMidpoint(Vec2(0.0f, 0.0f));
    _progress->setBarChangeRate(Vec2(1.0f, 0.0f));
    _progress->setPercentage(100);
    this->addChild(_progress);

    _timeLabel = Label::createWithTTF("100 S", "fonts/Marker Felt.ttf", 24);
    // position the label on the center of the screen
    _timeLabel->setPosition(Vec2(size.width / 2.0f - _progress->getContentSize().width / 2.0f - 30.0f, size.height - 20.0f));
    // add the label as a child to this layer
    this->addChild(_timeLabel);
    schedule(schedule_selector(GameWorld::updateTimeLine), 1.0f);
}

void GameWorld::updateTimeLine( float dt )
{
    float time = _progress->getPercentage() - dt;
    if (0.0f < time)
    {
        time = 100.0f < time? 100.0f : time;
        _progress->setPercentage(time);
        char str[16];
        sprintf(str, "%d S", (int)time);
        _timeLabel->setString(str);
    }
    else
    {
        gameOver();
    }
}

bool GameWorld::isTouchElement( Touch *touch )
{
    if (_touchElements[0] != TOUCH_NONE && _touchElements[1] != TOUCH_NONE) return false;

#if 0
    _drawNode->clear();
#endif
    std::vector<std::pair<int, Sprite3D *>> sortedElements;
    for (int i = 0; i < MAX_CAPACITY_NUM; ++i)
    {
        if (_elements[i])
        {
            Vec3 posInView;
            _foreCamera->getViewMatrix().transformPoint(_elements[i]->getPosition3D(), &posInView);
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
        _foreCamera->unproject(size, &nearP, &nearP);
        _foreCamera->unproject(size, &farP, &farP);
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
            scheduleOnce(schedule_selector(GameWorld::eliminateElements), 0.1f);
        }
        else
        {
            _elements[_touchElements[0]]->setColor(Color3B(255, 255, 255));
            //_elements[_touchElements[1]]->setColor(Color3B(255, 255, 255));
            _touchElements[0] = _touchElements[1];
             _touchElements[1] = TOUCH_NONE;
        }
    }

    return isTouch;
}

void GameWorld::eliminateElements(float dt)
{
    _elements[_touchElements[0]]->getParent()->removeChild(_elements[_touchElements[0]]);
    _elements[_touchElements[1]]->getParent()->removeChild(_elements[_touchElements[1]]);
    _elements[_touchElements[0]] = _elements[_touchElements[1]] = nullptr;
    _drawNode->clear();
     _touchElements[0] = _touchElements[1] = TOUCH_NONE;
     updateScore();
     updateTimeLine(-2.0f);
     if (_elementContainer->getChildrenCount() == 0)
         gamePass();
}

bool GameWorld::checkNeedEliminate()
{
    if (_elements[_touchElements[0]]->getName() == _elements[_touchElements[1]]->getName())
    {
        std::vector<int> path;
        if (BFSearch(_touchElements[0], _touchElements[1], path)){
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

bool GameWorld::canReached( int location )
{
    return _elements[location] == nullptr? true: false;
}

//bool HelloWorld::BFSearch( int start, int end, std::vector<int> &path, int searchCount)
//{
//    if (searchCount > 2) 
//        return false;
//    bool searchState = false;
//    do 
//    {
//		path.push_back(start);
//
//        int row = start / MAX_CAPACITY_NUM_IN_LINE;
//        //int col = start - (row * MAX_CAPACITY_NUM_IN_LINE);
//        //left row
//		std::vector<int> currentPath;
//        for (int i = start - 1; i >= (row * MAX_CAPACITY_NUM_IN_LINE); --i)
//        {
//            if (!canReached(i)){
//                if (i == end) {
//                    currentPath.push_back(i);
//                    searchState = true;
//                }
//                break;
//            }
//            if (BFSearch(i, end, currentPath, searchCount + 1)){
//                searchState = true;
//                break;
//            }
//        }
//        //right row
//		std::vector<int> temp;
//        for (int i = start + 1; i < ((row + 1) * MAX_CAPACITY_NUM_IN_LINE); ++i)
//        {
//            if (!canReached(i)){
//                if (i == end) {
//                    temp.push_back(i);
//                    searchState = true;
//                }
//                break;
//            }
//            if (BFSearch(i, end, temp, searchCount + 1)){
//                searchState = true;
//                break;
//            }
//        }
//        if (!temp.empty())
//		{
//			if (currentPath.empty() || temp.size() < currentPath.size())
//				temp.swap(currentPath);
//		}
//        //top col
//		temp.clear();
//        for (int i = start - MAX_CAPACITY_NUM_IN_LINE; i >= 0; i -= MAX_CAPACITY_NUM_IN_LINE)
//        {
//            if (!canReached(i)){
//                if (i == end) {
//                    temp.push_back(i);
//                    searchState = true;
//                }
//                break;
//            }
//            if (BFSearch(i, end, temp, searchCount + 1)){
//                searchState = true;
//                break;
//            }
//        }
//		if (!temp.empty())
//		{
//			if (currentPath.empty() || temp.size() < currentPath.size())
//				temp.swap(currentPath);
//		}
//        //button row
//		temp.clear();
//        for (int i = start + MAX_CAPACITY_NUM_IN_LINE; i < MAX_CAPACITY_NUM; i += MAX_CAPACITY_NUM_IN_LINE)
//        {
//            if (!canReached(i)){
//                if (i == end) {
//                    temp.push_back(i);
//                    searchState = true;
//                }
//                break;
//            }
//            if (BFSearch(i, end, temp, searchCount + 1)){
//                searchState = true;
//                break;
//            }
//        }
//        if (!searchState)
//            path.pop_back();
//		else
//		{
//			if (!temp.empty())
//			{
//				if (currentPath.empty() || temp.size() < currentPath.size())
//					temp.swap(currentPath);
//			}
//
//			path.insert(path.end(), currentPath.begin(), currentPath.end());
//		}
//    } while (false);
//
//    return searchState;
//}

bool GameWorld::BFSearch( int start, int end, std::vector<int> &path )
{
    std::set<int> S;
    std::set<int> T;
    S.insert(start);

    int crossNum = 0;
    bool isFind = false;
    std::map<int, std::vector<int> > inverseMap;
    while (!isFind && crossNum < 3)
    {
        for (auto iter : S)
        {
            int row = iter / MAX_CAPACITY_NUM_IN_LINE;
            for (int i = iter - 1; i >= (row * MAX_CAPACITY_NUM_IN_LINE); --i){
                if (!canReached(i)){
                    if (i == end){
                        isFind = true;
                        inverseMap[i].push_back(iter);
                    }
                    break;
                }
                T.insert(i);
                inverseMap[i].push_back(iter);
            }
            //if (isFind) break;

            for (int i = iter + 1; i < ((row + 1) * MAX_CAPACITY_NUM_IN_LINE); ++i){
                if (!canReached(i)){
                    if (i == end){
                        isFind = true;
                        inverseMap[i].push_back(iter);
                    }
                    break;
                }
                T.insert(i);
                inverseMap[i].push_back(iter);
            }
            //if (isFind) break;

            for (int i = iter - MAX_CAPACITY_NUM_IN_LINE; i >= 0; i -= MAX_CAPACITY_NUM_IN_LINE){
                if (!canReached(i)){
                    if (i == end){
                        isFind = true;
                        inverseMap[i].push_back(iter);
                    }
                    break;
                }
                T.insert(i);
                inverseMap[i].push_back(iter);
            }
            //if (isFind) break;

             for (int i = iter + MAX_CAPACITY_NUM_IN_LINE; i < MAX_CAPACITY_NUM; i += MAX_CAPACITY_NUM_IN_LINE){
                 if (!canReached(i)){
                     if (i == end){
                         isFind = true;
                         inverseMap[i].push_back(iter);
                     }
                     break;
                 }
                 T.insert(i);
                 inverseMap[i].push_back(iter);
             }
             //if (isFind) break;
        }
        S.insert(T.begin(), T.end());
        ++crossNum;
    }


    if (isFind){
        path.push_back(end);
        do 
        {
            int count = 0;
            if (crossNum == 1){
                break;
            }
            if (crossNum == 2){
                auto direct = inverseMap[end].at(0);
                path.push_back(direct);
                break;
            }
            if (crossNum == 3){
                bool findPath = false;
                auto vec = inverseMap[end];
                int nearest = vec.at(0);
                for (auto iter : vec){
                    nearest = abs(iter - end) < abs(nearest - end)? iter : nearest;
                    }

                path.push_back(nearest);
                auto vec1 = inverseMap[nearest];
                for (auto iter1 : vec1){
                    auto vec2 = inverseMap[iter1];
                    for (auto iter2 : vec2){
                        if (iter2 == start){
                            path.push_back(iter1);
                            findPath = true;
                            break;
                        }
                    }
                    if (findPath) break;
                }

            }
        } while (false);
        path.push_back(start);
    }

    return isFind;
}

void GameWorld::gameOver()
{
    auto scene = TransitionFade::create(0.5f, GameOver::createScene());
    Director::getInstance()->replaceScene(scene);
}

void GameWorld::setScore()
{
    auto size = Director::getInstance()->getWinSize();

    _scoreLabel = Label::createWithTTF("0 Score", "fonts/Marker Felt.ttf", 24);
    // position the label on the center of the screen
    _scoreLabel->setPosition(Vec2(size.width - _scoreLabel->getContentSize().width - 20.0f, size.height - 20.0f));
    // add the label as a child to this layer
    this->addChild(_scoreLabel);

    _score = 0;
}

void GameWorld::updateScore()
{
    ++_score;
    char str[32];
    sprintf(str, "%d Score", _score);
    _scoreLabel->setString(str);
}

void GameWorld::gamePass()
{
    int level = _level + 1;
    int lastLevel = _gameData.levelList.rbegin()->first;
    if (lastLevel < level)
        gameOver();
    else
        resetLevel(level);
}

bool GameWorld::readLevels()
{
    tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument;
    tinyxml2::XMLError error = xmlDoc->LoadFile("levels.xml");
    if (error != tinyxml2::XML_SUCCESS) return false;
    tinyxml2::XMLElement *linkgame = xmlDoc->FirstChildElement("LinkGame");
    if (linkgame)
    {
        tinyxml2::XMLElement *textures = linkgame->FirstChildElement("Textures");
        if (textures)
        {
            tinyxml2::XMLElement *texture = textures->FirstChildElement("texture");
            while (texture)
            {
                _gameData.textureList.push_back(texture->GetText());
                texture = texture->NextSiblingElement("texture");
            }
        }

        tinyxml2::XMLElement *level = linkgame->FirstChildElement("Level");
        while (level)
        {
            int val = atoi(level->Attribute("value"));
            _gameData.levelList[val].background = level->Attribute("background");
            tinyxml2::XMLElement *location = level->FirstChildElement("location");
            while (location)
            {
                int x = atoi(location->Attribute("x"));
                int y = atoi(location->Attribute("y"));
                _gameData.levelList[val].locations.push_back(std::pair<int, int>(x, y));
                location = location->NextSiblingElement("location");
            }

            level = level->NextSiblingElement("Level");
        }
    }
    
    delete xmlDoc;
    return true;
}

GameWorld::GameWorld()
    : _elementContainer(nullptr)
    , _foreCamera(nullptr)
    , _backCamera(nullptr)
    , _drawNode(nullptr)
    , _progress(nullptr)
    , _timeLabel(nullptr)
    , _scoreLabel(nullptr)
    , _levelLabel(nullptr)
{

}

cocos2d::Scene* GameStart::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameStart::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameStart::init()
{
    // 1. super init first
    if ( !Layer::init())
    {
        return false;
    }

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("startground.png");
    // position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    sprite->setAnchorPoint(Vec2(0.0f, 0.0f));
    //add the sprite as a child to this layer
    this->addChild(sprite);

    {
        auto startButton = ui::Button::create("animationbuttonnormal.png");
        startButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
        startButton->addTouchEventListener([&](Ref *pSender, ui::Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::ENDED){
                Director::getInstance()->replaceScene(GameWorld::createScene());
            }
        });
        startButton->setZoomScale(0.4f);
        startButton->setScale(1.5f);
        startButton->setPressedActionEnabled(true);
        auto text = ui::Text::create("Start","fonts/Marker Felt.ttf",24);
        text->setColor(Color3B(255, 255, 255));
        text->setPosition(startButton->getPosition());
        this->addChild(startButton);
        this->addChild(text);
    }

    {
        auto quitButton = ui::Button::create("animationbuttonnormal.png");
        quitButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f - visibleSize.height * 0.2));
        quitButton->addTouchEventListener([&](Ref *pSender, ui::Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::ENDED){
                Director::getInstance()->end();
            }
        });
        quitButton->setZoomScale(0.4f);
        quitButton->setScale(1.5f);
        quitButton->setPressedActionEnabled(true);
        auto text = ui::Text::create("Quit","fonts/Marker Felt.ttf",24);
        text->setColor(Color3B(255, 255, 255));
        text->setPosition(quitButton->getPosition());
        this->addChild(quitButton);
        this->addChild(text);
    }


    auto camera = Camera::createOrthographic(sprite->getContentSize().width, sprite->getContentSize().height, 1.0f, 1000.0f);
    camera->setCameraFlag(CameraFlag::USER1);
    camera->setPosition3D(Vec3(0.0f, 0.0f, 1.0f));
    camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    this->addChild(camera);
    return true;
}

cocos2d::Scene* GameOver::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameOver::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene

    return scene;
}

bool GameOver::init()
{
    // 1. super init first
    if ( !Layer::init())
    {
        return false;
    }

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("gameover.png");
    // position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    sprite->setAnchorPoint(Vec2(0.0f, 0.0f));
    //add the sprite as a child to this layer
    this->addChild(sprite);

    {
        auto resetButton = ui::Button::create("reset.png");
        resetButton->setPosition(Vec2(visibleSize.width / 2.0f - visibleSize.width * 0.2, visibleSize.height / 2.0f - visibleSize.height * 0.2));
        resetButton->addTouchEventListener([&](Ref *pSender, ui::Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::ENDED){
                Director::getInstance()->replaceScene(GameWorld::createScene());
            }
        });
        resetButton->setZoomScale(0.4f);
        resetButton->setScale(1.5f);
        resetButton->setPressedActionEnabled(true);
        this->addChild(resetButton);
    }

    {
        auto menuButton = ui::Button::create("menu.png");
        menuButton->setPosition(Vec2(visibleSize.width / 2.0f + visibleSize.width * 0.2, visibleSize.height / 2.0f - visibleSize.height * 0.2));
        menuButton->addTouchEventListener([&](Ref *pSender, ui::Widget::TouchEventType type){
            if (type == ui::Widget::TouchEventType::ENDED){
                Director::getInstance()->replaceScene(GameStart::createScene());
            }
        });
        menuButton->setZoomScale(0.4f);
        menuButton->setScale(1.5f);
        menuButton->setPressedActionEnabled(true);
        this->addChild(menuButton);
    }

    auto camera = Camera::createOrthographic(sprite->getContentSize().width, sprite->getContentSize().height, 1.0f, 1000.0f);
    camera->setCameraFlag(CameraFlag::USER1);
    camera->setPosition3D(Vec3(0.0f, 0.0f, 1.0f));
    camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    this->addChild(camera);
    return true;
}
