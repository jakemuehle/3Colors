#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

namespace three_color
{
    class ColorBank;
	class ColorGrid;
}

class HelloWorld : public cocos2d::CCLayer
{
public:
    ~HelloWorld();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    void registerWithTouchDispatcher();
    
private:
    three_color::ColorBank* bank;
	three_color::ColorGrid* m_pColorGrid;
};

#endif // __HELLOWORLD_SCENE_H__