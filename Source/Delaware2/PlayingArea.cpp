// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingArea.h"
//include debug
#include "DrawDebugHelpers.h"
#include "Card.h"
#include "Delaware2GameState.h"

//debug logging
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT(text), fstring))

APlayingArea::APlayingArea()
{
	OnActorBeginOverlap.AddDynamic(this, &APlayingArea::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APlayingArea::OnOverlapEnd);

	
}

void APlayingArea::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Cyan, true, -1, 0, 5);

	NameOfPlayingArea = GetActorNameOrLabel();
}

void APlayingArea::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && (otherActor != this))
	{
		ACard* Card = static_cast <ACard*>(otherActor);

		EPlayers CardOwner = Card->GetPlayerOwner();

		//UE_LOG(LogTemp, Warning, TEXT("Checking to see if CardOwner's first letter %c, is equal to the trigger label's first letter %c"), EPlayerAsString[(int)CardOwner - 1][0], NameOfPlayingArea[0]);

		if (NameOfPlayingArea.Len() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Name of Playing Area is NULL!"));
		}
		else if (EPlayerAsString[(int)CardOwner - 1].Len() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Name of PlayerAsString is NULL"));
		}
		else if (EPlayerAsString[(int)CardOwner - 1][0] == NameOfPlayingArea[0])
		{
			print("Trigger Stopping Impulse..");
			//Card->SetToFinalDestination();
			Card->SlowImpulse();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("CardOwner is %s NameOfPlayingArea is %s"), *EPlayerAsString[(int)CardOwner - 1], *NameOfPlayingArea);
		}
	}
}

void APlayingArea::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	
}