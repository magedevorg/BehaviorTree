// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MPrerequisites.h"


// BT 처리 결과
enum class MBTResult : MUINT16
{
	None			= 0,		
	Succeeded,			// 성공
	Failed,				// 실패	
	InProgress,			// 진행중
	Abort,				// 중단
	Skip,				// 그냥 넘어감
};



// 블랙보드 값 타입
enum class MBTBlackboardValueType : MUINT16
{
	None = 0,
	Int32,
	Bool,
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