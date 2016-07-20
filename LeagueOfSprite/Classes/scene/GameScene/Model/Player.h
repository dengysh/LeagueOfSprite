#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

#include ".\scene\GameScene\Model\Monster.h"
#include ".\public\Constant.h"

USING_NS_CC;

class  Player: public cocos2d::Sprite
{
public:
	static Player* getInstance();
	CREATE_FUNC(Player);
	virtual bool init();

	void releasePlayer();


	//Ѫ��
	void setHP(int num);
	int getHP();
	//����
	void setMP(int num);
	int getMP();
	//����ֵ
	void setExp(int num);
	int getExp();
	//����
	void setScore(int num);
	int getScore();
	
	void run(int direction);
	void jump();
	void stopRunning();
	void shoot();
	void stopShooting();

	Animate* getHealAnimate();
	void playHealAnimate();
	void healAnimateEnded();

	Animate* getShieldAnimate();
	void playShieldAnimate();
	void shieldAnimateEnded();

private:
	static Player* player_;
	int hp_num;
	int mp_num;
	int score;
	int exp;

	Sprite* stand;
	Sprite* runAnimate;
	Sprite* shootAnimate;

	Sprite *healAnimate;
	Sprite *shieldAnimate;

};
#endif // !_PLAYER_H_