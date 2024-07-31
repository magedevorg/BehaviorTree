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


//----------------------------------------------------------------
// Execute 결과
// MBTResult와 동일한 플래그 + 추가적인 처리를 위한 플래그가 있다
//----------------------------------------------------------------
enum class MBTExecuteResult : MUINT16
{
	Succeeded = 0,		// 성공
	Failed,				// 실패	
	InProgress,			// 진행중
	Abort,				// 중단

	// 추가적인 처리 플래그
	None,
	Skip,
};


// 실행 타입
enum class MBTExecuteType : MUINT16
{
	Play = 0,
	Finish,
};


// 실행 리턴 타입
enum class MBTExecuteReturnType : MUINT16
{
	None = 0,
	Skip,
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
