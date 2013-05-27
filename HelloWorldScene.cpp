#include "HelloWorldScene.h"
#include "AppMacros.h"

#include "ColorNodeFrames.h"
#include "ColorBank.h"

USING_NS_CC;

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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("HelloWorld.png");

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "Paws.plist" );

    CCSprite* pSprite = CCSprite::createWithTexture( pTexture );
    CCSprite* pawSprite = CCSprite::createWithSpriteFrameName("White Paw.png");

    {
      ccColor3B pawColor = { 255, 255, 0 };
      pawSprite->setColor( pawColor );
    }

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    this->addChild( pawSprite, 1 );

    CCRect boundingBox( pawSprite->boundingBox() );

    CCTouch* touch = new CCTouch();
    touch->setTouchInfo( 0, visibleSize.width/4 + origin.x, visibleSize.height/4*3 + origin.y );
    CCPoint touchPoint( visibleSize.width/4 + origin.x, visibleSize.height/4*3 + origin.y );

    bool isInBox = boundingBox.containsPoint( touchPoint );
    touch->release();
    
		{
			node[0].init();
			node[0].setColor( 1 );
			node[0].setPosition( CCPoint( 250.0f, 250.0f ) );
			node[0].setParent( this, 2 );

			node[1].init();
			node[1].setColor( 0 );
			node[1].setPosition( CCPoint( visibleSize.width/4*3 + origin.x, visibleSize.height/4*3 + origin.y ) );
			node[1].setParent( this, 2 );

			three_color::ColorNode node_tmp;
			node_tmp.init();
			node_tmp.setColor( 2 );
			node_tmp.setPosition( CCPoint( visibleSize.width/4 + origin.x, visibleSize.height/4*3 + origin.y ) );
			node_tmp.setParent( this, 2 );
			node_tmp.removeParent( this );
		}

		{
			color_bank = new three_color::ColorBank( CCRect( 40, 100, 100.0f, 120.0f ), 4, 3, 3, 3 );
			color_bank->init();
			addChild( color_bank );
		}

    return true;
}

void HelloWorld::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate( (CCTargetedTouchDelegate*)this, 0, true );
	CCLayer::onEnter();
}

void HelloWorld::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate( (CCTargetedTouchDelegate*)this );
	CCLayer::onExit();
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pPoint = pTouch->getLocationInView();
	pPoint.y = CCDirector::sharedDirector()->getVisibleSize().height - pPoint.y;

	for( three_color::ColorNode * iter = node; iter != node+2; ++iter )
	{
		if( iter->isOnSprite( pPoint ) )
		{
			iter->toggleSelect();
		}
	}

	return false;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
