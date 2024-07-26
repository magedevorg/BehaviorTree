// Fill out your copyright notice in the Description page of Project Settings.


#include "MBTDecorator.h"
#include "MBTNode.h"
#include "MBehaviorTree.h"


//-----------------------------------------------------------------
// MBTDecorator
//-----------------------------------------------------------------
MBTDecorator::MBTDecorator(class MBTNode* inNode)
	: Node(inNode)
{
}



//-----------------------------------------------------------------
// MBTBlackboardDecorator
//-----------------------------------------------------------------
MBTBlackboardDecorator::MBTBlackboardDecorator(class MBTNode* inNode)
	: MBTDecorator(inNode)
{	
}

void MBTBlackboardDecorator::InitDecorator(class MBehaviorTree* inBehaviorTree)
{
	inBehaviorTree->AddBlackboardDecorator(this);
}