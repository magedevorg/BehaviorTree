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
class MBTBlackboardDecorator : public MBTDecorator
{
public:
	MBTBlackboardDecorator(class MBTNode* inNode);

public:
	void SetKey(const MString& inKey) {
		Key = inKey;
	}

	// Ű�� ��´�
	const MString& GetKey() const {
		return Key;
	}

	void SetAbortMode(MBTFlowAbortMode inAbortMode) {
		AbortMode = inAbortMode;
	}
	
	MBTFlowAbortMode GetAbortMode() const {
		return AbortMode;
	}

	void SetBoolValue(MBOOL inValue)
	{
		BoolValue = inValue;
	}

	// ������ üũ
	MBOOL CheckCondition(class MBTBlackboard* inBlackboard);
	
protected:
	// ��� ���� üũ ����
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
	// �� ���� Ű ��
	MString Key;


	MBTFlowAbortMode AbortMode;

	//--------------------------------------------------
	// Ÿ�Կ� ���� ������
	//--------------------------------------------------
	// bool ��
	MBOOL BoolValue;
	MBTKeyOperation KeyOperation;

	// int ��
	MINT32 Int32Value;
	MBTArithmeticOperation ArithmeticOperation;
};




class MBTLoopDecorator : MBTDecorator
{

};