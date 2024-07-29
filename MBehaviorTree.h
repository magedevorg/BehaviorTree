// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MBTBase.h"
#include "MString.h"




//--------------------------------------------------------------------
// ������ Ʈ�� Ŭ����
// ��Ʈ ���� CompositeNode�� ����
//--------------------------------------------------------------------
class MBehaviorTree
{
	friend class MBTNode;

public:
	MBehaviorTree();
	virtual ~MBehaviorTree();

public:
	
	template <typename T>
	T* InitRootNode()
	{
		T* node = new T();
		RootNode = node;
		return node;
	}
	
	// ��� ������ ������ ȣ��
	void FinishedNodeSetting();


	// ������ Ʈ�� ����
	void UpdateBehaviorTree(float inDelta);
	
	// �������� �׽�ũ ��带 ����
	void SetInProgressTaskNode(class MBTTaskNode* inTaskNode) {
		InProgressTaskNode = inTaskNode;
	}

	// ��Ʈ ��带 ��´�
	class MBTCompositeNode* GetRootNode() {
		return RootNode;
	}

	// �� ���带 ��´�
	class MBTBlackboard* GetBlackboard() {
		return Blackboard;
	}

protected:
	// ������ ���ڷ����� �߰�
	void AddBlackboardDecorator(class MBTBlackboardDecorator* inDecorator);


	// 
	void CheckForceStartNode_BlackboardDecorator(MBTExecuteParam& inParam);
	
protected:
	// ��Ʈ ���
	class MBTCompositeNode* RootNode = nullptr;

	// �� ����
	class MBTBlackboard* Blackboard = nullptr;

	// �������� Task Node
	class MBTTaskNode* InProgressTaskNode = nullptr;


	//---------------------------------------------------------------
	// ���ڷ�����
	//---------------------------------------------------------------
	// ������ ���ڷ����� ��
	std::map<MString, std::list<class MBTBlackboardDecorator*>*> BlackboardDecoratorListMap;
};
