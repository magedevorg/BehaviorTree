// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute�ÿ� �ʿ��� �Ķ����
struct MBTExecuteParam
{
	// �ൿ Ʈ��
	class MBehaviorTree* BehaviorTree = nullptr;

	// ���� ���
	//int32 StartNum = 0;

	// ���� Ÿ��
	MBTExecuteType ExecuteType = MBTExecuteType::None;

	// ��� 
	MBTResult Result = MBTResult::Succeeded;
};



//------------------------------------------------------------
// ��� ���̽�
//------------------------------------------------------------
class MBTNodeBase
{
public:
	virtual ~MBTNodeBase() {}

public:
	// �ʱ�ȭ
	virtual void InitNode() {}

	// ����
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam);




protected:
	// ��� ��ȣ
	MINT32 Num = 0;
};


//------------------------------------------------------------
// ��� ���̽�
//------------------------------------------------------------
class MBTRootNode : public MBTNodeBase
{
public:
	


protected:
	// �ڽ� ���
	class MBTNode* ChildNode = nullptr;
};



//------------------------------------------------------------
// ���
//------------------------------------------------------------ 
class MBTNode : public MBTNodeBase
{
public:
	

protected:
	// �θ� ��� �߰�
	class MBTNodeBase* ParentNode = nullptr;
};
