#include "HelloWorldScene.h"


void MyContactListener::BeginContact( b2Contact* contact )
{

}

void MyContactListener::EndContact( b2Contact* contact )
{

}


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
    this->addChild(menu, 5);

	auto resetItem = MenuItemImage::create(
		"CloseSelected.png",
		"CloseNormal.png",
		CC_CALLBACK_1(HelloWorld::menuResetCallback, this));

	resetItem->setPosition(Vec2(origin.x + closeItem->getContentSize().width/2 ,
		origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	menu = Menu::create(resetItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	world_ = new b2World(gravity);
	world_->SetAllowSleeping(false);
    
	world_->SetDebugDraw(&m_debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_centerOfMassBit;
	flags += b2Draw::e_pairBit;
	m_debugDraw.SetFlags(flags);

	/**
     충돌 이벤트를 받기 위한 이벤트 등록을 한다
     */
	contactListener_ = new MyContactListener();
	world_->SetContactListener(contactListener_);

	b2BodyDef screen_body_def;
	screen_body_def.position.Set(0.0f, 0.0f);
	screen_body_ = world_->CreateBody(&screen_body_def);


	//충돌 영역 생성.
	Sprite* pSprite_collisionBox = Sprite::create("Icon.png");
	pSprite_collisionBox->setPosition( visibleSize.width * 0.5, visibleSize.height * 0.5);
	pSprite_collisionBox->setScale( 300/pSprite_collisionBox->getContentSize().width );
	pSprite_collisionBox->setTag(tag_collisionBox);
	this->addChild(pSprite_collisionBox);


	//충돌영역 FixtureDef 생성
	b2PolygonShape CollisionBox_shape;
	CollisionBox_shape.SetAsBox(pSprite_collisionBox->getBoundingBox().size.width/PTM_RATIO/2,
		pSprite_collisionBox->getBoundingBox().size.height/PTM_RATIO/2);

	b2Vec2 move(pSprite_collisionBox->getPosition().x/PTM_RATIO, pSprite_collisionBox->getPosition().y/PTM_RATIO);
	for(int32 i=0;i<CollisionBox_shape.GetVertexCount();i++)
	{
		CollisionBox_shape.m_vertices[i] = CollisionBox_shape.m_vertices[i] + move;
	}
	CollisionBox_shape.m_centroid+=move;
	
	b2FixtureDef CollisionBox_Fixture_Def;
	CollisionBox_Fixture_Def.shape = &CollisionBox_shape;
	CollisionBox_Fixture_Def.isSensor = true;
	fixture_collisionBox = screen_body_->CreateFixture(&CollisionBox_Fixture_Def);
	

	this->schedule(schedule_selector(HelloWorld::tick));

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

void HelloWorld::menuResetCallback( cocos2d::Ref* pSender )
{

}

void HelloWorld::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	world_->DrawDebugData();
	kmGLPopMatrix();
}

void HelloWorld::tick( float dt )
{

}
