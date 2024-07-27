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
	MBTResult inProgressTaskResult = MBTResult::InProgress;

	if (nullptr != InProgressTaskNode) {
		inProgressTaskResult = InProgressTaskNode->Update(this, inDelta);
	}

	// ���� ó��
	MBTExecuteParam param;
	{
		param.BehaviorTree = this;
		param.StartNum = 0;
		param.ExecuteType = MBTExecuteType::None;
	}


	const int32 MAX_NODE_NUM = 9999999;

	// ��� ��� ��ȣ
	MINT32 targetNodeNum = MAX_NODE_NUM;

	MBTResult result;



	// ���� �������� ��尡 �ִ°��
	if (nullptr != InProgressTaskNode)
	{
		// ���� �������� ��尡 �켱������ �������� üũ
		MINT32 taskNodeNum = InProgressTaskNode->GetNum();
		if (taskNodeNum < targetNodeNum)
		{
			// �ش� ��尡 �������ΰ�� ����
			if (MBTResult::InProgress == inProgressTaskResult) {
				return;
			}

			

			// �׿��� ���� �ش� ���� �̵��ؼ� �״�� ��� ó��
			param.StartNum = InProgressTaskNode->GetNum() + 1;
			param.ExecuteType = MBTExecuteType::CompleteTaskNode;
			result = inProgressTaskResult;


			InProgressTaskNode = nullptr;
		}
	}
	
	
	
	// ����
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