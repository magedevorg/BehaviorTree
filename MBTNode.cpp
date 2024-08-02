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

	// 등록된 데코레이터를 돌면서 값이 맞는지 체크(우선 AND 조건으로 체크)
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
// 모든 노드가 success일경우 success
//-----------------------------------------------------------------
MBTExecuteResult MBTSequenceNode::Execute(const MBTExecuteParam& inParam)
{
	MBTExecuteResult result = MBTExecuteResult::Skip;

	// 자식 노드 실행
	for (auto& childNode : ChildNodeList)
	{
		// 실행 및 결과 설정
		result = childNode->Execute(inParam);

		// none인경우 다음처리
		if (MBTExecuteResult::Skip == result) {
			continue;
		}

		// 실패/진행중/중단인 경우 리턴
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
// 모든 노드가 fail인경우 fail
//-----------------------------------------------------------------
MBTExecuteResult MBTSelectorNode::Execute(const MBTExecuteParam& inParam)
{
	MBTExecuteResult result = MBTExecuteResult::Skip;

	// 자식 노드 루프
	for (MBTNode* childNode : ChildNodeList)
	{
		// 실행 및 결과 설정
		result = childNode->Execute(inParam);

		// none인경우 다음처리
		if (MBTExecuteResult::Skip == result) {
			continue;
		}

		// 성공/처리중/중단인 경우 바로 리턴
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
	// 강제로 시작 노드 번호보다 작다면 스킵
	if (Num < inParam.ExecuteNodeNum) {
		return MBTExecuteResult::Skip;
	}

	// 강제 시작 노드라면 설정된 리턴값 처리
	if (Num == inParam.ExecuteNodeNum && (MBTExecuteResult::None != inParam.ExecuteNodeResult)) {
		return inParam.ExecuteNodeResult;
	}

	// 설정된 조건에 맞는지
	if (MFALSE == CheckExecuteCondition(inParam)) {
		return MBTExecuteResult::Failed;
	}

	// 테스크 노드 실행
	MBTResult result = ExecuteTaskNode(inParam.BehaviorTree);
	
	// 만약 진행중이라면 행동 트리에 진행중인 작업을 설정하고 리턴
	if (MBTResult::InProgress == result) {
		inParam.BehaviorTree->SetInProgressTaskNode(this);
	}

	return (MBTExecuteResult)result;
}



MBTResult MBTTaskNode::Update(class MBehaviorTree* inBehaviorTree, float inDelta)
{
	return UpdateTaskNode(inBehaviorTree, inDelta);
}