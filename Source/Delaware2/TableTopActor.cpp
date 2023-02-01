// Fill out your copyright notice in the Description page of Project Settings.


#include "TableTopActor.h"
#include "Components/BoxComponent.h"
#include "Delaware2GameState.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

// Sets default values
ATableTopActor::ATableTopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComponent;

	TableTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table Top Mesh"));
	TableTopMesh->SetupAttachment(RootComponent);

	TrumpSelectionSymbol = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Trump Symbol-All"));
	TrumpSelectionSymbol->SetupAttachment(TableTopMesh);
	TrumpSelectionSymbol->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATableTopActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATableTopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

