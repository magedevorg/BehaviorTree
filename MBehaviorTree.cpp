// Fill out your copyright notice in the Description page of Project Settings.


#include "MBehaviorTree.h"
#include "MBTNode.h"
#include "MBTBlackboard.h"
#include "MBTDecorator.h"


MBehaviorTree::MBehaviorTree()
{
	// 블랙보드 생성
	Blackboard = new MBTBlackboard();
}


MBehaviorTree::~MBehaviorTree()
{
	delete Blackboard;

	if (nullptr != RootNode) {
		delete RootNode;
	}
	
	// 리스트 제거
	for (auto& iter : BlackboardDecoratorListMap) {
		delete iter.second;
	}
}



void MBehaviorTree::FinishedNodeSetting()
{
	// 번호 초기화
	MINT32 num = 0;
	RootNode->InitNum(num);

	// 뎁스 초기화
	MINT32 depth = 0;
	RootNode->InitDepth(depth);

	// 데코레이터 초기화
	RootNode->InitDecorator(this);
}


void MBehaviorTree::UpdateBehaviorTree(float inDelta)
{
	// 진행중이던 Task Node가 있다면 처리
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