// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
#include "Delaware2GameModeBase.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Front = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Front Material"));
	Back = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Back Material"));
	TrumpFront = CreateDefaultSubobject<UMaterialInterface>(TEXT("Trump Card Front Material"));

	if (Back != nullptr)
		GetStaticMeshComponent()->SetMaterial(0, Back);

	SetActorLocation(FVector(0,0,0));
}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();

	CardID = (int)Suit * 20 + (int)Rank;
}

// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving)
	{
		MoveToLocation(MoveLocation);
	}
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
	if (IsFaceUp)
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
	}
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

void ACard::SetToLocation(FVector* location)
{
	if (location == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("location is NULL!"));
		return;
	}
	FVector TempVector = FVector::Zero();

	TempVector.X = location->X;
	TempVector.Y = location->Y;
	TempVector.Z = location->Z;

	FString VectorLocation = TempVector.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Sending Card ID %d to: %s"), GetCardID(), *TempVector.ToString());
	
	SetActorLocation(TempVector);
	CurrentLocation = location;
}

void ACard::MoveToLocation(FVector* location)
{
	if (location == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("location passed to ACard::MoveToLocation is null"));
		return;
	}
	MoveLocation = location;
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (GetActorLocation() == *location)
	{
		IsMoving = false;
		return;
	}
	else if (!IsMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting to move Card #%d"), GetCardID());
		IsMoving = true;
	}

	FVector DeltaLocation(0.f);
	DeltaLocation = *location * DeltaTime * CardMoveSpeed; //X = Value * DeltaTime * Speed
	
	AddActorWorldOffset(DeltaLocation);
}

void ACard::Enable()
{
	GetRootComponent()->SetComponentTickEnabled(true);
	GetRootComponent()->SetActive(true);
	SetActorEnableCollision(true);
}

void ACard::Disable()
{
	GetRootComponent()->SetComponentTickEnabled(false);
	GetRootComponent()->SetActive(false);
	SetActorEnableCollision(false);
}