// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"
#include "MString.h"


class MBTDecorator
{
public:
	MBTDecorator(class MBTNode* inNode);
	virtual ~MBTDecorator() {}

public:
	// 초기화
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree) {}

	// 소유 노드를 얻는다
	class MBTNode* GetNode() {
		return Node;
	}

protected:
	// 소유 노드
	class MBTNode* Node;
};




//-----------------------------------------------------------
// 블랙보드의 특정 값이 변경된경우 호출
//-----------------------------------------------------------
class MBTBlackboardDecorator : MBTDecorator
{
public:
	MBTBlackboardDecorator(class MBTNode* inNode);

public:
	// 초기화
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree) override;

public:
	const MString& GetKey() {
		return Key;
	}

protected:
	MString Key;
};




class MBTLoopDecorator : MBTDecorator
{

};