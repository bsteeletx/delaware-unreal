
// Fill out your copyright notice in the Description page of Project Settings.

#include "Delaware2GameMode.h"
#include "Card.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"



ADelaware2GameMode::ADelaware2GameMode()
{
	CurrentState = EGameStates::None;

	HandCounter = 0;
}

void ADelaware2GameMode::Reset()
{
	HandCounter = 0;
}


