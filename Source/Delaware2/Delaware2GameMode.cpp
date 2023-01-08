
// Fill out your copyright notice in the Description page of Project Settings.

#include "Delaware2GameMode.h"
#include "Card.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"



ADelaware2GameMode::ADelaware2GameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EGameStates::None;

	Dealer = EPlayers::South;
	DeckCounter = 0;
	HandCounter = 0;

	DealLocations.Reserve(4);
}

void ADelaware2GameMode::BeginPlay()
{

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay Function called!"));


	/*for (ACard* Card : TActorRange<ACard>(GetWorld()))
	{
		Deck.Add(Card);
	}

	for (ATargetPoint* Target : TActorRange<ATargetPoint>(GetWorld()))
	{
		FString Label = Target->GetActorNameOrLabel();
		FVector Location = Target->GetActorLocation();

		if (Label.Equals(TEXT("NorthCardStartLoc")))
		{
			DealLocations.Add(EPlayers::North, Location);
			continue;
		}
		if (Label == TEXT("EastCardStartLoc"))
		{
			DealLocations.Add(EPlayers::East, Location);
			continue;
		}
		if (Label == TEXT("SouthCardStartLoc"))
		{
			DealLocations.Add(EPlayers::South, Location);
			continue;
		}
		if (Label == TEXT("WestCardStartLoc"))
		{
			DealLocations.Add(EPlayers::West, Location);
			continue;
		}
	}*/


}

void ADelaware2GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (CurrentState == EGameStates::Dealing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dealing Card %d"), DeckCounter);

		if (DeckCounter % 4 != 0)
		{
			DealCard();
		}
		else if (DealTime > DealDelay && DeckCounter % 4 == 0 && DeckCounter < 80)
		{
			DealCard();
			DealTime = 0;
		}
		else if (DeckCounter < 80)
		{
			DealTime += DeltaTime;
		}
		else
		{
			CurrentState = EGameStates::None; //TODO: Change to something reasonable after getting deal right
		}

		DeckCounter++;
	}
}

void ADelaware2GameMode::DealCard()
{
	if (DeckCounter % 4 == 0)
	{
		if (DeckCounter == 0)
		{
			PlayerToDealTo = ++Dealer;
		}
		else
		{
			++PlayerToDealTo;
		}
	}

	ACard* CardToDeal = GetNextCard();

	if (DealLocations.Num() != 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("DealLocations TMap not set!"));
		return;
	}

	FVector LocationToDealTo = DealLocations[PlayerToDealTo];

	UE_LOG(LogTemp, Warning, TEXT("LocationToDealTo is %s"), *LocationToDealTo.ToString())

		//have to raise the card by 0.2 and the offset by width and multiply each by the (int)(DeckCounter / 16) + (int)DeckCounter % 4 : West(DeckCounter, Z): 
		GetDealingOffset(LocationToDealTo);

	//CardToDeal->DealToLocation(&LocationToDealTo);

}

void ADelaware2GameMode::Reset()
{
	Shuffle();

	if ((int)Dealer + 1 > (int)EPlayers::West)
	{
		Dealer = EPlayers::North; //reset to beginning
	}
	else
	{
		Dealer = (EPlayers)((int)(Dealer)+1);
	}

	DeckCounter = 0;
	HandCounter = 0;
}

uint8 ADelaware2GameMode::GetDeckCounter()
{
	return HandCounter - 1;
}

void ADelaware2GameMode::SetDeckCounter(uint8 counter)
{
	HandCounter = counter;
}

void ADelaware2GameMode::Shuffle()
{
	uint8 Random;

	for (int i = 0; i < 14; i++)
	{
		for (int j = 79; j > 0; j--)
		{
			Random = FMath::RandRange(0, j);

			Swap<ACard*>(Deck[j], Deck[Random]);
		}
	}
}

ACard* ADelaware2GameMode::GetCardByID(uint8 value)
{
	return Deck[value];
}

void ADelaware2GameMode::GetDealingOffset(FVector& locationToDealTo)
{
	bool DealingHorizontal = (int)PlayerToDealTo % 2 == 1;
	bool DealingReverse = (int)PlayerToDealTo % 2 == 0;

	float MoveAmount = ((int)(DeckCounter / 16) + (int)DeckCounter % 4) * CardSpacing;
	float DepthAmount = CardSpacingByDepth * ((int)(DeckCounter / 16) + (int)DeckCounter % 4);

	if (DealingReverse)
	{
		MoveAmount = -MoveAmount;
	}

	if (DealingHorizontal)
	{
		locationToDealTo.X += MoveAmount;
	}
	else
	{
		locationToDealTo.Y += MoveAmount;
	}

	locationToDealTo.Z = DepthAmount;
}

ACard* ADelaware2GameMode::GetNextCard()
{
	return GetCardByID(DeckCounter++);
}

EPlayers ADelaware2GameMode::GetDealer()
{
	return Dealer;
}

void ADelaware2GameMode::IncrementDealer()
{
	++Dealer;
}

EGameStates ADelaware2GameMode::GetCurrentState()
{
	return CurrentState;
}
