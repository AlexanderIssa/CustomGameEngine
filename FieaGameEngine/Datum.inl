#pragma once
#include "pch.h"
#include "Datum.h"

using namespace Fiea::GameEngine;

//template<>
//int32_t& Datum::Get<int32_t>(size_t idx)
//{
//	if (_datType == DatumType::Int)
//	{
//		return intArr[idx];
//	}
//	static int32_t defaultValue = 0;
//	return defaultValue;
//}

//void* Datum::Get(size_t idx)
//{
//	switch (_datType)
//	{
//	case DatumType::Unknown:
//		break;
//	case DatumType::Int:
//		return &intArr[idx];
//		break;
//	case DatumType::Float:
//		return &floatArr[idx];
//		break;
//	case DatumType::String:
//		return &stringArr[idx];
//		break;
//	case DatumType::Vec4:
//		return &vecArr[idx];
//		break;
//	case DatumType::Mat4x4:
//		return &matrixArr[idx];
//		break;
//	default:
//		break;
//	}
//
//	return nullptr;
//}