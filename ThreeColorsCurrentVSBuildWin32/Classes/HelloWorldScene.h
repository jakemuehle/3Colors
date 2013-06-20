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
    
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void registerWithTouchDispatcher();
    
private:
    three_color::ColorBank* bank;
	three_color::ColorGrid* m_pColorGrid;

	unsigned int m_nColors;
	unsigned int m_nRows;
	unsigned int m_nColumns;
	unsigned int m_nComboSize;
	unsigned int m_nSequences;
};

#endif // __HELLOWORLD_SCENE_H__
