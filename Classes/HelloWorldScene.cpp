#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"
#include "GameBoard.h"

USING_NS_CC;

Scene* HelloWorld::createScene(){
    return HelloWorld::create();
}

static void problemLoading(const char* filename){
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init(){
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //Movement using Actions

    // //Basic Movement using MoveBy
    // auto moveByAction = MoveBy::create(3,Point(100,0));
    // mySprite->runAction(moveByAction);


    // //Basic Movement using MoveTo
    // auto moveToAction = MoveTo::create(3,Point(-100,0));
    // mySprite->runAction(moveToAction);

    // //Basic Movement using JumpBy
    // auto jumpByAction = JumpBy::create(3,Point(100,0),50,1);
    // mySprite->runAction(jumpByAction);

    // //Basic Movement using JumpTo
    // auto jumpToAction = JumpTo::create(3,Point(100,0),50,1);
    // mySprite->runAction(jumpToAction);

    // //Basic Movement using BazierBy
    // ccBezierConfig bezier;
    // bezier.controlPoint_1 = Point(0,visibleSize.height / 2);
    // bezier.controlPoint_2 = Point(300,-visibleSize.height / 2);
    // bezier.endPosition = Point(200,100);

    // auto bezierByAction = BezierBy::create(3,bezier);

    // mySprite->runAction(bezierByAction);

    // //Basic Movement using BezierTo
    // ccBezierConfig bezier;
    // bezier.controlPoint_1 = Point(0,visibleSize.height / 2);
    // bezier.controlPoint_2 = Point(300,-visibleSize.height / 2);
    // bezier.endPosition = Point(200,100);

    // auto bezierToAction = BezierTo::create(3,bezier);

    // mySprite->runAction(bezierToAction);

    //Positioning using Place
    // auto placeAction = Place::create(Point(10,10));
    // mySprite->runAction(placeAction);

    //Scaling using ScaleBy
    // auto scaleByAction = ScaleBy::create(3,2.0);
    // mySprite->runAction(scaleByAction);

    //Scaling using ScaleTo
    // auto scaleToAction = ScaleTo::create(3,2.0);
    // mySprite->runAction(scaleToAction);
    // return true;

    //Rotating using RotateBy
    // auto rotateByAction = RotateBy::create(3,Vec3(0.0,0.0,90.0));
    // mySprite->runAction(rotateByAction);

    //Rotating using RotateTo
    // auto rotateToAction = RotateTo::create(3,Vec3(0,0,-90));
    // mySprite->runAction(rotateToAction);


    //Tinting using TintBy
    // auto tintByAction = TintBy::create(3,0,0,-255);
    // mySprite->runAction(tintByAction);


    //Repeat Action
    // auto action = RotateBy::create(3,45);
    // auto repeatAction = Repeat::create(action,8);

    // mySprite->runAction(repeatAction);

    //Repeat Action Forever
    // auto action = RotateBy::create(3,45);
    // auto repeatForeverAction = RepeatForever::create(action);

    // mySprite->runAction(repeatForeverAction);

    //Sequences of Actions
    // auto actionOne = RotateBy::create(3,45);
    // auto actionTwo = RotateBy::create(3,-45);
    // auto sequence = Sequence::create(actionOne,actionTwo,NULL);
    // auto repeatSequenceForever = RepeatForever::create(sequence);
    // mySprite->runAction(repeatSequenceForever);


    //Skewing using SkewTo
    // auto skewToAction = SkewTo::create(3,10,20);
    // mySprite->runAction(skewToAction);

    // //Playing Sound Effect
    // AudioEngine::preload("audio/death.mp3");
    // // AudioEngine::play2d("audio/death.mp3",false,1.0f,NULL);
    // // return true;

    // //Playing sound fx repeatedly
    // auto soundFxId = AudioEngine::play2d("audio/death.mp3",true);

    // //Calling a function from an action
    // auto soundFxStopAction = CallFunc::create([soundFxId](){
    //     AudioEngine::stop(soundFxId);
    //     CCLOG("Stopped Sound FX"); //Logging to the console
    // });

    // auto soundFxStopDelay = DelayTime::create(10); //Creating a delay action

    // auto soundFxStopSequence = Sequence::create(soundFxStopDelay,soundFxStopAction,NULL); //Scheduling a function using a delay and function sequence

    // this->runAction(soundFxStopSequence); 


    // //Single Touch Events
    // auto listener = EventListenerTouchOneByOne::create();
    // listener->setSwallowTouches(true);

    // listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
    // listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
    // listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded,this);


    // _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    

    //Multi Touch events
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan,this);
    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved,this);
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

    //Add a background image to the menu scene
    auto bgImage = Sprite::create("backgrounds/menu_screen_bg.png");
    bgImage->setPosition(Vec2(visibleSize.width / 2,visibleSize.height / 2));
    bgImage->setContentSize(Size(visibleSize.width - 20,visibleSize.height));
    this->addChild(bgImage);

    //Creating a Menu
    mainMenu = Menu::create();
    mainMenu->setPosition(Vec2(0,0));
    this->addChild(mainMenu);
   

    //Add the play button option
    auto playOption = MenuItemFont::create("Play", CC_CALLBACK_1(HelloWorld::playClicked,this));
    playOption->setPosition(Vec2(visibleSize.width / 2,visibleSize.height / 2));
    mainMenu->addChild(playOption);


    // auto m_map = TMXTiledMap::create("maps/test.tmx");
    // m_map->setPosition(Vec2(visibleSize.width / 2,visibleSize.height / 2));
    // m_map->setScale(1);
    // this->addChild(m_map);

    return true;
}

void HelloWorld::playClicked(Ref *pSender){
    CCLOG("Play Clicked");
    Director::getInstance()->pushScene(GameBoard::createScene());
}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    for(Touch *touch : touches){
        CCLOG("Touch Began at (%4.2f, %4.2f)",touch->getLocation().x,touch->getLocation().y);
    }
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    for(Touch *touch : touches){
        CCLOG("Touch Moved to (%4.2f, %4.2f)",touch->getLocation().x,touch->getLocation().y);
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    for(Touch *touch : touches){
        CCLOG("Touch Ended at (%4.2f, %4.2f)",touch->getLocation().x,touch->getLocation().y);
    }
}
