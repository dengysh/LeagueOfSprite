#include "GameScene.h"

GameScene* GameScene::createScene()
{
	auto scene = new GameScene();
	scene->initWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -200));
	scene->autorelease();
	return scene;
}

bool GameScene::init() {
	if (!Scene::init())
	{
		return false;
	}

	//play background music
	//SoundManager::getInstance()->gameSceneinit();

	this->initWithPhysics();
	this->getPhysicsWorld()->setGravity(Vec2(0, -400));
	
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//add map
	auto spbk = Sprite::create("GameScene/scene1.png");
	//spbk->setPosition(Point::ZERO);
	//spbk->setAnchorPoint(Point::ZERO);
	spbk->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(spbk);

	//add player
	this->addChild(PlayerController::getInstance(), 1);
	this->addChild(MonsterController::getInstance(), 1);

	auto ground = Node::create();
	auto body = PhysicsBody::createEdgeSegment(Vec2(0, 100), Vec2(1280, 100));
	body->setTag(Constant::getEdgeTag());
	ground->setPhysicsBody(body);
	//ground->getPhysicsBody()->setDynamic(false);
	ground->getPhysicsBody()->setGravityEnable(false);
	ground->getPhysicsBody()->setDynamic(false);
	ground->getPhysicsBody()->setCategoryBitmask(0x000000FF);
	ground->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
	ground->getPhysicsBody()->setContactTestBitmask(0x0000000F);

	this->addChild(ground, 1);

	auto leftwall = Node::create();
	auto wallbody = PhysicsBody::createEdgeSegment(Vec2(0, 100), Vec2(0, 640));
	wallbody->setTag(Constant::getEdgeTag());
	leftwall->setPhysicsBody(wallbody);
	//ground->getPhysicsBody()->setDynamic(false);
	leftwall->getPhysicsBody()->setGravityEnable(false);
	leftwall->getPhysicsBody()->setDynamic(false);
	leftwall->getPhysicsBody()->setCategoryBitmask(0x000000FF);
	leftwall->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
	leftwall->getPhysicsBody()->setContactTestBitmask(0x0000000F);

	this->addChild(leftwall, 1);

	auto rightwall = Node::create();
	auto rightwallbody = PhysicsBody::createEdgeSegment(Vec2(1050, 100), Vec2(1050, 640));
	rightwallbody->setTag(Constant::getEdgeTag());
	rightwall->setPhysicsBody(rightwallbody);
	//ground->getPhysicsBody()->setDynamic(false);
	rightwall->getPhysicsBody()->setGravityEnable(false);
	rightwall->getPhysicsBody()->setCategoryBitmask(0x000000FF);
	rightwall->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
	rightwall->getPhysicsBody()->setContactTestBitmask(0x0000000F);

	this->addChild(rightwall, 1);

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	listener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool GameScene::onContactBegin(PhysicsContact& contact) {
	if (contact.getShapeB()->getBody() == NULL || contact.getShapeB()->getBody()->getNode() == NULL
		|| contact.getShapeA()->getBody() == NULL || contact.getShapeA()->getBody()->getNode() == NULL) {
		return false;
	}


	int tagA = contact.getShapeA()->getBody()->getTag();
	int tagB = contact.getShapeB()->getBody()->getTag();
	log("tagA: %d", tagA);
	log("tagB: %d", tagB);

	//weapon hit ground
	if (tagA == Constant::getEdgeTag() && tagB == Constant::getArrowTag()) {
		contact.getShapeB()->getBody()->getNode()->removeFromParentAndCleanup(true);
	}
	else if (tagB == Constant::getEdgeTag() && tagA == Constant::getArrowTag()) {
		contact.getShapeA()->getBody()->getNode()->removeFromParentAndCleanup(true);
	}

	//weapon hit monster
	if (tagA == Constant::getMonsterTag() && tagB == Constant::getArrowTag()) {
		contact.getShapeB()->getBody()->getNode()->removeFromParentAndCleanup(true);
		MonsterController::getInstance()->getMonster()->beingHit();
		SimpleAudioEngine::getInstance()->playEffect("Sound/hit.wav");
	}
	else if (tagB == Constant::getMonsterTag() && tagA == Constant::getArrowTag()) {
		contact.getShapeA()->getBody()->getNode()->removeFromParentAndCleanup(true);
		MonsterController::getInstance()->getMonster()->beingHit();
		SimpleAudioEngine::getInstance()->playEffect("Sound/hit.wav");
	}

	//player land ground
	if (tagA == Constant::getPlayerTag() && tagB == Constant::getEdgeTag()) {
		PlayerController::getInstance()->setOnAir(false);
		PlayerController::getInstance()->setSecondJump(false);
		log("OnAir: %d", PlayerController::getInstance()->getOnAir());
	} else if (tagB == Constant::getPlayerTag() && tagA == Constant::getEdgeTag()) {
		PlayerController::getInstance()->setOnAir(false);
		PlayerController::getInstance()->setSecondJump(false);
		log("OnAir: %d", PlayerController::getInstance()->getOnAir());
	}

	return true;
}

void GameScene::onContactSeparate(PhysicsContact& contact) {

}