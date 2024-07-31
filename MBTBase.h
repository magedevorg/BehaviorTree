// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MPrerequisites.h"


//----------------------------------------------------------------
// BT ó�� ���
// ����ڰ� �����ϴ� ���
//----------------------------------------------------------------
enum class MBTResult : MUINT16
{
	Succeeded = 0,		// ����
	Failed,				// ����	
	InProgress,			// ������
	Abort,				// �ߴ�
};


//----------------------------------------------------------------
// Execute ���
// MBTResult�� ������ �÷��� + �߰����� ó���� ���� �÷��װ� �ִ�
//----------------------------------------------------------------
enum class MBTExecuteResult : MUINT16
{
	Succeeded = 0,		// ����
	Failed,				// ����	
	InProgress,			// ������
	Abort,				// �ߴ�

	// �߰����� ó�� �÷���
	None,
	Skip,
};


// ���� Ÿ��
enum class MBTExecuteType : MUINT16
{
	Play = 0,
	Finish,
};


// ���� ���� Ÿ��
enum class MBTExecuteReturnType : MUINT16
{
	None = 0,
	Skip,
};



// ������ �� Ÿ��
enum class MBTBlackboardValueType : MUINT16
{
	None = 0,
	Int32,
	Bool,
	Vector3,
};


// �����Ǿ�����
enum class MBTKeyOperation : MUINT16
{
	Set,				
	NotSet
};


// ��� ����
enum class MBTArithmeticOperation : uint8
{
	Equal,		
	NotEqual,
	Less,		
	LessOrEqual,
	Greater,	
	GreaterOrEqual,
};




enum class MBTFlowAbortMode : uint8
{
	None				UMETA(DisplayName = "Nothing"),
	LowerPriority		UMETA(DisplayName = "Lower Priority"),
	Self				UMETA(DisplayName = "Self"),
	Both				UMETA(DisplayName = "Both"),
};
