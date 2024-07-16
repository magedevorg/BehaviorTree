// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"



// execute시에 필요한 파라미터
struct MBTExecuteParam
{
	// 행동 트리
	class MBehaviorTree* BehaviorTree = nullptr;

	// 시작 노드
	//int32 StartNum = 0;

	// 실행 타입
	MBTExecuteType ExecuteType = MBTExecuteType::None;

	// 결과 
	MBTResult Result = MBTResult::Succeeded;
};



//------------------------------------------------------------
// 노드 베이스
//------------------------------------------------------------
class MBTNodeBase
{
public:
	virtual ~MBTNodeBase() {}

public:
	// 초기화
	virtual void InitNode() {}

	// 실행
	virtual bool Execute(MBTResult& inResult, const MBTExecuteParam& inParam);




protected:
	// 노드 번호
	MINT32 Num = 0;
};


//------------------------------------------------------------
// 노드 베이스
//------------------------------------------------------------
class MBTRootNode : public MBTNodeBase
{
public:
	


protected:
	// 자식 노드
	class MBTNode* ChildNode = nullptr;
};



//------------------------------------------------------------
// 노드
//------------------------------------------------------------ 
class MBTNode : public MBTNodeBase
{
public:
	

protected:
	// 부모 노드 추가
	class MBTNodeBase* ParentNode = nullptr;
};
