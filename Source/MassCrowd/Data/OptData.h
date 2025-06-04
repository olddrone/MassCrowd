// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)
enum class EOptState : uint8
{
	EOS_Default		UMETA(DisplayName = "Default"),
	EOS_First		UMETA(DisplayName = "First"),
	EOS_Second 		UMETA(DisplayName = "Second"),
	EOS_Third 		UMETA(DisplayName = "Third")
};

namespace OptArea
{
	const float Default = 800.f;
	const float First = 1600.f;
	const float Second = 2400.f;
};