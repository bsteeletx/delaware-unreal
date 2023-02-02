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

	if (GetActorLocation().Z < -100 && OwnedBy == EPlayers::North)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card %s, owned by %s, is off the table!!!"), *GetFullCardName(), *EPlayerAsString[(int)OwnedBy - 1]);
	}
	
	FRotator CurrentRotation = GetActorRotation();
	//if (!IsFaceUp)
	//{
	//	CurrentRotation.ClampAxis(33.0);
	//}

	ADelaware2GameState* GameState = static_cast<ADelaware2GameState*>(GetWorld()->GetGameState());

	if (GameState != nullptr)
	{
		if (GameState->GetCurrentState() == EGameStates::Dealing) //need to take this out, I think--make sure it's mentioned somewhere else (
		{
			if (GetVelocity().IsNearlyZero(1) && GetPlayerOwner() != EPlayers::None && HitPlayArea && !HasFiredSort)
			{
				GameState->CardReadyToSort(GetPlayerOwner());
				HasFiredSort = true;
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

void ACard::SetFinalDestination(FVector* newDestination)
{
	UE_LOG(LogTemp, Warning, TEXT("Final Destination WAS %s"), *FinalDestination.ToCompactString());
	FinalDestination = *newDestination;
	UE_LOG(LogTemp, Warning, TEXT("Final Destination is NOW %s"), *FinalDestination.ToCompactString());
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
	FRotator Rotation = GetActorRotation();
	Rotation.Pitch = 180.f;
	if (GetPlayerOwner() == EPlayers::North)
	{
		UE_LOG(LogTemp, Warning, TEXT("Flipping %s"), *GetFullCardName())
	}
	SetActorRotation(Rotation);
	IsFaceUp = !IsFaceUp;
}

void ACard::Rotate(float zValue)
{
	FRotator Rotation(FRotator::ZeroRotator);
	Rotation.Yaw = zValue;
	
	if (GetPlayerOwner() == EPlayers::North)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotating %s's %s %f degrees, new rotation is: %s"), *EPlayerAsString[(int)GetPlayerOwner() - 1], *GetFullCardName(), Rotation.Yaw, *Rotation.ToCompactString());
	}
	SetActorRotation(Rotation);
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

void ACard::SetCollision(bool enableCollision)
{
	SetPhysics(enableCollision);

	if (enableCollision)
	{
		CardMesh->SetCollisionProfileName(FName("PhysicsActor"));
	}
	else
	{
		CardMesh->SetCollisionProfileName(FName("NoCollision"));
	}
}

void ACard::SetPhysics(bool enablePhysics)
{
	CardMesh->SetSimulatePhysics(enablePhysics);
}

void ACard::SetToLocation(FVector* location, bool useCurrentRotation, FRotator rotation)
{
	if (location == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("location is NULL!"));
		return;
	}
	
	FRotator CurrentRotation = FRotator(0, 0, 0);
	if (!useCurrentRotation)
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
		//UE_LOG(LogTemp, Warning, TEXT("%s was unable to be placed at %s! Placing at %s instead"), *GetFullCardName(), *NewLocation.ToCompactString(), *GetActorLocation().ToCompactString());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s was placed at %s"), *GetFullCardName(), *NewLocation.ToCompactString());
	}

	//SetActorEnableCollision(true);
}

void ACard::SlowImpulse()
{
	if (!HitPlayArea)
	{ 	//UE_LOG(LogTemp, Warning, TEXT("Card Class Stopping Impulse..."));
		CardMesh->AddImpulse(-GetVelocity() / InverseForceMultiplier / 2);
		HitPlayArea = true;
	}
}

void ACard::DealToLocation(FVector* destination)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attempting to send %s to %s"), *GetFullCardName(), *destination->ToCompactString());

	FVector CardLocation = GetActorLocation();

	FVector ImpulseVector = (*destination - CardLocation)/InverseForceMultiplier; 

	FinalDestination = *destination;
	
	//UE_LOG(LogTemp, Warning, TEXT("Adding Impulse to %s of %s"), *GetFullCardName(), *ImpulseVector.ToString());
	CardMesh->AddImpulse(ImpulseVector); //-1575 + 185
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

FVector* ACard::GetFinalDestination()
{
	return &FinalDestination;
}