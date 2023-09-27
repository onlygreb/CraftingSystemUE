// Fill out your copyright notice in the Description page of Project Settings.


#include "CTItemSpawner.h"

#include "CT_Item.h"

// Sets default values
ACTItemSpawner::ACTItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTItemSpawner::SpawnItem(TSubclassOf<class ACT_Item> ItemToSpawn, FVector location)
{
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor<ACT_Item>(ItemToSpawn, location, GetActorRotation(), spawnParameters);
}



