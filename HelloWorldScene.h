#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "GLES-Render.h"

USING_NS_CC;

#define PTM_RATIO   32.0f


struct MyContact 
{
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;

	bool operator==(const MyContact& other) const
	{
		return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
	}
};

class MyContactListener : public b2ContactListener
{
public:
	std::vector<MyContact> _contacts;
	MyContactListener()
		: _contacts()
	{}
	~MyContactListener(){}
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold){}
	virtual void PostSolve(b2Contact* contact, const b2Manifold* oldManifold){}
};

enum
{
	tag_mine = 1,
	tag_collisionBox = 2,
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuResetCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	HelloWorld()
		: m_debugDraw(PTM_RATIO)
		, m_bCreateMine(true)
		, m_bMoving(false)
		, fixture_mine(nullptr)
		, fixture_collisionBox(nullptr)
	{

	}

	~HelloWorld()
	{
	
	}

	void tick(float dt);

private:
	b2World *world_;
	b2Body *screen_body_;

	b2Fixture *fixture_mine;
	b2Fixture *fixture_collisionBox;
	MyContactListener *contactListener_;

	GLESDebugDraw m_debugDraw;

	Sprite* pSprite_mine;
	Sprite* pSprite_collisionBox;

	bool m_bCreateMine;
	bool m_bMoving;

protected:
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent){}
};

#endif // __HELLOWORLD_SCENE_H__
