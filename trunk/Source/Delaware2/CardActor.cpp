// Fill out your copyright notice in the Description page of Project Settings.


#include "CardActor.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
#include "Delaware2GameModeBase.h"

// Sets default values
ACardActor::ACardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComponent;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card Mesh"));
	CardMesh->SetupAttachment(BoxComponent);

	Front = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Front Material"));
	Back = CreateDefaultSubobject<UMaterialInterface>(TEXT("Card Back Material"));
	TrumpFront = CreateDefaultSubobject<UMaterialInterface>(TEXT("Trump Card Front Material"));

	if (Back != nullptr)
		CardMesh->SetMaterial(0, Back);

	SetActorLocation(FVector(0,0,0));
}

// Called when the game starts or when spawned
void ACardActor::BeginPlay()
{
	Super::BeginPlay();

	CardID = (int)Suit * 20 + (int)Rank;
}

// Called every frame
void ACardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving)
	{
		MoveToLocation(MoveLocation);
	}
}

void ACardActor::SetRank(ERank rank)
{
	if (Rank != ERank::None)
		Rank = rank;
}

void ACardActor::SetSuit(ESuit suit)
{
	if (Suit != ESuit::None)
		Suit = suit;
}

void ACardActor::RaiseHeight()
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

void ACardActor::ResetHeight()
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

void ACardActor::Flip()
{
	if (IsFaceUp)
	{
		CardMesh->SetMaterial(0, Back);
	}
	else
	{
		if (IsTrumpSuit)
		{
			CardMesh->SetMaterial(0, TrumpFront);
		}
		else
		{
			CardMesh->SetMaterial(0, Front);
		}
	}
}

void ACardActor::ToggleTrump()
{
	IsTrumpSuit = !IsTrumpSuit;
}

UMaterialInterface* ACardActor::GetFront()
{
	if (Front != nullptr)
		return Front;

	return nullptr;
}

UMaterialInterface* ACardActor::GetTrump()
{
	if (TrumpFront != nullptr)
		return TrumpFront;
	
	return nullptr;
}

ERank ACardActor::GetRank() const
{
	return Rank;
}

ESuit ACardActor::GetSuit() const
{
	return Suit;
}

uint8 ACardActor::GetCardID() const
{
	return CardID;
}

void ACardActor::SetToLocation(FVector* location)
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

void ACardActor::MoveToLocation(FVector* location)
{
	if (location == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("location passed to ACardActor::MoveToLocation is null"));
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

void ACardActor::Enable()
{
	GetRootComponent()->SetComponentTickEnabled(true);
	GetRootComponent()->SetActive(true);
	SetActorEnableCollision(true);
}

void ACardActor::Disable()
{
	GetRootComponent()->SetComponentTickEnabled(false);
	GetRootComponent()->SetActive(false);
	SetActorEnableCollision(false);
}