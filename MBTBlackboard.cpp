// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTBlackboard.h"


MBTBlackboard::~MBTBlackboard()
{
	MCLEAR_PTR_MAP(Int32ValueMap);
	MCLEAR_PTR_MAP(BoolValueMap);
	MCLEAR_PTR_MAP(Vector3ValueMap);
}


MBTBlackboardValueType MBTBlackboard::GetValueType(const MString& inKey)
{
	auto findIter = ValueTypeMap.find(inKey);
	if (ValueTypeMap.end() != findIter) {
		return findIter->second;
	}

	return MBTBlackboardValueType::None;
}