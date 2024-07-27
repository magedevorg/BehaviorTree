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
//-----------------------------------------------------------------
MBOOL MBTSequenceNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// �ڽ� ��� ����
	for (auto& childNode : ChildNodeList)
	{
		if (MFALSE == childNode->Execute(inResult, inParam)) {
			continue;
		}

		if (MBTResult::Failed == inResult ||
			MBTResult::InProgress == inResult ||
			MBTResult::Abort == inResult)
		{
			break;
		}
	}
	

	return MTRUE;
}



//-----------------------------------------------------------------
// MBTSelectorNode
//-----------------------------------------------------------------
bool MBTSelectorNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// �ڽ� ��� ����
	for (MBTNode* childNode : ChildNodeList)
	{
		// 
		if (MFALSE == childNode->Execute(inResult, inParam)) {
			continue;
		}

		// ������ ��� ����
		if (MBTResult::Succeeded == inResult ||
			MBTResult::InProgress == inResult ||
			MBTResult::Abort == inResult)
		{
			break;
		}
	}

	/*
	// ���� üũ�� ���
	if (nullptr != LoopDecorator)
	{
		inParam.inBehaviorTree->SetLoopNode(this);
		inResult = EOWBTResult::Abort;
		return true;
	}
	else
	{
		inParam.inBehaviorTree->SetLoopNode(nullptr);
	}
	*/

	return true;
}



//-----------------------------------------------------------------
// MBTTaskNode
//-----------------------------------------------------------------
MBOOL MBTTaskNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// ������ ������� üũ
	if (Num < inParam.StartNum) {
		return MFALSE;
	}

	// �׽�ũ ��� ����
	inResult = ExecuteTaskNode(inParam.BehaviorTree);

	// ���� �������̶�� �ൿ Ʈ���� �������� �۾��� �����ϰ� ����
	if (MBTResult::InProgress == inResult) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}


	return MTRUE;
}



MBTResult MBTTaskNode::Update(class MBehaviorTree* inBehaviorTree, float inDelta)
{
	return UpdateTaskNode(inBehaviorTree, inDelta);
}