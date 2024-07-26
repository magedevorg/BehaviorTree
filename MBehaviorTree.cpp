// Fill out your copyright notice in the Description page of Project Settings.


#include "MBehaviorTree.h"
#include "MBTNode.h"
#include "MBTBlackboard.h"
#include "MBTDecorator.h"


MBehaviorTree::MBehaviorTree()
{
	// ������ ����
	Blackboard = new MBTBlackboard();
}


MBehaviorTree::~MBehaviorTree()
{
	delete Blackboard;

	if (nullptr != RootNode) {
		delete RootNode;
	}
	
	// ����Ʈ ����
	for (auto& iter : BlackboardDecoratorListMap) {
		delete iter.second;
	}
}



void MBehaviorTree::FinishedNodeSetting()
{
	// ��ȣ �ʱ�ȭ
	MINT32 num = 0;
	RootNode->InitNum(num);

	// ���� �ʱ�ȭ
	MINT32 depth = 0;
	RootNode->InitDepth(depth);

	// ���ڷ����� �ʱ�ȭ
	RootNode->InitDecorator(this);
}


void MBehaviorTree::UpdateBehaviorTree(float inDelta)
{
	// �������̴� Task Node�� �ִٸ� ó��
	if (nullptr != InProgressTaskNode)
	{
		InProgressTaskNode->UpdateTaskNode(this, inDelta);
	}
}

void MBehaviorTree::AddBlackboardDecorator(class MBTBlackboardDecorator* inDecorator)
{
	std::list<class MBTBlackboardDecorator*>* decoratorList = nullptr;
	{
		auto findIter = BlackboardDecoratorListMap.find(inDecorator->GetKey());
		if (BlackboardDecoratorListMap.end() == findIter)
		{
			decoratorList = new std::list<class MBTBlackboardDecorator*>();
			BlackboardDecoratorListMap.emplace(inDecorator->GetKey(), decoratorList);
		}
		else
		{
			decoratorList = findIter->second;
		}
	}
	
	decoratorList->push_back(inDecorator);
}