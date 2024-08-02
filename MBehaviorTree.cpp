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
	// ���� �Ķ���� ����
	MBTExecuteParam param;
	{
		param.BehaviorTree = this;
		param.ExecuteNodeNum = std::numeric_limits<MINT32>::max();
		param.ExecuteNodeResult = MBTExecuteResult::None;
	}


	// ������ üũ
	CheckForceStartNode_BlackboardDecorator(param);


	// ���� �������� ��尡 �ִ°��
	if (nullptr != InProgressTaskNode)
	{
		// �������� ��� ��ȣ
		const MINT32 inProgressTaskNodeNum = InProgressTaskNode->GetNum();

		// ���� �����ؾ��ϴ� ��庸�� �������� ����� �켱������ �� ���ٸ� �״��� ����
		if (inProgressTaskNodeNum < param.ExecuteNodeNum)
		{
			// �ش� ��尡 �������ΰ�� ����
			MBTResult inProgressTaskResult = InProgressTaskNode->Update(this, inDelta);
			if (MBTResult::InProgress == inProgressTaskResult) {
				return;
			}

			// �׿��� ���� �ش� ���� �̵��ؼ� �״�� ��� ó��
			param.ExecuteNodeNum = inProgressTaskNodeNum;
			param.ExecuteNodeResult = (MBTExecuteResult)inProgressTaskResult;
			
			// �ʱ�ȭ
			InProgressTaskNode = nullptr;
		}
	}
	
	// ������ �����尡 ���ٸ� 0���� ����
	if (std::numeric_limits<MINT32>::max() == param.ExecuteNodeNum) {
		param.ExecuteNodeNum = 0;
	}
	
	// ����
	RootNode->Execute(param);
}

void MBehaviorTree::AddBlackboardDecorator(class MBTBlackboardDecorator* inDecorator)
{
	MBTFlowAbortMode abortMode = inDecorator->GetAbortMode();

	// �ߴ� ��尡 �����Ǿ����� �ʴٸ� �߰����� �ʴ´�
	if (MBTFlowAbortMode::None == abortMode) {
		return;
	}

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



void MBehaviorTree::CheckForceStartNode_BlackboardDecorator(MBTExecuteParam& inParam)
{
	// ����� �� ���� ������ �ִ��� üũ
	// ���� ��ϵ� ������ ���ڷ����ʹ� �ߴܼ����� �Ǿ��ִ°͵���


	std::vector<MBTBlackboardValueBase*>* changeValueList = Blackboard->GetChangeValueList();
	if (MFALSE == changeValueList->empty())
	{
		// ���泻���� �����Ѵٸ� ������ ���鼭 ����� ���ڷ����Ϳ� ������ �������� üũ
		for (MBTBlackboardValueBase* value : *changeValueList)
		{
			// ��� ���ڷ����Ͱ� �����ϴ��� üũ
			auto findIter = BlackboardDecoratorListMap.find(value->Key);
			if (BlackboardDecoratorListMap.end() != findIter)
			{
				// ����� �����Ѵٸ�
				std::list<class MBTBlackboardDecorator*>* decoratorList = findIter->second;
				for (class MBTBlackboardDecorator* decorator : *decoratorList)
				{
					MBTNode* node = decorator->GetNode();
					const MINT32 nodeNum = node->GetNum();

					// ���� ��ȣ�� ��带 ����
					inParam.ExecuteNodeNum = FMath::Min(inParam.ExecuteNodeNum, nodeNum);

					// ����� None���� �����ؼ� �ش� ��忡�� ����ǵ���
					inParam.ExecuteNodeResult = MBTExecuteResult::None;
				}
			}
		}

		// ó���� �Ϸ�Ǿ��ٸ� �������� �ʱ�ȭ
		Blackboard->ClearChangeValueList();
	}
}


