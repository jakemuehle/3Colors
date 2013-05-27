#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "ColorNode.h"

namespace three_color
{
	class ColorBank;
}

class HelloWorld : public cocos2d::CCLayer, cocos2d::CCTargetedTouchDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
		
		bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
		void onEnter();
		void onExit();

private:
	three_color::ColorNode node[2];
	three_color::ColorBank * color_bank;

};

#endif // __HELLOWORLD_SCENE_H__
