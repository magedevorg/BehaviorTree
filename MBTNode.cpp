// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTNode.h"
#include "MBehaviorTree.h"
#include "MBTDecorator.h"
#include "MBTBlackboard.h"



//-----------------------------------------------------------------
// MBTNode
//-----------------------------------------------------------------
MBTNode::~MBTNode()
{
	// ��ϵ� ���ڷ��̴� ����
	MCLEAR_PTR_LIST(BlackboardDecoratorList);
}


void MBTNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	// ������ ���ڷ����͸� behavirotree�� �߰�
	for (auto& decorator : BlackboardDecoratorList) {
		inBehaviorTree->AddBlackboardDecorator(decorator);
	}
}

MBTBlackboardDecorator* MBTNode::AddBlackboardDecorator()
{
	MBTBlackboardDecorator* decorator = new MBTBlackboardDecorator(this);
	BlackboardDecoratorList.push_back(decorator);
	return decorator;
}


MBOOL MBTNode::CheckExecuteCondition(const MBTExecuteParam& inParam)
{
	// �����带 ��´�
	MBTBlackboard* blackboard = inParam.BehaviorTree->GetBlackboard();

	// ��ϵ� ���ڷ����͸� ���鼭 ���� �´��� üũ
	for (MBTBlackboardDecorator* decoration : BlackboardDecoratorList)
	{
		if (MFALSE == decoration->CheckCondition(blackboard)) {
			return MFALSE;
		}
	}

	return MTRUE;
}


//-----------------------------------------------------------------
// MBTCompositeNode
//-----------------------------------------------------------------
MBTCompositeNode::~MBTCompositeNode()
{
	for (auto& childNode : ChildNodeList) {
		delete childNode;
	}
}


void MBTCompositeNode::InitNum(int32& inBase)
{
	// �ڽ��� �����ϰ�
	MBTNode::InitNum(inBase);

	// �ڽĿ� ����
	for (MBTNode* node : ChildNodeList) {
		node->InitNum(inBase);
	}
}

void MBTCompositeNode::InitDepth(int32& inBase)
{	
	Depth = inBase++;

	// �ڽĿ� ����
	for (MBTNode* node : ChildNodeList) {
		node->InitDepth(inBase);
	}
}



void MBTCompositeNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	MBTNode::InitDecorator(inBehaviorTree);

	// ��ϵ� �ڽ� ������ ���ڷ����͸� �ʱ�ȭ
	for (MBTNode* childNode : ChildNodeList) {
		childNode->InitDecorator(inBehaviorTree);
	}
}


//-----------------------------------------------------------------
// MBTSequenceNode
// ��� ��尡 success�ϰ�� success
//-----------------------------------------------------------------
MBTResult MBTSequenceNode::Execute(const MBTExecuteParam& inParam)
{
	MBTResult result = MBTResult::Skip;

	// �ڽ� ��� ����
	for (auto& childNode : ChildNodeList)
	{
		// ���� �� ��� ����
		result = childNode->Execute(inParam);

		// none�ΰ�� ����ó��
		if (MBTResult::Skip == result) {
			continue;
		}

		// ����/������/�ߴ��� ��� ����
		switch (result)
		{
		case MBTResult::Failed:
		case MBTResult::InProgress:
		case MBTResult::Abort:
			return result;
		}
	}
	
	return result;
}



//-----------------------------------------------------------------
// MBTSelectorNode
// ��� ��尡 fail�ΰ�� fail
//-----------------------------------------------------------------
MBTResult MBTSelectorNode::Execute(const MBTExecuteParam& inParam)
{
	MBTResult result = MBTResult::Skip;

	// �ڽ� ��� ����
	for (MBTNode* childNode : ChildNodeList)
	{
		// ���� �� ��� ����
		result = childNode->Execute(inParam);

		// none�ΰ�� ����ó��
		if (MBTResult::Skip == result) {
			continue;
		}

		// ����/ó����/�ߴ��� ��� �ٷ� ����
		switch (result)
		{
		case MBTResult::Succeeded:
		case MBTResult::InProgress:
		case MBTResult::Abort:
			return result;
		}
	}

	return result;
}



//-----------------------------------------------------------------
// MBTTaskNode
//-----------------------------------------------------------------
MBTResult MBTTaskNode::Execute(const MBTExecuteParam& inParam)
{
	// ������ ���� ��� ��ȣ���� �۴ٸ� ��ŵ
	if (Num < inParam.ExecuteNodeNum) {
		return MBTResult::Skip;
	}

	// ���� ���� ����� ������ ���ϰ� ó��
	if (Num == inParam.ExecuteNodeNum && (MBTResult::None != inParam.ExecuteNodeResult)) {
		return inParam.ExecuteNodeResult;
	}


	// �׽�ũ ��� ����
	MBTResult result = ExecuteTaskNode(inParam.BehaviorTree);

	// ���� �������̶�� �ൿ Ʈ���� �������� �۾��� �����ϰ� ����
	if (MBTResult::InProgress == result) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}

	return result;
}



MBTResult MBTTaskNode::Update(class MBehaviorTree* inBehaviorTree, float inDelta)
{
	return UpdateTaskNode(inBehaviorTree, inDelta);
}