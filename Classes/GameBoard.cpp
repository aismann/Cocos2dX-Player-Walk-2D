#include "GameBoard.h"
#include "audio/include/AudioEngine.h"
#include <vector>

USING_NS_CC;

Scene* GameBoard::createScene(){
    return GameBoard::create();
}

//static void problemLoading(const char* filename){
//    printf("Error while loading: %s\n", filename);
//    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameBoardScene.cpp\n");
//}

bool GameBoard::init(){
    scheduleUpdate();

    if ( !Scene::init() ){
        return false;
    }


    //Add the character spritesheet to the sprite cache
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Characters/Walk/walk.plist");



    auto visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameBoard::onTouchesBegan,this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameBoard::onTouchesMoved,this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameBoard::onTouchesEnded,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    GameBoard::land_map = TMXTiledMap::create("maps/world_map_quadrant_one.tmx");
    GameBoard::land_map->setPosition(Vec2(0,0));
    GameBoard::land_map->setName("world_tilemap");
    //GameBoard::land_map->setScale(1);
    GameBoard::worldLayer = Layer::create();
    GameBoard::moving = false;
    GameBoard::movement_direction = Direction::None;
    GameBoard::worldLayer->addChild(GameBoard::land_map);
    GameBoard::worldLayer->setPosition(Vec2(0,0));
    GameBoard::worldLayer->setScale(1);
    GameBoard::prevPosition = GameBoard::worldLayer->getPosition();
    this->addChild(worldLayer);

#if !(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) && !(CC_TARGET_PLATFORM == CC_PLATFORM_MAC) && !(CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    GameBoard::uiLayer = Layer::create();
    this->addChild(uiLayer);

    //UI Layer - A Button
    GameBoard::abtn = Sprite::create("controls/transparentDark/transparentDark34.png");
    GameBoard::abtn->setPosition(Vec2(visibleSize.width - GameBoard::abtn->getBoundingBox().size.width - 30,60));
    GameBoard::abtn->setScale(0.5);

    //UI Layer - B Button
    GameBoard::bbtn = Sprite::create("controls/transparentDark/transparentDark35.png");
    GameBoard::bbtn->setPosition(Vec2(visibleSize.width - GameBoard::bbtn->getBoundingBox().size.width / 2,60));
    GameBoard::bbtn->setScale(0.5);

    //UI Layer - Left DPAD Button
    GameBoard::leftDpadBtn = Sprite::create("controls/transparentDark/transparentDark22.png");
    GameBoard::leftDpadBtn->setPosition(Vec2(55,89));
    GameBoard::leftDpadBtn->setScale(0.5);

    //UI Layer - Right DPAD Button
    GameBoard::rightDpadBtn = Sprite::create("controls/transparentDark/transparentDark23.png");
    GameBoard::rightDpadBtn->setPosition(Vec2(GameBoard::leftDpadBtn->getBoundingBox().origin.x + 20 + GameBoard::rightDpadBtn->getBoundingBox().size.width,GameBoard::leftDpadBtn->getBoundingBox().origin.y + GameBoard::leftDpadBtn->getBoundingBox().size.height / 2));
    GameBoard::rightDpadBtn->setScale(0.5);

    //UI Layer - Up DPAD Button
    GameBoard::upDpadBtn = Sprite::create("controls/transparentDark/transparentDark24.png");
    GameBoard::upDpadBtn->setPosition(Vec2(GameBoard::leftDpadBtn->getBoundingBox().origin.x + (GameBoard::leftDpadBtn->getBoundingBox().size.width / 2) + (GameBoard::rightDpadBtn->getBoundingBox().origin.x - GameBoard::leftDpadBtn->getBoundingBox().origin.x) / 2, 115));
    GameBoard::upDpadBtn->setScale(0.5);

    //UI Layer - Down DPAD Button
    GameBoard::downDpadBtn = Sprite::create("controls/transparentDark/transparentDark25.png");
    GameBoard::downDpadBtn->setPosition(Vec2(GameBoard::upDpadBtn->getBoundingBox().origin.x + GameBoard::upDpadBtn->getBoundingBox().size.width / 2,GameBoard::upDpadBtn->getBoundingBox().origin.y - 30));
    GameBoard::downDpadBtn->setScale(0.5);

    GameBoard::uiLayer->addChild(GameBoard::abtn);
    GameBoard::uiLayer->addChild(GameBoard::bbtn);
    GameBoard::uiLayer->addChild(GameBoard::leftDpadBtn);
    GameBoard::uiLayer->addChild(GameBoard::rightDpadBtn);
    GameBoard::uiLayer->addChild(GameBoard::upDpadBtn);
    GameBoard::uiLayer->addChild(GameBoard::downDpadBtn);
#endif

    //Retrieve the fighting stance animation frames from the sprite sheet
    GameBoard::playerWalkingFrames = getAnimation("walk_%05d.png", 47);
    
    // Create the Player Sprite using the first frame from the fighting stance animation
    GameBoard::player = Sprite::createWithSpriteFrame(GameBoard::playerWalkingFrames.front());
    GameBoard::player->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));
    GameBoard::player->setScale(0.25);
    this->addChild(GameBoard::player);


    return true;
}

void GameBoard::btnAPressed(){
    CCLOG("Pressing A Button");
}

Point GameBoard::tileCoordForPosition(Point position){
    float playerXOffset = (Director::getInstance()->getVisibleSize().width / 2);
    float playerYOffset = (Director::getInstance()->getVisibleSize().height / 2) + (GameBoard::player->getTextureRect().size.height / 3);
    Point playerPos = Point(playerXOffset,playerYOffset);
    auto worldPos = GameBoard::worldLayer->convertToNodeSpace(playerPos);
    int x = worldPos.x / GameBoard::land_map->getTileSize().width;
    int y = worldPos.y / GameBoard::land_map->getTileSize().height;
    y = GameBoard::land_map->getMapSize().height - y;
    auto ret = Point(x,y);
    //CCLOG("(%d,%d)",x,y);
    return ret; 
}

cocos2d::Vector<SpriteFrame *> GameBoard::getAnimation(const char *format, int count){
    auto spritecache = SpriteFrameCache::getInstance();
    cocos2d::Vector<SpriteFrame *> animFrames;
    char frameName[100];
    for(int i = 1; i <= count; i++){
        sprintf(frameName, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(frameName));
    }
    return animFrames;
}

void GameBoard::update(float delta) {
    //Check for tilemap collisions 
    auto collisionLayer = GameBoard::land_map->getLayer("Forts");
    //CCLOG("%s",collisionLayer->getLayerName().c_str());
    Point tileCoord = this->tileCoordForPosition(GameBoard::player->getTextureRect().origin);
    int tileGid = collisionLayer->getTileGIDAt(tileCoord);
    Value properties = GameBoard::land_map->getPropertiesForGID(tileGid);
    if (!properties.isNull()){
        ValueMap dict = properties.asValueMap();
        Value collision = dict["Collidable"];
        if (!collision.isNull()) {
            CCLOG("Colliding with Fort");
            GameBoard::worldLayer->stopAllActions();
            GameBoard::player->stopAllActions();
            GameBoard::worldLayer->setPosition(GameBoard::prevPosition);
        }else{
            GameBoard::prevPosition = GameBoard::worldLayer->getPosition();
        }
    }else{
        GameBoard::prevPosition = GameBoard::worldLayer->getPosition();
    }

    //Handle Player Movement
    int movement_amount = 1.5;
    if(GameBoard::moving){
         //Generate an animation using the fighting stance animation frames
        switch(GameBoard::movement_direction){
            case Direction::Up:                
                //GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,-90.0)),Animate::create(GameBoard::playerWalkAnimation),NULL));
                GameBoard::worldLayer->runAction(MoveBy::create(0.1,Vec3(0,-movement_amount,0)));
                break;
            case Direction::Down:
                //GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,90)),Animate::create(GameBoard::playerWalkAnimation),NULL));
                GameBoard::worldLayer->runAction(MoveBy::create(0.1,Vec3(0,movement_amount,0)));
                break;
            case Direction::Left:
                GameBoard::worldLayer->runAction(MoveBy::create(0.1,Vec3(movement_amount,0,0)));
                break;
            case Direction::Right:
                GameBoard::worldLayer->runAction(MoveBy::create(0.1,Vec3(-movement_amount,0,0)));
                break;
            default: 
                break;
        }
    }
}

void GameBoard::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    for(Touch *touch : touches){
        auto location = touch->getLocation();
        if(GameBoard::abtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            CCLOG("Pressing A Button");
        }else if(GameBoard::bbtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            CCLOG("Pressing B Button");
        }else if(GameBoard::leftDpadBtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            GameBoard::playerWalkAnimation = Animation::createWithSpriteFrames(GameBoard::playerWalkingFrames, 1.0f/47);
            GameBoard::moving = true;
            GameBoard::movement_direction = Direction::Left;
            GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,-180)),Repeat::create(Animate::create(GameBoard::playerWalkAnimation),1000),NULL));
        }else if(GameBoard::upDpadBtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            GameBoard::playerWalkAnimation = Animation::createWithSpriteFrames(GameBoard::playerWalkingFrames, 1.0f/47);
            GameBoard::moving = true;
            GameBoard::movement_direction = Direction::Up;    
            GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,-90)),Repeat::create(Animate::create(GameBoard::playerWalkAnimation),1000),NULL));
        }else if(GameBoard::rightDpadBtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            GameBoard::playerWalkAnimation = Animation::createWithSpriteFrames(GameBoard::playerWalkingFrames, 1.0f/47);
            GameBoard::moving = true;
            GameBoard::movement_direction = Direction::Right;
            GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,0)),Repeat::create(Animate::create(GameBoard::playerWalkAnimation),1000),NULL));
        }else if(GameBoard::downDpadBtn->getBoundingBox().containsPoint(GameBoard::convertToWorldSpace(location))){
            GameBoard::playerWalkAnimation = Animation::createWithSpriteFrames(GameBoard::playerWalkingFrames, 1.0f/47);
            GameBoard::moving = true;
            GameBoard::movement_direction = Direction::Down;
            GameBoard::player->runAction(Spawn::create(RotateTo::create(0.2,Vec3(0,0,90)),Repeat::create(Animate::create(GameBoard::playerWalkAnimation),1000),NULL));
        }else{
            //CCLOG("Not Touching UI Controls");
        }
    }
}

void GameBoard::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    for(Touch *touch : touches){
        CCLOG("Touch Moved to (%4.2f, %4.2f)",touch->getLocation().x,touch->getLocation().y);
    }
}

void GameBoard::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){
    GameBoard::moving = false;
    GameBoard::player->stopAllActions();
    //Stop all world movement
    for(Touch *touch : touches){
        GameBoard::playerWalkAnimation = Animation::createWithSpriteFrames(GameBoard::playerWalkingFrames,1.0f/1);
        GameBoard::player->runAction(Animate::create(GameBoard::playerWalkAnimation));
    }
}
