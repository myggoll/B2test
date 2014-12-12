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


	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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
	//Reset My box

	m_bCreateMine = true;
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
	if( fixture_mine && fixture_mine->GetBody() )
	{
		b2Body *b = fixture_mine->GetBody();
		Sprite *sprite = (Sprite *)b->GetUserData();
		sprite->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
			b->GetPosition().y * PTM_RATIO));
		sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
	}
}

bool HelloWorld::onTouchBegan( Touch *pTouch, Event *pEvent )
{
	Vec2 touchPos = this->convertTouchToNodeSpace(pTouch);
	//log("Touch Began %.2f, %.2f", touchPos.x, touchPos.y);

	if( m_bCreateMine )
	{
		pSprite_mine = Sprite::create("Icon.png");
		pSprite_mine ->setPosition(touchPos);
		pSprite_mine->setTag(tag_mine);
		this->addChild(pSprite_mine);

		b2BodyDef block_body_def;
		block_body_def.type = b2_staticBody;
		block_body_def.position.Set(pSprite_mine->getPositionX()/PTM_RATIO,
			pSprite_mine->getPositionY()/PTM_RATIO);
		block_body_def.userData = pSprite_mine;

		b2Body *block_body = world_->CreateBody(&block_body_def);

		b2PolygonShape block_polygon;
		block_polygon.SetAsBox(pSprite_mine->getContentSize().width/PTM_RATIO/2,
			pSprite_mine->getContentSize().height/PTM_RATIO/2);
		b2FixtureDef block_fixture_def;
		block_fixture_def.shape = &block_polygon;

		fixture_mine = block_body->CreateFixture(&block_fixture_def);

		m_bCreateMine = false;
	}

	if( pSprite_mine->getBoundingBox().containsPoint(touchPos) )
	{
		m_bMoving = true;
	}

	return true;
}

void HelloWorld::onTouchMoved( Touch *pTouch, Event *pEvent )
{
	Vec2 touchPos = this->convertTouchToNodeSpace(pTouch);
	//log("Touch Moved %.2f, %.2f", touchPos.x, touchPos.y);

	if( m_bMoving && fixture_mine )
	{
		fixture_mine->GetBody()->SetTransform( b2Vec2(touchPos.x/PTM_RATIO, touchPos.y/PTM_RATIO), fixture_collisionBox->GetBody()->GetAngle());
	}
}

void HelloWorld::onTouchEnded( Touch *pTouch, Event *pEvent )
{
	log("Touch Ended");
	m_bMoving = false;
}
