// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delaware2GameMode.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum class ERank : uint8 { None = 0, Jack, Queen, King, Ten, Ace, All, Any };

const static ERank AllRanks[] = { ERank::Jack, ERank::Queen, ERank::King, ERank::Ten, ERank::Ace };

const static FString RanksAsStrings[] = { "Jack", "Queen", "King", "Ten", "Ace" };

UENUM(BlueprintType)
enum class ESuit : uint8 { None = 0, Diamonds, Clubs, Hearts, Spades, All, Any };

const static ESuit AllSuits[] = { ESuit::Diamonds, ESuit::Clubs,  ESuit::Hearts, ESuit::Spades };

const static FString SuitsAsStrings[] = { "Diamonds", "Clubs", "Hearts", "Spades" };

UCLASS()
class DELAWARE2_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ACard(char suitNumber, char rankNumber);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	void DealToLocation(FVector* destination);
	
	void Flip();

	uint8 GetCardID() const;
	FVector* GetFinalDestination();
	EPlayers GetPlayerOwner();
	ERank GetRank() const;
	ESuit GetSuit() const;
	FString GetRankAsString() const;
	FString GetSuitAsString() const;
	FString GetFullCardName() const;

	void RaiseHeight();
	void ResetHeight();
	void Rotate(float zValue);

	void SetCollision(bool enableCollision);
	void SetFinalDestination(FVector* newDestination);
	void SetPhysics(bool enablePhysics);
	void SetPlayerOwner(EPlayers player);
	void SetRank(ERank rank);
	void SetSuit(ESuit suit);
	void SetToLocation(FVector* location, bool useCurrentRotation = false, FRotator rotation = FRotator::ZeroRotator);
	void SlowImpulse();
	

	void ToggleTrump();

private:

	uint8 CreateCardID();
	ERank GetRankFromClassName(FString className);
	ESuit GetSuitFromClassName(FString className);
	uint8 GetSuitRankCounter();

	UFUNCTION()
	class UMaterialInterface* GetFront();

	UFUNCTION()
	class UMaterialInterface* GetTrump();

	//////////////////////////////////

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Front;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Back;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* TrumpFront;
	
	UPROPERTY(EditAnywhere)
	ESuit Suit;

	UPROPERTY(EditAnywhere)
	ERank Rank;

	UPROPERTY(VisibleAnywhere)
	uint8 CardID;

	UPROPERTY(EditAnywhere)
	float HoverRaiseAmount = 25.f;

	UPROPERTY(EditAnywhere)
	float CardMoveSpeed = 25.f;

	bool IsTrumpSuit = false;
	bool IsFaceUp = false;
	bool IsRaised = false;
	bool HitPlayArea = false;
	bool HasFiredSort = false;

	FVector FinalDestination = { 0.0, 0.0, 0.0 };

	UPROPERTY(EditAnywhere)
	float InverseForceMultiplier = 5.f;

	UPROPERTY(VisibleAnywhere)
		EPlayers OwnedBy;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CardMesh;
};