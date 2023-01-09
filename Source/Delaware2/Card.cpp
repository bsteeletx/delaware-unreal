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

	Front = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Front Material"));
	Back = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Back Material"));
	TrumpFront = CreateDefaultSubobject<UMaterialInterface>(TEXT("Trump Card Front Material"));

	TSubclassOf<UStaticMeshComponent> MeshComponent;

	if (Back != nullptr)
	{
		GetStaticMeshComponent()->SetMaterial(0, Back);
		UStaticMeshComponent* SMFront = static_cast<UStaticMeshComponent*>(GetStaticMeshComponent()->GetChildComponent(0));
		SMFront->SetMaterial(0, Front);
	}

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

	if (GetActorLocation().Z < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card %d is off the table!!!"), GetCardID());
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


void ACard::SetMoveDestination(FVector* destination)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Destination to %s"), *destination->ToString());
	FinalDestination = *destination;
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
	
	FRotator CurrentRotation = GetActorRotation();
	SetActorEnableCollision(false);
	TeleportTo(*location, CurrentRotation);
	SetActorEnableCollision(true);
	CurrentLocation = location;
}

void ACard::DealToLocation(FVector* destination)
{
	//How do I give an impulse to a card to vector that points to where I want it to go, but I'm not sure where I'm starting from?
	//I have the location
	//I have the location of the card
	//Vector is destination - locationOfCard

	FVector CardLocation = GetActorLocation();

	FVector ImpulseVector = (*destination - CardLocation)/InverseForceMultiplier; 

	UE_LOG(LogTemp, Warning, TEXT("Adding Impulse to Card %d of %s"), GetCardID(), *ImpulseVector.ToString());
	GetStaticMeshComponent()->AddImpulse(ImpulseVector);
}


//Tried to move them programmatically, was kind of wonky, going with physics
/*
void ACard::MoveToLocation(FVector* location)
{
	FVector Location;

	if (location == nullptr)
	{
		if (MoveLocation.IsZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("location passed to ACard::MoveToLocation is null"));
			return;
		}

		Location = MoveLocation;
	}
	else
	{
		SetMoveDestination(location);
		Location = *location;
	}

	if (GetActorLocation() == Location)
	{
		IsMoving = false;
		UE_LOG(LogTemp, Warning, TEXT("Final Location Reached for Card #%d"), GetCardID());
		return;
	}
	else if (!IsMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempting to move Card #%d, to its ultimate destination, %s"), GetCardID(), *MoveLocation.ToString());
		IsMoving = true;
	}

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector DeltaLocation(0.f);
	DeltaLocation = Location * DeltaTime * CardMoveSpeed; //X = Value * DeltaTime * Speed

	UE_LOG(LogTemp, Warning, TEXT("Card #%d is at %s, trying to move it to %s for now"), GetCardID(), *GetActorLocation().ToString(), *DeltaLocation.ToString());
	
	AddActorWorldOffset(DeltaLocation, false, nullptr, ETeleportType::ResetPhysics);
}
*/

void ACard::Enable()
{
	//GetRootComponent()->SetComponentTickEnabled(true);
	//GetRootComponent()->SetActive(true);
	//SetActorEnableCollision(true);
}

void ACard::Disable()
{
	//GetRootComponent()->SetComponentTickEnabled(false);
	//GetRootComponent()->SetActive(false);
	//SetActorEnableCollision(false);
}