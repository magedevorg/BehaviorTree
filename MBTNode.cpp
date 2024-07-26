// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTNode.h"
#include "MBehaviorTree.h"
#include "MBTDecorator.h"




//-----------------------------------------------------------------
// MBTTaskBaseNode
//-----------------------------------------------------------------
MBTNode::~MBTNode()
{
	// 등록된 데코레이더 제거
	for (MBTDecorator* decorator : DecoratorList) {
		delete decorator;
	}
}

void MBTNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	// 등록된 데코레이터 초기화
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
	// 자신을 설정하고
	MBTNode::InitNum(inBase);

	// 자식에 설정
	for (MBTNode* node : ChildNodeList) {
		node->InitNum(inBase);
	}
}

void MBTCompositeNode::InitDepth(int32& inBase)
{	
	Depth = inBase++;

	// 자식에 설정
	for (MBTNode* node : ChildNodeList) {
		node->InitDepth(inBase);
	}
}



void MBTCompositeNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	MBTNode::InitDecorator(inBehaviorTree);

	// 등록된 자식 노드들의 데코레이터를 초기화
	for (MBTNode* childNode : ChildNodeList) {
		childNode->InitDecorator(inBehaviorTree);
	}
}


//-----------------------------------------------------------------
// MBTSequenceNode
//-----------------------------------------------------------------
bool MBTSequenceNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// 자식 노드 실행
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
	// 자식 노드 루프
	for (MBTNode* childNode : ChildNodeList)
	{
		// 
		if (false == childNode->Execute(inResult, inParam)) {
			continue;
		}

		// 성공인 경우 리턴
		if (MBTResult::Succeeded == inResult ||
			MBTResult::InProgress == inResult ||
			MBTResult::Abort == inResult)
		{
			break;
		}
	}

	/*
	// 루프 체크후 등록
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
	// 동작할 노드인지 체크
	if (Num < inParam.StartNum) {
		return false;
	}

	// 테스크 노드 실행
	inResult = ExecuteTaskNode(inParam.BehaviorTree);

	// 만약 진행중이라면 행동 트리에 진행중인 작업을 설정하고 리턴
	if (MBTResult::InProgress == inResult) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}


	return true;
}