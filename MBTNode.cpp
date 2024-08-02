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

	// ��ϵ� ���ڷ����͸� ���鼭 ���� �´��� üũ(�켱 AND �������� üũ)
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
MBTExecuteResult MBTSequenceNode::Execute(const MBTExecuteParam& inParam)
{
	MBTExecuteResult result = MBTExecuteResult::Skip;

	// �ڽ� ��� ����
	for (auto& childNode : ChildNodeList)
	{
		// ���� �� ��� ����
		result = childNode->Execute(inParam);

		// none�ΰ�� ����ó��
		if (MBTExecuteResult::Skip == result) {
			continue;
		}

		// ����/������/�ߴ��� ��� ����
		switch (result)
		{
		case MBTExecuteResult::Failed:
		case MBTExecuteResult::InProgress:
		case MBTExecuteResult::Abort:
			return result;
		}
	}
	
	return result;
}



//-----------------------------------------------------------------
// MBTSelectorNode
// ��� ��尡 fail�ΰ�� fail
//-----------------------------------------------------------------
MBTExecuteResult MBTSelectorNode::Execute(const MBTExecuteParam& inParam)
{
	MBTExecuteResult result = MBTExecuteResult::Skip;

	// �ڽ� ��� ����
	for (MBTNode* childNode : ChildNodeList)
	{
		// ���� �� ��� ����
		result = childNode->Execute(inParam);

		// none�ΰ�� ����ó��
		if (MBTExecuteResult::Skip == result) {
			continue;
		}

		// ����/ó����/�ߴ��� ��� �ٷ� ����
		switch (result)
		{
		case MBTExecuteResult::Succeeded:
		case MBTExecuteResult::InProgress:
		case MBTExecuteResult::Abort:
			return result;
		}
	}

	return result;
}



//-----------------------------------------------------------------
// MBTTaskNode
//-----------------------------------------------------------------
MBTExecuteResult MBTTaskNode::Execute(const MBTExecuteParam& inParam)
{
	// ������ ���� ��� ��ȣ���� �۴ٸ� ��ŵ
	if (Num < inParam.ExecuteNodeNum) {
		return MBTExecuteResult::Skip;
	}

	// ���� ���� ����� ������ ���ϰ� ó��
	if (Num == inParam.ExecuteNodeNum && (MBTExecuteResult::None != inParam.ExecuteNodeResult)) {
		return inParam.ExecuteNodeResult;
	}

	// ������ ���ǿ� �´���
	if (MFALSE == CheckExecuteCondition(inParam)) {
		return MBTExecuteResult::Failed;
	}

	// �׽�ũ ��� ����
	MBTResult result = ExecuteTaskNode(inParam.BehaviorTree);
	
	// ���� �������̶�� �ൿ Ʈ���� �������� �۾��� �����ϰ� ����
	if (MBTResult::InProgress == result) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}

	return (MBTExecuteResult)result;
}



MBTResult MBTTaskNode::Update(class MBehaviorTree* inBehaviorTree, float inDelta)
{
	return UpdateTaskNode(inBehaviorTree, inDelta);
}