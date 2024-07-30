// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute�ÿ� �ʿ��� �Ķ����
struct MBTExecuteParam
{
	// ������ Ʈ��
	class MBehaviorTree* BehaviorTree = nullptr;

	//-------------------------------------------------------
	// ���� ����
	//-------------------------------------------------------
	// ������ 
	MINT32 ExecuteNodeNum = 0;
	MBTExecuteResult ExecuteNodeResult = MBTExecuteResult::None;
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
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) {
		return MBTExecuteResult::Succeeded;
	}

	// ��� ����
	virtual MBTResult Update(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}
	

	//-----------------------------------------------------------
	// ���ڷ����� �߰�
	//-----------------------------------------------------------
	class MBTBlackboardDecorator* AddBlackboardDecorator();

	
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
	// �ش� ��尡 ����ɼ� �ִ��� üũ
	MBOOL CheckExecuteCondition(const MBTExecuteParam& inParam);

	
	

protected:
	// ��� ��ȣ
	MINT32 Num = 0;

	// Ʈ�� ����
	MINT32 Depth = 0;

	//--------------------------------------------------------------
	// ���ڷ����� ����Ʈ
	//--------------------------------------------------------------
	// ������ ���ڷ�����
	std::list<class MBTBlackboardDecorator*> BlackboardDecoratorList;
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
		//childNode->InitNode();
		ChildNodeList.push_back(childNode);
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
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;
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
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;
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
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;

	// ��� ����
	virtual MBTResult Update(class MBehaviorTree* inBehaviorTree, float inDelta) override;

protected:
	// �۾� ����
	virtual MBTResult ExecuteTaskNode(class MBehaviorTree* inBehaviorTree) {
		return MBTResult::Succeeded;
	}

	// ��� ���� ó��
	virtual MBTResult UpdateTaskNode(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}
};

