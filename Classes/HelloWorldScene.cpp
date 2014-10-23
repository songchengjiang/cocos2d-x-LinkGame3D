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
    _elementContainer->removeAllChildren();
    _touchElements[0] = _touchElements[1] = TOUCH_NONE;
    for (int i = 0; i < MAX_ELEMENT_NUM_IN_LINE; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT_NUM_IN_LINE; ++j)
        {
            auto sprite = Sprite3D::create("models/cow.c3b");
            auto size = sprite->getBoundingBox().size;
            std::string tex = images[(unsigned int)(CCRANDOM_0_1() * (IMAGE_NUM - 1))];
            sprite->setTexture(tex);
            sprite->setName(tex);
            sprite->setPosition3D(Vec3((-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.width / 2.0f) + i) * size.width, 0.0f, (-(MAX_ELEMENT_NUM_IN_LINE / 2 - size.height / 2.0f) + j) * size.height));
            //sprite->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.01f, 255, 0, 0), TintTo::create(0.01f, 255, 255, 255), nullptr)));
            _elementContainer->addChild(sprite);
            _elements[i * MAX_ELEMENT_NUM_IN_LINE + j] = sprite;
        }
    }
    auto sprite = Sprite3D::create("models/cow.c3b");
    sprite->setPosition3D(Vec3(0.0f, 1.0f, 0.0f));
    _elementContainer->addChild(sprite);
    _elementContainer->setCameraMask((unsigned short)CameraFlag::USER1);
}

void HelloWorld::setCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _camera=Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
    _camera->setCameraFlag(CameraFlag::USER1);
    _camera->setPosition3D(Vec3(0.0f, MAX_ELEMENT_NUM_IN_LINE * 2.0f, -MAX_ELEMENT_NUM_IN_LINE * 2.0f));
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
    bool isTouch = false;
    for (int i = 0; i < MAX_ELEMENT_NUM_IN_LINE; ++i)
    {
        for (int j = 0; j < MAX_ELEMENT_NUM_IN_LINE; ++j)
        {
            int index = i * MAX_ELEMENT_NUM_IN_LINE + j;
            auto location = touch->getLocationInView();
            Vec3 nearP(location.x, location.y, 0.0f), farP(location.x, location.y, 1.0f);
            auto size = Director::getInstance()->getWinSize();
            _camera->unproject(size, &nearP, &nearP);
            _camera->unproject(size, &farP, &farP);
            auto ray = Ray(nearP, (farP - nearP).getNormalized());
            const AABB &aabb = _elements[index]->getAABB();
            if (ray.intersects(aabb))
            {
                _elements[index]->setColor(Color3B(255, 0, 0));
                _touchElements[0] == TOUCH_NONE? _touchElements[0] = index: _touchElements[1] = index;
                isTouch = true;
            }
        }
    }

    if (_touchElements[0] != TOUCH_NONE && _touchElements[1] != TOUCH_NONE)
    {
        if (checkNeedEliminate())
        {
            _elements[_touchElements[0]]->setVisible(false);
            _elements[_touchElements[1]]->setVisible(false);
            _touchElements[0] = _touchElements[1] = TOUCH_NONE;
        }
        else
        {
            _elements[_touchElements[0]]->setColor(Color3B(255, 255, 255));
            _elements[_touchElements[1]]->setColor(Color3B(255, 255, 255));
        }
    }

    return isTouch;
}

bool HelloWorld::checkNeedEliminate()
{
    return false;
}
