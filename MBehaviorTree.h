// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MBTBase.h"
#include "MString.h"




//--------------------------------------------------------------------
// 비헤비어 트리 클래스
// 루트 노드는 CompositeNode로 설정
//--------------------------------------------------------------------
class MBehaviorTree
{
	friend class MBTNode;

public:
	MBehaviorTree();
	virtual ~MBehaviorTree();

public:
	
	template <typename T>
	T* InitRootNode()
	{
		T* node = new T();
		RootNode = node;
		return node;
	}
	
	// 노드 설정이 끝나면 호출
	void FinishedNodeSetting();


	// 비헤비어 트리 갱신
	void UpdateBehaviorTree(float inDelta);
	
	// 진한중인 테스크 노드를 설정
	void SetInProgressTaskNode(class MBTTaskNode* inTaskNode) {
		InProgressTaskNode = inTaskNode;
	}

	// 루트 노드를 얻는다
	class MBTCompositeNode* GetRootNode() {
		return RootNode;
	}

	// 블랙 보드를 얻는다
	class MBTBlackboard* GetBlackboard() {
		return Blackboard;
	}

protected:
	// 블랙보드 데코레이터 추가
	void AddBlackboardDecorator(class MBTBlackboardDecorator* inDecorator);


	// 
	void CheckForceStartNode_BlackboardDecorator(MBTExecuteParam& inParam);
	
protected:
	// 루트 노드
	class MBTCompositeNode* RootNode = nullptr;

	// 블랙 보드
	class MBTBlackboard* Blackboard = nullptr;

	// 진행중인 Task Node
	class MBTTaskNode* InProgressTaskNode = nullptr;


	//---------------------------------------------------------------
	// 데코레이터
	//---------------------------------------------------------------
	// 블랙보드 데코레이터 맵
	std::map<MString, std::list<class MBTBlackboardDecorator*>*> BlackboardDecoratorListMap;
};
