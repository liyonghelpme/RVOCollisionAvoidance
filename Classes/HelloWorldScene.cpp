#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;
const float sca = 7.5f;

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
HelloWorld::~HelloWorld() {
	delete sim;
	views.clear();
}

void HelloWorld::updateSpritePosition() {
	for(int i = 0; i < views.size(); i++){
		CCSprite *sp = views[i];
		Vector2 v = sim->getAgentPosition(i);
		sp->setPosition(ccp(v.x()*sca, v.y()*sca));
	}
	
}
void HelloWorld::setPreferredVelocity() {
	for(int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i) {
		Vector2 goalVector = goals[i]-sim->getAgentPosition(i);
		if(absSq(goalVector) > 1.0f) {
			goalVector = normalize(goalVector)*5;
		}
		sim->setAgentPrefVelocity(i, goalVector);
	}
}
void HelloWorld::update(float diff) {
	passTime += diff;
	if(reachGoal())
		return;
	while(passTime >= timeStep) {
		passTime -= timeStep;
		updateSpritePosition();
		setPreferredVelocity();
		sim->doStep();
	}
}
bool HelloWorld::reachGoal() {
	for(size_t i = 0; i < sim->getNumAgents(); ++i) {
		if(absSq(sim->getAgentPosition(i)-goals[i]) > sim->getAgentRadius(i))
			return false;
	}
	return true;
}
void HelloWorld::setupScenario() {
	sim->setTimeStep(timeStep);
	sim->setAgentDefaults(3.0f, 10, 10.0f, 10.0f, 1.5f, 2.0f);
	//20*20zhi
	//¿Õ¼ä³É±ÈÀý °ë¾¶1.5 ----> °ë¾¶20
	//¿Õ¼ä200   ------> 13.3 * 200 == 2666
	//56 --- 750 °ë¾¶
	/*
	for(int i=0; i < 200; ++i){
		Vector2 p = 56.0f*RVO::Vector2(std::cos(i*2.0f*M_PI/250.0f), std::sin(i*2.0f*M_PI/250.0f));
		
		sim->addAgent(p);
		goals.push_back(-sim->getAgentPosition(i));

		CCSprite *sp = CCSprite::create("whiteBall.png");
		views.push_back(sp);
		sp->setPosition(ccp(p.x()*sca, p.y()*sca));
		CCLog("vector is %f %f", p.x()*sca, p.y()*sca);
		CCLog("goals is %f", -sim->getAgentPosition(i));
		addChild(sp, 0, i);
	}
	*/
	CCSprite *sp;
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			Vector2 p;
			p = Vector2(55+i*10, 55+j*10);
			sim->addAgent(p);
			goals.push_back(Vector2(-75, -75));
			
			sp = CCSprite::create("whiteBall.png");
			views.push_back(sp);
			sp->setPosition(ccp(p.x()*sca, p.y()*sca));
			addChild(sp, 0, i);

			p = Vector2(-55-i*10, 55+j*10);
			sim->addAgent(p);
			goals.push_back(Vector2(75, -75));
			
			sp = CCSprite::create("whiteBall.png");
			views.push_back(sp);
			sp->setPosition(ccp(p.x()*sca, p.y()*sca));
			addChild(sp, 0, i);
			
			p = Vector2(55+i*10, -55-j*10);
			sim->addAgent(p);
			goals.push_back(Vector2(-75, 75));
			
			sp = CCSprite::create("whiteBall.png");
			views.push_back(sp);
			sp->setPosition(ccp(p.x()*sca, p.y()*sca));
			addChild(sp, 0, i);
			
			p = Vector2(-55-i*10, -55-j*10);
			sim->addAgent(p);
			goals.push_back(Vector2(75, 75));
			sp = CCSprite::create("whiteBall.png");
			views.push_back(sp);
			sp->setPosition(ccp(p.x()*sca, p.y()*sca));
			addChild(sp, 0, i);
		}
	}
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
	passTime = 0;
	timeStep = 0.016f;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	setPosition(ccp(visibleSize.width/2, visibleSize.height/2));

	sim = new RVO::RVOSimulator();
	setupScenario();
	scheduleUpdate();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

   
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
