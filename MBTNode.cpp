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
	// 등록된 데코레이더 제거
	MCLEAR_PTR_LIST(BlackboardDecoratorList);
}


void MBTNode::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	// 블랙보드 데코레이터를 behavirotree에 추가
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
	// 블랙보드를 얻는다
	MBTBlackboard* blackboard = inParam.BehaviorTree->GetBlackboard();

	// 등록된 데코레이터를 돌면서 값이 맞는지 체크
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
MBOOL MBTSequenceNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// 자식 노드 실행
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
	// 자식 노드 루프
	for (MBTNode* childNode : ChildNodeList)
	{
		// 
		if (MFALSE == childNode->Execute(inResult, inParam)) {
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
MBOOL MBTTaskNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// 동작할 노드인지 체크
	if (Num < inParam.StartNum) {
		return MFALSE;
	}

	// 테스크 노드 실행
	inResult = ExecuteTaskNode(inParam.BehaviorTree);

	// 만약 진행중이라면 행동 트리에 진행중인 작업을 설정하고 리턴
	if (MBTResult::InProgress == inResult) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}


	return MTRUE;
}



MBTResult MBTTaskNode::Update(class MBehaviorTree* inBehaviorTree, float inDelta)
{
	return UpdateTaskNode(inBehaviorTree, inDelta);
}