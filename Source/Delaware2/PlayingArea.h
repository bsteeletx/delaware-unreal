// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "PlayingArea.generated.h"

/**
 * 
 */
UCLASS()
class DELAWARE2_API APlayingArea : public ATriggerVolume
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	APlayingArea();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

private:

	FString NameOfPlayingArea;
	//class ACard* SpecificCardType;

};
