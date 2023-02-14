// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
//#include "Delaware2GameState.h"
#include "TableTopActor.h"
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

ACard::ACard(ERank rank, ESuit suit)
{
	Rank = rank;
	Suit = suit;
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

	if (CurrentRotation.Pitch > 7.5f)
	{
		CurrentRotation.Pitch = 7.5f;
	}
	else if (CurrentRotation.Pitch < -7.5f)
	{
		CurrentRotation.Pitch = -7.5f;
	}
	if (CurrentRotation.Roll > 7.5f)
	{
		CurrentRotation.Roll = 7.5f;
	}
	else if (CurrentRotation.Roll < -7.5f)
	{
		CurrentRotation.Roll = -7.5f;
	}
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
				GameState->TellTableToSortPlayerCards(GetPlayerOwner());
				HasFiredSort = true;
			}
		}
	}
}

/// <summary>
/// Returns this cards rank as a string
/// </summary>
/// <returns>
/// </returns>
FString ACard::GetRankAsString() const
{
	return RanksAsStrings[(int)GetRank()-1];
}


/// <summary>
/// Returns this card's suit as a string
/// </summary>
/// <returns></returns>
FString ACard::GetSuitAsString() const
{
	return SuitsAsStrings[(int)GetSuit() - 1];
}

/// <summary>
/// Returns a string that provides a reasonable definition of what the card is (i.e., "Jack of Clubs")
/// </summary>
/// <returns></returns>
FString ACard::GetFullCardName() const
{
	FString Name = GetRankAsString() + " of " + GetSuitAsString();
	return Name;
}

/// <summary>
/// Sets Final Destination to a new value
/// </summary>
/// <param name="newDestination">The new FinalDestination</param>
void ACard::SetFinalDestination(FVector* newDestination)
{
	FinalDestination = *newDestination;
}

/// <summary>
/// Creates a CardID with the equation: Suit*20 + Rank*4 + CardCopyNumber (there are 3 copies of every card)
/// </summary>
/// <returns>uint8 of the new CardID</returns>
uint8 ACard::CreateCardID()
{
	int SuitAsInt = (int)Suit;
	int RankAsInt = (int)Rank;
	int Counter = GetSuitRankCounter();
	
	int NewID = (SuitAsInt-1) * 20 + (RankAsInt-1) * 4 + Counter; 
	return NewID;
}

/// <summary>
/// Finds the copy # of the card
/// </summary>
/// <returns>uint8 of the copy number of the card</returns>
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

/// <summary>
/// Goes through the className and searches for the in game name of the Card and returns the ERank of the card
/// </summary>
/// <param name="className">In Game name of the Card</param>
/// <returns>ERank of the Card</returns>
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

/// <summary>
/// Goes through the className and searches for the in-game name of the Card and returns the ESuit of the card
/// </summary>
/// <param name="className">In-Game name of the Card</param>
/// <returns>ESuit of the Card</returns>
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

/// <summary>
/// old function from previous version of game. Not sure if it will be used in this one or not.
/// was called when player touched a card. Would cause the card to raise up from his hand just a bit to let the player know which card was selected
/// </summary>
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

/// <summary>
/// old function from previous version of game. Not sure if it will be used in this one or not.
/// was called when player was touching this card, but then touched another card. This would reset the card back to its original position
/// </summary>
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

/// <summary>
/// Takes card and flips it 180 degree so that it is face up instead of face down or vice versa
/// </summary>
void ACard::Flip()
{
	FRotator Rotation = GetActorRotation();

	if (IsFaceUp)
	{
		Rotation.Pitch = -180.f;
	}
	else
	{
		Rotation.Pitch = 180.f;
	}
	
	SetActorRotation(Rotation);
	IsFaceUp = !IsFaceUp;
}

/// <summary>
/// Rotates Card to new rotation value--Yaw only
/// </summary>
/// <param name="zValue">The new yaw value to rotate to</param>
void ACard::Rotate(float zValue)
{
	FRotator Rotation(FRotator::ZeroRotator);
	Rotation.Yaw = zValue;
	SetActorRotation(Rotation);
}

/// <summary>
/// not currently used, we'll see if I use it when I get to it
/// was to flip boolean telling whether this card's suit was named trump or not
/// </summary>
void ACard::ToggleTrump()
{
	IsTrumpSuit = !IsTrumpSuit;
}

/// <summary>
/// not currently used, but gets the Front, non-trump value of Material Interface
/// </summary>
/// <returns>Pointer to Material Interface that represents the front, non-trump version of this card</returns>
UMaterialInterface* ACard::GetFront()
{
	if (Front != nullptr)
		return Front;

	return nullptr;
}

/// <summary>
/// not currently used, but get the Trump version of the Material Interface for this card
/// </summary>
/// <returns>Pointer to Material Interface that represents the trump version of this card</returns>
UMaterialInterface* ACard::GetTrump()
{
	if (TrumpFront != nullptr)
		return TrumpFront;
	
	return nullptr;
}

/// <summary>
/// returns the ERank of this card
/// </summary>
/// <returns></returns>
ERank ACard::GetRank() const
{
	return Rank;
}

/// <summary>
/// returns the ESuit of this card
/// </summary>
/// <returns></returns>
ESuit ACard::GetSuit() const
{
	return Suit;
}

/// <summary>
/// returns the CardID of this card
/// </summary>
/// <returns></returns>
uint8 ACard::GetCardID() const
{
	return CardID;
}

/// <summary>
/// Enables/Disables Physics, then collision
/// </summary>
/// <param name="enableCollision">set to true if you want collision</param>
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

/// <summary>
/// Enables/Disables Physics
/// </summary>
/// <param name="enablePhysics">true to enable Physics</param>
void ACard::SetPhysics(bool enablePhysics)
{
	CardMesh->SetSimulatePhysics(enablePhysics);
}

/// <summary>
/// Sets Card to location
/// </summary>
/// <param name="location">where card is to be placed</param>
/// <param name="useCurrentRotation">true if you're not changine the rotation</param>
/// <param name="rotation">if useCurrentRotation is false, sets the cards rotation to this value</param>
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

/// <summary>
/// Puts brakes on cards, currently only if they hit the players play area
/// </summary>
void ACard::SlowImpulse()
{
	if (!HitPlayArea)
	{ 	
		CardMesh->AddImpulse(-GetVelocity() / InverseForceMultiplier / 2);
		HitPlayArea = true;
	}
}

/// <summary>
/// Sends an impulse to a card that drives it towards a location. Sets FinalDestination in case it doesn't quite get there
/// </summary>
/// <param name="destination">where to aim the impulse</param>
void ACard::DealToLocation(FVector* destination)
{
	FVector CardLocation = GetActorLocation();
	FVector ImpulseVector = (*destination - CardLocation)/InverseForceMultiplier; 
	SetFinalDestination(destination);
	
	CardMesh->AddImpulse(ImpulseVector); 
}

/// <summary>
/// Sets the OwnedBy variable, which denotes which player controls card
/// </summary>
/// <param name="owner">which player is the new owner</param>
void ACard::SetPlayerOwner(EPlayers owner)
{
	OwnedBy = owner;
}

/// <summary>
/// Returns Card's player owner
/// </summary>
/// <returns></returns>
EPlayers ACard::GetPlayerOwner()
{
	return OwnedBy;
}

/// <summary>
/// Returns Card's Final Destination
/// </summary>
/// <returns></returns>
FVector* ACard::GetFinalDestination()
{
	return &FinalDestination;
}