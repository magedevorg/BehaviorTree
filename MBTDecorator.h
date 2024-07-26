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
	// �ʱ�ȭ
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree) {}

	// ���� ��带 ��´�
	class MBTNode* GetNode() {
		return Node;
	}

protected:
	// ���� ���
	class MBTNode* Node;
};




//-----------------------------------------------------------
// �������� Ư�� ���� ����Ȱ�� ȣ��
//-----------------------------------------------------------
class MBTBlackboardDecorator : MBTDecorator
{
public:
	MBTBlackboardDecorator(class MBTNode* inNode);

public:
	// �ʱ�ȭ
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