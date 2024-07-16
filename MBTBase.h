// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MPrerequisites.h"


// BT 처리 결과
enum class MBTResult : MUINT16
{
	Succeeded = 0,		// 성공
	Failed,				// 실패	
	InProgress,			// 진행중
	Abort
};



// 실행 타입
enum class MBTExecuteType : MUINT16
{
	None = 0,				// 일반적인 시작
	CheckCondition,			// 조건을 체크
	CompleteTaskNode,		// 작업 노드가 종료됨
	Loop,					// 루프
};