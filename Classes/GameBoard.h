#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include "cocos2d.h"
#include <vector>

enum Direction{
    Left,
    Right,
    Up,
    Down,
    None
};

class GameBoard : public cocos2d::Scene{
    public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;   

        CREATE_FUNC(GameBoard);

        //Player Sprite
        cocos2d::Sprite *player;

        //World Layer
        cocos2d::Layer *worldLayer;

        //UI Layer
        cocos2d::Layer *uiLayer;

        //A button for UI layer
        cocos2d::Sprite *abtn;

        //B button for UI layer
        cocos2d::Sprite *bbtn;

        //Left DPAD Button for UI Layer
        cocos2d::Sprite *leftDpadBtn;

        //Right DPAD Button for UI Layer
        cocos2d::Sprite *rightDpadBtn;

        //Up DPAD Button for UI Layer
        cocos2d::Sprite *upDpadBtn;

        //Down DPAD Button for UI Layer
        cocos2d::Sprite *downDpadBtn;

        //TileMap
        cocos2d::TMXTiledMap *land_map;

        //Variables for tracking movenement
        bool moving;
        
        //Track which direction to move
        Direction movement_direction;

        //Previous Position
        cocos2d::Vec2 prevPosition;

        //Get tile coordinate
        cocos2d::Point tileCoordForPosition(cocos2d::Point);

        //Multi-Touch Events
        void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
        void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
        void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

        //Update
        void update(float) override;

        //Handle Menu Input Calls
        void btnAPressed();

        cocos2d::Vector<cocos2d::SpriteFrame *> getAnimation(const char *, int);

        //Player Animations
        cocos2d::Animation *playerWalkAnimation;
        //cocos2d::RepeatForever *walkForever;
        cocos2d::Vector<cocos2d::SpriteFrame *> playerWalkingFrames;
};

#endif // __GAME_BOARD_H__
