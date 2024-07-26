// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTNode.h"
#include "MBehaviorTree.h"
#include "MBTDecorator.h"




//-----------------------------------------------------------------
// MBTTaskBaseNode
//-----------------------------------------------------------------
MBTNode::~MBTNode()
{
	// ��ϵ� ���ڷ��̴� ����
	for (MBTDecorator* decorator : DecoratorList) {
		delete decorator;
	}
}

void MBTNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	// ��ϵ� ���ڷ����� �ʱ�ȭ
	for (MBTDecorator* decorator : DecoratorList) {
		decorator->InitDecorator(inBehaviorTree);
	}
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
bool MBTSequenceNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// �ڽ� ��� ����
	for (auto& childNode : ChildNodeList)
	{
		childNode->Execute(inResult, inParam);

		if (MBTResult::Failed == inResult ||
			MBTResult::InProgress == inResult ||
			MBTResult::Abort == inResult)
		{
			break;
		}
	}
	

	return true;
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
		if (false == childNode->Execute(inResult, inParam)) {
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
bool MBTTaskNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// ������ ������� üũ
	if (Num < inParam.StartNum) {
		return false;
	}

	// �׽�ũ ��� ����
	inResult = ExecuteTaskNode(inParam.BehaviorTree);

	// ���� �������̶�� �ൿ Ʈ���� �������� �۾��� �����ϰ� ����
	if (MBTResult::InProgress == inResult) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}


	return true;
}