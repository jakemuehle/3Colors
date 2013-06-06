#include "HelloWorldScene.h"
#include "ColorBank.h"
#include "ColorNode.h"
#include "ColorGrid.h"

// Test macros
//#define TEST_MAKE_SELECTION
#define TEST_SELECT

using namespace cocos2d;

HelloWorld::~HelloWorld()
{
    if(bank)
    {
        bank->release();
        bank = NULL;
    }
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    //pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    //this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    //CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    //pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    //this->addChild(pSprite, 0);
    
	bank = new three_color::ColorBank(cocos2d::CCRect(0, size.height - 400, 200, 200),4,3,3,3);
    bank->init();
	bank->setPosition(0,0);
    this->addChild( bank, 1 );

	m_pColorGrid = new three_color::ColorGrid(cocos2d::CCRect(200,0,size.width - 175, size.height), 8, 8, 3, 8, 8, 8, 8);
	m_pColorGrid->init();
	m_pColorGrid->setPosition(0,0);
	this->addChild(m_pColorGrid, 1);
    
    m_bTouchEnabled = true;
    
    return true;
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    
#ifdef TEST_MAKE_SELECTION
    
    three_color::PaletteIndex colors[] = { 2, 1, 1 };
    
    bank->makeSelection(colors, 3);
    
#endif // TEST_SELECT
    
#ifdef TEST_SELECT
    
    static bool should_select = true;
    
    if( should_select )
    {
        three_color::PaletteIndex colors[] = { 1, 0 };
        
        bank->select(colors, 1);
    }
    else
    {
        bank->deselectAll();
    }
    
    should_select = ! should_select;
    
#endif // TEST_SELECT
    
    return true;
}

void HelloWorld::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
