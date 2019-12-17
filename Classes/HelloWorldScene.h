#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // // a selector callback
    // void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    //Multi-Touch Events
    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);


    //Game Menu
    cocos2d::Menu *mainMenu;

    void playClicked(Ref *pSender);

    //Single Touch Events
    // bool onTouchBegan(cocos2d::Touch *touch,cocos2d::Event *event);
    // void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    // void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif // __HELLOWORLD_SCENE_H__
