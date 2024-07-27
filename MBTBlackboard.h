// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"
#include "MString.h"


//----------------------------------------------------------------
// �����忡�� ����ϴ� ��
//----------------------------------------------------------------
class MBTBlackboardValueBase
{
public:
	MBTBlackboardValueBase(const MString& inKey)
		: Key(inKey)
		, IsChange(false)
	{}

	// Ű
	MString Key;

	// ������ ����Ǿ������� ���� �÷���
	bool IsChange = false;
};


template <typename T>
class MBTBlackboardValue : public MBTBlackboardValueBase
{
public:
	MBTBlackboardValue(const MString& inKey, T inValue)
		: MBTBlackboardValueBase(inKey)
		, Value(inValue)
	{
	}

	T Value;
};




class MBTBlackboard
{
public:
	MBTBlackboard() {}
	virtual ~MBTBlackboard();

public:

	// �� Ÿ���� ��´�
	MBTBlackboardValueType GetValueType(const MString& inKey);

	//-------------------------------------------------------------
	// int32
	//-------------------------------------------------------------
	void AddInt32Value(const MString& inKey, MINT32 inValue) {
		AddValueLogic<MINT32>(Int32ValueMap, inKey, inValue, MBTBlackboardValueType::Int32);
	}

	void SetInt32Value(const MString& inKey, MINT32 inValue) {
		SetValueLogic<MINT32>(Int32ValueMap, inKey, inValue);
	}

	MINT32 GetInt32Value(const MString& inKey) {
		return GetValueLogic<MINT32>(Int32ValueMap, inKey);
	}

	//-------------------------------------------------------------
	// bool
	//-------------------------------------------------------------
	void AddBoolValue(const MString& inKey, MBOOL inValue) {
		AddValueLogic<bool>(BoolValueMap, inKey, inValue, MBTBlackboardValueType::Bool);
	}

	void SetBoolValue(const MString& inKey, MBOOL inValue) {
		SetValueLogic<MBOOL>(BoolValueMap, inKey, inValue);
	}

	MBOOL GetBoolValue(const MString& inKey) {
		return GetValueLogic<MBOOL>(BoolValueMap, inKey);
	}
	
protected:
	// ����� ������ ���� ���
	template <typename T>
	void AddValueLogic(std::map<MString, MBTBlackboardValue<T>*>& inMap, const MString& inKey, const T& inValue, MBTBlackboardValueType inType)
	{
		// �̹� ��ϵ� Ű���� üũ
		MASSERT(ValueTypeMap.end() == ValueTypeMap.find(inKey));
		ValueTypeMap.emplace(inKey, inType);

		// Ű - �� ������ ���
		inMap.emplace(inKey, new MBTBlackboardValue<T>(inKey, inValue));
	}

	// ���� ���� ����
	template<typename T>
	void SetValueLogic(std::map<MString, MBTBlackboardValue<T>*>& inMap, const MString& inKey, const T& inValue)
	{
		auto findIter = inMap.find(inKey);
		MASSERT(inMap.end() != findIter);

		// ����� ���� ��´�
		MBTBlackboardValue<T>* blackBoardValue = findIter->second;

		// ���� ����� ��츸 ó��
		if (blackBoardValue->Value == inValue) {
			return;
		}

		// ���� ����
		blackBoardValue->Value = inValue;
	
		// �÷��װ� �����Ǿ����� �ʴٸ� �������ְ� ���� ����Ʈ�� �߰�
		if (false == blackBoardValue->IsChange)
		{
			blackBoardValue->IsChange = true;
			ChangeValueList.push_back(blackBoardValue);
		}
	}

	template<typename T>
	const T& GetValueLogic(std::map<MString, MBTBlackboardValue<T>*>& inMap, const MString& inKey)
	{
		auto findIter = inMap.find(inKey);
		MASSERT(inMap.end() != findIter);

		MBTBlackboardValue<T>* blackBoardValue = findIter->second;
		return blackBoardValue->Value;
	}

protected:
	//--------------------------------------------------------------
	// �� �ڷ��� Ÿ��
	//--------------------------------------------------------------
	// ������ ��
	std::map<MString, MBTBlackboardValue<MINT32>*> Int32ValueMap;

	// �ο� ��
	std::map<MString, MBTBlackboardValue<MBOOL>*> BoolValueMap;

	//--------------------------------------------------------------
	// ��Ÿ
	//--------------------------------------------------------------
	// ����� �� ����Ʈ
	std::vector<MBTBlackboardValueBase*> ChangeValueList;

	// �ش� Ű�� �� Ÿ�� ��
	std::map<MString, MBTBlackboardValueType> ValueTypeMap;
};
