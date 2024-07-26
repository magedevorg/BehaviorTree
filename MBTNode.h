// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute�ÿ� �ʿ��� �Ķ����
struct MBTExecuteParam
{
	// ������ Ʈ��
	class MBehaviorTree* BehaviorTree = nullptr;

	// ���� ���
	int32 StartNum = 0;

	// ���� Ÿ��
	MBTExecuteType ExecuteType = MBTExecuteType::None;
};





//------------------------------------------------------------
// Behavior Tree �ֻ��� ���
// - ��� ����(��ȣ, ���� ��)�� �⺻ �������̽��� ������
//------------------------------------------------------------
class MBTNode
{
public:
	MBTNode() {}
	virtual ~MBTNode();

public:
	//-----------------------------------------------------------
	// �ʱ�ȭ
	//-----------------------------------------------------------
	// ��ȣ �ʱ�ȭ
	virtual void InitNum(MINT32& inBase) {
		Num = inBase++;
	}

	// ���� �ʱ�ȭ
	virtual void InitDepth(MINT32& inBase) {
		Depth = inBase++;
	}

	// ���ڷ��̴� �ʱ�ȭ
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree);


	//-----------------------------------------------------------
	// ����Լ�
	//-----------------------------------------------------------
	// ����
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) { return true; }

	// ���ڷ����� �߰�
	template<typename T>
	T* AddDecorator()
	{
		T* decorator = new T(this);
		DecoratorList.push_back(decorator);
	}


	//-----------------------------------------------------------
	// Getter
	//-----------------------------------------------------------
	// ��ȣ�� ��´�
	MINT32 GetNum() const {
		return Num;
	}

	MINT32 GetDepth() const {
		return Depth;
	}

protected:
	// ��� ��ȣ
	MINT32 Num = 0;

	// Ʈ�� ����
	MINT32 Depth = 0;

	// ���ڷ����� ����Ʈ
	std::list<class MBTDecorator*> DecoratorList;
};




//------------------------------------------------------------
// ������ ���
// - �ڽ� ��� ����
//------------------------------------------------------------ 
class MBTCompositeNode : public MBTNode
{
public:
	MBTCompositeNode() {}
	virtual ~MBTCompositeNode();

public:
	// �ʱ�ȭ
	virtual void InitNum(int32& inBase) override;
	virtual void InitDepth(int32& inBase) override;

	// ���ڷ��̴� �ʱ�ȭ
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree) override;

	template<typename T>
	T* AddChildNode()
	{
		T* childNode = new T();
		childNode->InitNode();
		ChildNodeList.insert(childNode);
		return childNode;
	}

protected:
	// �ڽ� ��� ����Ʈ
	std::list<class MBTNode*> ChildNodeList;
};


//------------------------------------------------------------
// ������ ���
// ���������� �����ϴ� Failed/InProgress/Abort�϶� ����
//------------------------------------------------------------
class MBTSequenceNode : public MBTCompositeNode
{
public:
	MBTSequenceNode() {}

public:
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;
};



//------------------------------------------------------------
// ������ ���
// ���������� �����ϴ� Succeeded/InProgress/Abort�϶� ����
//------------------------------------------------------------
class MBTSelectorNode : public MBTCompositeNode
{
public:
	MBTSelectorNode() {}

public:
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;
};




//------------------------------------------------------------
// �۾� ���
//------------------------------------------------------------ 
class MBTTaskNode : public MBTNode
{
public:
	MBTTaskNode() {}

public:
	// ��� ����
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;


	// ��� ���� ó��
	virtual MBTResult UpdateTaskNode(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}

protected:
	// �۾� ����
	virtual MBTResult ExecuteTaskNode(class MBehaviorTree* inBehaviorTree) {
		return MBTResult::Succeeded;
	}
};

