// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute시에 필요한 파라미터
struct MBTExecuteParam
{
	// 비헤비더 트리
	class MBehaviorTree* BehaviorTree = nullptr;

	// 시작 노드
	int32 StartNum = 0;

	// 실행 타입
	MBTExecuteType ExecuteType = MBTExecuteType::None;
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
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) { return true; }

	// 데코레이터 추가
	template<typename T>
	T* AddDecorator()
	{
		T* decorator = new T(this);
		DecoratorList.push_back(decorator);
	}


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
	// 노드 번호
	MINT32 Num = 0;

	// 트리 깊이
	MINT32 Depth = 0;

	// 데코레이터 리스트
	std::list<class MBTDecorator*> DecoratorList;
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
		childNode->InitNode();
		ChildNodeList.insert(childNode);
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
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;
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
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;
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
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam) override;


	// 노드 갱신 처리
	virtual MBTResult UpdateTaskNode(class MBehaviorTree* inBehaviorTree, float inDelta) {
		return MBTResult::Succeeded;
	}

protected:
	// 작업 시작
	virtual MBTResult ExecuteTaskNode(class MBehaviorTree* inBehaviorTree) {
		return MBTResult::Succeeded;
	}
};

