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

MBOOL MBTBlackboardDecorator::CheckCondition(class MBTBlackboard* inBlackboard)
{
	// Ÿ�Ժ� ó��
	MBTBlackboardValueType valueType = inBlackboard->GetValueType(Key);
	switch (valueType)
	{
	case MBTBlackboardValueType::Bool:
	{
		// ���� ��´�
		MBOOL value = inBlackboard->GetBoolValue(Key);

		MBOOL check = MFALSE;
		if (MBTKeyOperation::Set == KeyOperation) {
			check = MTRUE;
		}

		if (check == value) {
			return TRUE;
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