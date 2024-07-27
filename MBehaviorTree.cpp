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
	MBTResult inProgressTaskResult = MBTResult::InProgress;

	if (nullptr != InProgressTaskNode) {
		inProgressTaskResult = InProgressTaskNode->Update(this, inDelta);
	}

	// 동작 처리
	MBTExecuteParam param;
	{
		param.BehaviorTree = this;
		param.StartNum = 0;
		param.ExecuteType = MBTExecuteType::None;
	}


	const int32 MAX_NODE_NUM = 9999999;

	// 대상 노드 번호
	MINT32 targetNodeNum = MAX_NODE_NUM;

	MBTResult result;



	// 현재 진행중인 노드가 있는경우
	if (nullptr != InProgressTaskNode)
	{
		// 현재 진행중인 노드가 우선순위가 더높은지 체크
		MINT32 taskNodeNum = InProgressTaskNode->GetNum();
		if (taskNodeNum < targetNodeNum)
		{
			// 해당 노드가 진행중인경우 리턴
			if (MBTResult::InProgress == inProgressTaskResult) {
				return;
			}

			

			// 그외의 경우는 해당 노드로 이동해서 그대로 결과 처리
			param.StartNum = InProgressTaskNode->GetNum() + 1;
			param.ExecuteType = MBTExecuteType::CompleteTaskNode;
			result = inProgressTaskResult;


			InProgressTaskNode = nullptr;
		}
	}
	
	
	
	// 실행
	RootNode->Execute(result, param);
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