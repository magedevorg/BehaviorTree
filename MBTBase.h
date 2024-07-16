// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MPrerequisites.h"


// BT ó�� ���
enum class MBTResult : MUINT16
{
	Succeeded = 0,		// ����
	Failed,				// ����	
	InProgress,			// ������
	Abort
};



// ���� Ÿ��
enum class MBTExecuteType : MUINT16
{
	None = 0,				// �Ϲ����� ����
	CheckCondition,			// ������ üũ
	CompleteTaskNode,		// �۾� ��尡 �����
	Loop,					// ����
};