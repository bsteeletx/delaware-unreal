// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Delaware2GameMode.h"
#include "Card.generated.h"

UCLASS()
class DELAWARE2_API ACard : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRank(ERank rank);
	void SetSuit(ESuit suit);

	//ACard(char suitNumber, char rankNumber);
	uint8 GetCardID() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void RaiseHeight();
	void ResetHeight();

	void Flip();

	void ToggleTrump();

	ERank GetRank() const;
	ESuit GetSuit() const;

	void SetToLocation(FVector* location);
	void DealToLocation(FVector* destination);
	void Enable();
	void Disable();


private:

	UFUNCTION()
	class UMaterialInterface* GetFront();

	UFUNCTION()
	class UMaterialInterface* GetTrump();

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

	bool IsMoving = false;
	bool IsTrumpSuit = false;
	bool IsFaceUp = false;
	bool IsRaised = false;

	FVector* CurrentLocation;

	FVector FinalDestination = { 0.0, 0.0, 0.0 };

	void SetMoveDestination(FVector* destination);

	FVector GetDealOffset(EPlayers dealer);

	UPROPERTY(EditAnywhere)
	float InverseForceMultiplier = 1000.0f;
};
