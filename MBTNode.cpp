// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTNode.h"
#include "MBehaviorTree.h"




bool MBTNodeBase::Execute(MBTResult& inResult, const MBTExecuteParam& inParam)
{
	//inParam.BehaviorTree->SetNodeStack(Depth, this);
	return true;
}

