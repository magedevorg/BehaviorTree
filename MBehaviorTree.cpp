// Fill out your copyright notice in the Description page of Project Settings.


#include "MBehaviorTree.h"
#include "MBTNode.h"
#include "MBTBlackboard.h"
#include "MBTDecorator.h"


MBehaviorTree::MBehaviorTree()
{
	// 블랙보드 생성
	Blackboard = new MBTBlackboard();
}


MBehaviorTree::~MBehaviorTree()
{
	delete Blackboard;

	if (nullptr != RootNode) {
		delete RootNode;
	}
	
	// 리스트 제거
	for (auto& iter : BlackboardDecoratorListMap) {
		delete iter.second;
	}
}



void MBehaviorTree::FinishedNodeSetting()
{
	// 번호 초기화
	MINT32 num = 0;
	RootNode->InitNum(num);

	// 뎁스 초기화
	MINT32 depth = 0;
	RootNode->InitDepth(depth);

	// 데코레이터 초기화
	RootNode->InitDecorator(this);
}


void MBehaviorTree::UpdateBehaviorTree(float inDelta)
{
	// 동작 파라미터 설정
	MBTExecuteParam param;
	{
		param.BehaviorTree = this;
		param.ExecuteNodeNum = std::numeric_limits<MINT32>::max();
		param.ExecuteNodeResult = MBTExecuteResult::None;
	}


	// 블랙보드 체크
	CheckForceStartNode_BlackboardDecorator(param);


	// 현재 진행중인 노드가 있는경우
	if (nullptr != InProgressTaskNode)
	{
		// 진행중인 노드 번호
		const MINT32 inProgressTaskNodeNum = InProgressTaskNode->GetNum();

		// 강제 진행해야하는 노드보다 진행중인 노드의 우선순위가 더 높다면 그댜로 진행
		if (inProgressTaskNodeNum < param.ExecuteNodeNum)
		{
			// 해당 노드가 진행중인경우 리턴
			MBTResult inProgressTaskResult = InProgressTaskNode->Update(this, inDelta);
			if (MBTResult::InProgress == inProgressTaskResult) {
				return;
			}

			// 그외의 경우는 해당 노드로 이동해서 그대로 결과 처리
			param.ExecuteNodeNum = inProgressTaskNodeNum;
			param.ExecuteNodeResult = (MBTExecuteResult)inProgressTaskResult;
			
			// 초기화
			InProgressTaskNode = nullptr;
		}
	}
	
	// 설정된 실행노드가 없다면 0부터 시작
	if (std::numeric_limits<MINT32>::max() == param.ExecuteNodeNum) {
		param.ExecuteNodeNum = 0;
	}
	
	// 실행
	RootNode->Execute(param);
}

void MBehaviorTree::AddBlackboardDecorator(class MBTBlackboardDecorator* inDecorator)
{
	MBTFlowAbortMode abortMode = inDecorator->GetAbortMode();

	// 중단 모드가 설정되어있지 않다면 추가하지 않는다
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
	// 변경된 블랙 보드 정보가 있는지 체크
	// 여기 등록된 블랙보드 데코레이터는 중단설정이 되어있는것들임


	std::vector<MBTBlackboardValueBase*>* changeValueList = Blackboard->GetChangeValueList();
	if (MFALSE == changeValueList->empty())
	{
		// 변경내용이 존재한다면 루프를 돌면서 대상이 데코레이터에 설정된 내용인지 체크
		for (MBTBlackboardValueBase* value : *changeValueList)
		{
			// 대상 데코레이터가 존재하는지 체크
			auto findIter = BlackboardDecoratorListMap.find(value->Key);
			if (BlackboardDecoratorListMap.end() != findIter)
			{
				// 대상이 존재한다면
				std::list<class MBTBlackboardDecorator*>* decoratorList = findIter->second;
				for (class MBTBlackboardDecorator* decorator : *decoratorList)
				{
					MBTNode* node = decorator->GetNode();
					const MINT32 nodeNum = node->GetNum();

					// 낮은 번호의 노드를 설정
					inParam.ExecuteNodeNum = FMath::Min(inParam.ExecuteNodeNum, nodeNum);

					// 결과는 None으로 설정해서 해당 노드에서 실행되도록
					inParam.ExecuteNodeResult = MBTExecuteResult::None;
				}
			}
		}

		// 처리가 완료되었다면 변경정보 초기화
		Blackboard->ClearChangeValueList();
	}
}


