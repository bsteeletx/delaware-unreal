// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableTopActor.generated.h"

UCLASS()
class DELAWARE2_API ATableTopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATableTopActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TableTopMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UPaperFlipbookComponent* TrumpSelectionSymbol;

	UPROPERTY(EditAnywhere, Category = "Flipbooks")
	class UPaperFlipbook* SuitSymbols[5];
};
