// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTDecorator.h"
#include "MBTNode.h"
#include "MBehaviorTree.h"
#include "MBTBlackboard.h"

//-----------------------------------------------------------------
// MBTDecorator
//-----------------------------------------------------------------
MBTDecorator::MBTDecorator(class MBTNode* inNode)
	: Node(inNode)
{
}



//-----------------------------------------------------------------
// MBTBlackboardDecorator
//-----------------------------------------------------------------
MBTBlackboardDecorator::MBTBlackboardDecorator(class MBTNode* inNode)
	: MBTDecorator(inNode)
{	
}

void MBTBlackboardDecorator::CheckBool(const MString& inKey, MBOOL inValue)
{
	Key = inKey;
	BoolValue = inValue;
}



MBOOL MBTBlackboardDecorator::CheckCondition(class MBTBlackboard* inBlackboard)
{
	// 타입별 처리
	MBTBlackboardValueType valueType = inBlackboard->GetValueType(Key);
	switch (valueType)
	{
	case MBTBlackboardValueType::Bool:
	{
		// 값을 얻는다
		MBOOL value = inBlackboard->GetBoolValue(Key);

		if (value == BoolValue) {
			return MTRUE;
		}

	} break;
	case MBTBlackboardValueType::Int32:
	{
		MINT32 value = inBlackboard->GetInt32Value(Key);
		return CheckArithmeticOperation(value, Int32Value);

	} break;
	}

	MASSERT(MFALSE);
	return MFALSE;
}