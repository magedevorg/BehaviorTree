// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MPrerequisites.h"


//----------------------------------------------------------------
// BT 처리 결과
// 사용자가 참조하는 결과
//----------------------------------------------------------------
enum class MBTResult : MUINT16
{
	Succeeded = 0,		// 성공
	Failed,				// 실패	
	InProgress,			// 진행중
	Abort,				// 중단
};




// 실행 타입
enum class MBTExecuteType : MUINT16
{
	Play = 0,
	Finish,
};


// 블랙보드 값 타입
enum class MBTBlackboardValueType : MUINT16
{
	None = 0,
	Int32,
	Bool,
	Vector3,
};


// 설정되었는지
enum class MBTKeyOperation : MUINT16
{
	Set,				
	NotSet
};


// 산술 연산
enum class MBTArithmeticOperation : uint8
{
	Equal,		
	NotEqual,
	Less,		
	LessOrEqual,
	Greater,	
	GreaterOrEqual,
};




enum class MBTFlowAbortMode : uint8
{
	None				UMETA(DisplayName = "Nothing"),
	LowerPriority		UMETA(DisplayName = "Lower Priority"),
	Self				UMETA(DisplayName = "Self"),
	Both				UMETA(DisplayName = "Both"),
};






// 노드 동작시 사용할 파라미터
struct MBTExecuteParam
{
	// 비헤비더 트리
	class MBehaviorTree* BehaviorTree = nullptr;

	//-------------------------------------------------------
	// 실행 정보
	//-------------------------------------------------------
	// 실행 노드 번호
	MINT32 ExecuteNodeNum = 0;

	// 설정된 결과를 사용할것인지
	MBOOL ExecuteNodeUseResult = MFALSE;
	MBTResult ExecuteNodeResult = MBTResult::Succeeded;
};

