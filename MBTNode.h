// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute시에 필요한 파라미터
struct MBTExecuteParam
{
	// 비헤비더 트리
	class MBehaviorTree* BehaviorTree = nullptr;

	//-------------------------------------------------------
	// 실행 정보
	//-------------------------------------------------------
	// 실행노드 
	MINT32 ExecuteNodeNum = 0;
	MBTExecuteResult ExecuteNodeResult = MBTExecuteResult::None;
};





//------------------------------------------------------------
// Behavior Tree 최상위 노드
// - 노드 정보(번호, 깊이 등)와 기본 인터페이스를 가진다
//------------------------------------------------------------
class MBTNode
{
public:
	MBTNode() {}
	virtual ~MBTNode();

public:
	//-----------------------------------------------------------
	// 초기화
	//-----------------------------------------------------------
	// 번호 초기화
	virtual void InitNum(MINT32& inBase) {
		Num = inBase++;
	}

	// 깊이 초기화
	virtual void InitDepth(MINT32& inBase) {
		Depth = inBase++;
	}

	// 데코레이더 초기화
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree);


	//-----------------------------------------------------------
	// 기능함수
	//-----------------------------------------------------------
	// 실행
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) {
		return MBTExecuteResult::Succeeded;
	}

	// 노드 갱신
	virtual MBTResult Update(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}
	

	//-----------------------------------------------------------
	// 데코레이터 추가
	//-----------------------------------------------------------
	class MBTBlackboardDecorator* AddBlackboardDecorator();

	
	//-----------------------------------------------------------
	// Getter
	//-----------------------------------------------------------
	// 번호를 얻는다
	MINT32 GetNum() const {
		return Num;
	}

	MINT32 GetDepth() const {
		return Depth;
	}

protected:
	// 해당 노드가 실행될수 있는지 체크
	MBOOL CheckExecuteCondition(const MBTExecuteParam& inParam);

	
	

protected:
	// 노드 번호
	MINT32 Num = 0;

	// 트리 깊이
	MINT32 Depth = 0;

	//--------------------------------------------------------------
	// 데코레이터 리스트
	//--------------------------------------------------------------
	// 블랙보드 데코레이터
	std::list<class MBTBlackboardDecorator*> BlackboardDecoratorList;
};




//------------------------------------------------------------
// 컴포짓 노드
// - 자식 노드 관리
//------------------------------------------------------------ 
class MBTCompositeNode : public MBTNode
{
public:
	MBTCompositeNode() {}
	virtual ~MBTCompositeNode();

public:
	// 초기화
	virtual void InitNum(int32& inBase) override;
	virtual void InitDepth(int32& inBase) override;

	// 데코레이더 초기화
	virtual void InitDecorator(class MBehaviorTree* inBehaviorTree) override;

	template<typename T>
	T* AddChildNode()
	{
		T* childNode = new T();
		//childNode->InitNode();
		ChildNodeList.push_back(childNode);
		return childNode;
	}

protected:
	// 자식 노드 리스트
	std::list<class MBTNode*> ChildNodeList;
};


//------------------------------------------------------------
// 시퀀스 노드
// 순차적으로 실행하다 Failed/InProgress/Abort일때 리턴
//------------------------------------------------------------
class MBTSequenceNode : public MBTCompositeNode
{
public:
	MBTSequenceNode() {}

public:
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;
};



//------------------------------------------------------------
// 셀렉터 노드
// 순차적으로 실행하다 Succeeded/InProgress/Abort일때 리턴
//------------------------------------------------------------
class MBTSelectorNode : public MBTCompositeNode
{
public:
	MBTSelectorNode() {}

public:
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;
};




//------------------------------------------------------------
// 작업 노드
//------------------------------------------------------------ 
class MBTTaskNode : public MBTNode
{
public:
	MBTTaskNode() {}

public:
	// 노드 실행
	virtual MBTExecuteResult Execute(const MBTExecuteParam& inParam) override;

	// 노드 갱신
	virtual MBTResult Update(class MBehaviorTree* inBehaviorTree, float inDelta) override;

protected:
	// 작업 시작
	virtual MBTResult ExecuteTaskNode(class MBehaviorTree* inBehaviorTree) {
		return MBTResult::Succeeded;
	}

	// 노드 갱신 처리
	virtual MBTResult UpdateTaskNode(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}
};

