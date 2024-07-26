// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTBlackboard.h"


MBTBlackboard::~MBTBlackboard()
{
	MCLEAR_PTR_MAP(IntValueMap);
	MCLEAR_PTR_MAP(BoolValueMap);
	MCLEAR_PTR_MAP(StringValueMap);
}
