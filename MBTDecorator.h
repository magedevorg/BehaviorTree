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
	// 조건을 체크
	MBOOL CheckCondition(class MBTBlackboard* inBlackboard);
	
	// 키를 얻는다
	const MString& GetKey() {
		return Key;
	}


protected:
	// 산술 연산 체크 로직
	template <typename T>
	MBOOL CheckArithmeticOperation(const T& inBlackboardValue, const T& inCheckValue)
	{
		switch (ArithmeticOperation)
		{
		case MBTArithmeticOperation::Equal:
			return inBlackboardValue == inCheckValue;
		case MBTArithmeticOperation::NotEqual:
			return inBlackboardValue != inCheckValue;

		case MBTArithmeticOperation::Greater:
			return inCheckValue < inBlackboardValue;
		case MBTArithmeticOperation::GreaterOrEqual:
			return inCheckValue <= inBlackboardValue;

		case MBTArithmeticOperation::Less:
			return inBlackboardValue < inCheckValue;
		case MBTArithmeticOperation::LessOrEqual:
			return inBlackboardValue <= inCheckValue;
		}

		MASSERT(MFALSE);
		return MFALSE;
	}


protected:
	// 블랙 보드 키 값
	MString Key;

	//--------------------------------------------------
	// 타입에 따른 설정값
	//--------------------------------------------------
	// bool 형
	MBOOL BoolValue;
	MBTKeyOperation KeyOperation;

	// int 형
	MINT32 Int32Value;
	MBTArithmeticOperation ArithmeticOperation;
};




class MBTLoopDecorator : MBTDecorator
{

};