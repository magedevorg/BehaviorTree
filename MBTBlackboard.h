// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MBTBase.h"
#include "MString.h"


//----------------------------------------------------------------
// 블랙보드에서 사용하는 값
//----------------------------------------------------------------
class MBTBlackboardValueBase
{
public:
	MBTBlackboardValueBase(const MString& inKey)
		: Key(inKey)
		, IsChange(false)
	{}

	// 키
	MString Key;

	// 정보가 변경되었는지에 대한 플래그
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

	// 값 타입을 얻는다
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
	// 사용할 블랙보드 값을 등록
	template <typename T>
	void AddValueLogic(std::map<MString, MBTBlackboardValue<T>*>& inMap, const MString& inKey, const T& inValue, MBTBlackboardValueType inType)
	{
		// 이미 등록된 키인지 체크
		MASSERT(ValueTypeMap.end() == ValueTypeMap.find(inKey));
		ValueTypeMap.emplace(inKey, inType);

		// 키 - 값 정보를 등록
		inMap.emplace(inKey, new MBTBlackboardValue<T>(inKey, inValue));
	}

	// 정보 설정 로직
	template<typename T>
	void SetValueLogic(std::map<MString, MBTBlackboardValue<T>*>& inMap, const MString& inKey, const T& inValue)
	{
		auto findIter = inMap.find(inKey);
		MASSERT(inMap.end() != findIter);

		// 사용할 값을 얻는다
		MBTBlackboardValue<T>* blackBoardValue = findIter->second;

		// 값이 변경된 경우만 처리
		if (blackBoardValue->Value == inValue) {
			return;
		}

		// 값을 변경
		blackBoardValue->Value = inValue;
	
		// 플래그가 설정되어있지 않다면 설정해주고 변경 리스트에 추가
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
	// 각 자료형 타입
	//--------------------------------------------------------------
	// 정수형 값
	std::map<MString, MBTBlackboardValue<MINT32>*> Int32ValueMap;

	// 부울 값
	std::map<MString, MBTBlackboardValue<MBOOL>*> BoolValueMap;

	//--------------------------------------------------------------
	// 기타
	//--------------------------------------------------------------
	// 변경된 값 리스트
	std::vector<MBTBlackboardValueBase*> ChangeValueList;

	// 해당 키의 값 타입 맵
	std::map<MString, MBTBlackboardValueType> ValueTypeMap;
};
