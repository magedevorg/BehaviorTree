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
	/*
	// 블랙보드 데코레이터를 behavirotree에 추가
	for (auto& decorator : BlackboardDecoratorList) {
		inBehaviorTree->AddBlackboardDecorator(decorator);
	}
	*/
}




MBTBlackboardDecorator* MBTNode::AddBlackboardDecorator()
{
	MBTBlackboardDecorator* decorator = new MBTBlackboardDecorator(this);
	BlackboardDecoratorList.push_back(decorator);
	return decorator;
}


MBOOL MBTNode::CheckExecuteCondition(const MBTExecuteParam& inParam)
{
	// 실행 조건을 체크한다
	
	// 블랙 보드 체크
	{
		// 블랙보드를 얻는다
		MBTBlackboard* blackboard = inParam.BehaviorTree->GetBlackboard();

		// 등록된 데코레이터를 돌면서 값이 맞는지 체크(우선 AND 조건으로 체크)
		for (MBTBlackboardDecorator* decoration : BlackboardDecoratorList)
		{
			if (MFALSE == decoration->CheckCondition(blackboard)) {
				return MFALSE;
			}
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


void MBTCompositeNode::InitNum(MINT32& inBase)
{
	// 자신을 설정하고
	MBTNode::InitNum(inBase);

	// 자식에 설정
	for (MBTNode* node : ChildNodeList) {
		node->InitNum(inBase);
	}
}

void MBTCompositeNode::InitDepth(MINT32& inBase)
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

MBOOL MBTCompositeNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// 해당 노드가 실행될수 있는지 체크
	if (MFALSE == CheckExecuteCondition(inParam)) 
	{
		inResult = MBTResult::Failed;
		return MTRUE;
	}

	MBOOL isProcess = MFALSE;
	
	// 스택에 설정
	inParam.BehaviorTree->PushNodeStack(this);

	for (auto& childNode : ChildNodeList)
	{
		if (MFALSE == childNode->Execute(inResult, inParam)) {
			continue;
		}

		isProcess = MTRUE;

		// 중단 결과인지 체크
		if (MTRUE == CheckStopFlag(inResult)) {
			break;
		}
	}

	if (MFALSE == isProcess || (MBTResult::InProgress != inResult))
	{
		// 처리가 되지 않았거나 
		// 결과 플래그가 처리중이라면 스택에서 제거
		inParam.BehaviorTree->PopNodeStack(this);
	}

	return isProcess;
}


//-----------------------------------------------------------------
// MBTSequenceNode
// 모든 노드가 success일경우 success
//-----------------------------------------------------------------
MBOOL MBTSequenceNode::CheckStopFlag(MBTResult inResult)
{
	switch (inResult)
	{
	case MBTResult::Failed:
	case MBTResult::InProgress:
	case MBTResult::Abort:
		return MTRUE;
	}

	return MFALSE;
}

//-----------------------------------------------------------------
// MBTSelectorNode
// 모든 노드가 fail인경우 fail
//-----------------------------------------------------------------
MBOOL MBTSelectorNode::CheckStopFlag(MBTResult inResult)
{
	switch (inResult)
	{
	case MBTResult::Succeeded:
	case MBTResult::InProgress:
	case MBTResult::Abort:
		return MTRUE;
	}
	return MFALSE;
}


//-----------------------------------------------------------------
// MBTTaskNode
//-----------------------------------------------------------------
MBOOL MBTTaskNode::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	// 강제로 시작 노드 번호보다 작다면 스킵
	if (Num < inParam.ExecuteNodeNum) {
		return MFALSE;
	}

	// 강제 시작 노드라면 설정된 리턴값 처리
	if (Num == inParam.ExecuteNodeNum && (MTRUE == inParam.ExecuteNodeUseResult))
	{
		inResult = inParam.ExecuteNodeResult;
		return MTRUE;
	}

	// 스택에 설정
	inParam.BehaviorTree->PushNodeStack(this);

	// 결과를 얻는다
	MBTResult result = ExecuteTaskNode(inParam.BehaviorTree);
	if (MBTResult::InProgress != inResult)
	{
		// 처리가 되지 않았거나 
		// 결과 플래그가 처리중이라면 스택에서 제거
		inParam.BehaviorTree->PopNodeStack(this);
	}

	return MTRUE;
}

