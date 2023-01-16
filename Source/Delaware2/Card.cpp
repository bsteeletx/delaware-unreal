// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
#include "Delaware2GameState.h"
#include "Delaware2GameMode.h"


// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OwnedBy = EPlayers::None;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));

	RootComponent = CardMesh;

	Front = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Front Material"));
	Back = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Back Material"));
	TrumpFront = CreateDefaultSubobject<UMaterialInterface>(TEXT("Trump Card Front Material"));

	SetActorLocation(FVector(0,0,0));

	//OnActorHit.AddDynamic(this, &ACard::OnCardHit);

}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();

	FString ClassName("");
	GetClass()->GetName(ClassName);
	/*
	ERank ThisRank = GetRankFromClassName(ClassName);
	ESuit ThisSuit = GetSuitFromClassName(ClassName);
	SetRank(ThisRank);
	SetSuit(ThisSuit);
	*/
	CardID = CreateCardID();

	//UE_LOG(LogTemp, Warning, TEXT("CardID of class %s is %d"), *ClassName, CardID);
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Card %d is off the table!!!"), GetCardID());
	}
	
	FRotator CurrentRotation = GetActorRotation();
	if (!IsFaceUp)
	{
		CurrentRotation.ClampAxis(33.0);
	}

	ADelaware2GameState* GameState = static_cast<ADelaware2GameState*>(GetWorld()->GetGameState());

	if (GameState != nullptr)
	{
		if (GameState->GetCurrentState() == EGameStates::Dealing)
		{
			if (GetVelocity().IsNearlyZero() && HasHitPlayingArea && !HasBeenPlacedInPlayingArea)
			{
				SetToFinalDestination();
			}
		}
	}
}

FString ACard::GetRankAsString() const
{
	return RanksAsStrings[(int)GetRank()-1];
}

FString ACard::GetSuitAsString() const
{
	return SuitsAsStrings[(int)GetSuit() - 1];
}

FString ACard::GetFullCardName() const
{
	FString Name = GetRankAsString() + " of " + GetSuitAsString();
	return Name;
}

uint8 ACard::CreateCardID()
{
	int SuitAsInt = (int)Suit;
	int RankAsInt = (int)Rank;
	int Counter = GetSuitRankCounter();
	
	int NewID = (SuitAsInt-1) * 20 + (RankAsInt-1) * 4 + Counter; 
	//UE_LOG(LogTemp, Warning, TEXT("Suit %d * 20 + Rank %d * 4 + Counter %d = %d"), SuitAsInt - 1, RankAsInt - 1, Counter, NewID);
	return NewID;
}

uint8 ACard::GetSuitRankCounter()
{
	const FString Label = GetActorNameOrLabel();
	if (Label.Contains("2"))
	{
		return 1;
	}
	if (Label.Contains("3"))
	{
		return 2;
	}
	if (Label.Contains("4"))
	{
		return 3;
	}

	return 0;
}

ERank ACard::GetRankFromClassName(FString className)
{
	if (className.Contains("A"))
	{
		return ERank::Ace;
	}
	if (className.Contains("K"))
	{
		return ERank::King;
	}
	if (className.Contains("Q"))
	{
		return ERank::Queen;
	}
	if (className.Contains("J"))
	{
		return ERank::Jack;
	}

	return ERank::Ten;
}

ESuit ACard::GetSuitFromClassName(FString className)
{
	if (className.Contains("C_C"))
	{
		return ESuit::Clubs;
	}
	if (className.Contains("D"))
	{
		return ESuit::Diamonds;
	}
	if (className.Contains("H"))
	{
		return ESuit::Hearts;
	}

	return ESuit::Spades;
}

void ACard::SetRank(ERank rank)
{
	if (rank != ERank::None)
		Rank = rank;
}

void ACard::SetSuit(ESuit suit)
{
	if (suit != ESuit::None)
		Suit = suit;
}

void ACard::RaiseHeight()
{
	if (!IsRaised)
	{
		FVector Location = GetActorLocation();

		Location.Z += HoverRaiseAmount;
		Location.Y -= HoverRaiseAmount;

		SetActorLocation(Location);

		IsRaised = true;
	}
}

void ACard::ResetHeight()
{
	if (IsRaised)
	{
		FVector Location = GetActorLocation();

		Location.Z -= HoverRaiseAmount;
		Location.Y += HoverRaiseAmount;

		SetActorLocation(Location);

		IsRaised = false;
	}
}

void ACard::Flip()
{
	/*if (IsFaceUp)
	{
		GetStaticMeshComponent()->SetMaterial(0, Back);
	}
	else
	{
		if (IsTrumpSuit)
		{
			GetStaticMeshComponent()->SetMaterial(0, TrumpFront);
		}
		else
		{
			GetStaticMeshComponent()->SetMaterial(0, Front);
		}
	}*/
}

void ACard::ToggleTrump()
{
	IsTrumpSuit = !IsTrumpSuit;
}

UMaterialInterface* ACard::GetFront()
{
	if (Front != nullptr)
		return Front;

	return nullptr;
}

UMaterialInterface* ACard::GetTrump()
{
	if (TrumpFront != nullptr)
		return TrumpFront;
	
	return nullptr;
}

ERank ACard::GetRank() const
{
	return Rank;
}

ESuit ACard::GetSuit() const
{
	return Suit;
}

uint8 ACard::GetCardID() const
{
	return CardID;
}

void ACard::SetToLocation(FVector* location, bool useRotation, FRotator rotation)
{
	if (location == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("location is NULL!"));
		return;
	}
	
	FRotator CurrentRotation = FRotator(0, 0, 0);
	if (!useRotation)
	{
		ADelaware2GameState* GameState = static_cast<ADelaware2GameState*>(GetWorld()->GetGameState());
		if (GameState->GetDealer() == EPlayers::East || GameState->GetDealer() == EPlayers::West)
		{
			CurrentRotation = FRotator(0, 0, 90);
		}
	}
	else
	{
		CurrentRotation = rotation;
	}
	
	const FVector NewLocation = *location;
	if (!SetActorLocation(NewLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was unable to be placed at %s! Placing at %s instead"), *GetFullCardName(), *NewLocation.ToCompactString(), *GetActorLocation().ToCompactString());
	}

	//SetActorEnableCollision(true);
}

void ACard::SlowImpulse()
{
	//UE_LOG(LogTemp, Warning, TEXT("Card Class Stopping Impulse..."));
	CardMesh->AddImpulse(-GetVelocity() / InverseForceMultiplier / 2);
		
	HasHitPlayingArea = true;
}

void ACard::SetToFinalDestination()
{
	if (FinalDestination == FVector::Zero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Final Destination is 0!"));
		return;
	}

	if (GetVelocity().Equals(FVector::ZeroVector), 1.0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Placing %s of %s at Final Destination, %s"), *RanksAsStrings[(int)Rank-1], *SuitsAsStrings[(int)Suit-1], *FinalDestination.ToCompactString());
		FRotator Rotation;
		if (GetPlayerOwner() == EPlayers::North || GetPlayerOwner() == EPlayers::South)
		{
			Rotation = FRotator::ZeroRotator;
		}
		else
		{
			Rotation = FRotator(0, 90, 0);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Rotation is set to %s"), *Rotation.ToCompactString());
		SetActorRotation(Rotation);
		SetToLocation(&FinalDestination); 
		SetActorTickEnabled(false);
		HasHitPlayingArea = false;
		HasBeenPlacedInPlayingArea = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Velocity is NOT zero!"));
	}

	//TODO: Add setting Rotation too
}

void ACard::DealToLocation(FVector* destination)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attempting to send %s to %s"), *GetFullCardName(), *destination->ToCompactString());

	FVector CardLocation = GetActorLocation();

	FVector ImpulseVector = (*destination - CardLocation)/InverseForceMultiplier; 

	FinalDestination = *destination;

	//UE_LOG(LogTemp, Warning, TEXT("Adding Impulse to %s of %s"), *GetFullCardName(), *ImpulseVector.ToString());
	CardMesh->AddImpulse(ImpulseVector);
	//GetStaticMeshComponent()->AddImpulse(ImpulseVector);
}

void ACard::SetPlayerOwner(EPlayers owner)
{
	OwnedBy = owner;
}

EPlayers ACard::GetPlayerOwner()
{
	return OwnedBy;
}