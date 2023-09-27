// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CT_Item.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CT_CraftingMachine.generated.h"

UCLASS()
class FPCRAFTING_API ACT_CraftingMachine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACT_CraftingMachine();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* OverlapComponent;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* UserMappingContext;

	UPROPERTY(EditAnywhere)
	class UInputAction* OpenMachineAction;

	UPROPERTY(EditAnywhere)
	TArray<int> RecipesID;

	UFUNCTION(BlueprintImplementableEvent, Category="Interactable")
	void OnOpenMachine(const TArray<int>& recipesIDArray, const TArray<int>& currentItemsInProximityID);

	UFUNCTION(BlueprintImplementableEvent, Category="Auxiliar")
	void ToggleShowOpenMachineText(const bool show);

	UPROPERTY(BlueprintReadWrite)
	FVector locationToSpawnItems;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<ACT_Item*> ItemsInProximity;
	bool hasShownUIOpenMachineText;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OpenMachine();

	UFUNCTION(BlueprintCallable)
	virtual ACT_CraftingMachine* CraftItem(TSubclassOf<class ACT_Item> ItemToSpawn,
	                                       TArray<int> ItemsInProximityToRemoveID);

	UFUNCTION(BlueprintCallable)
	virtual ACT_CraftingMachine* CraftItemWithoutExpendingItems(TSubclassOf<class ACT_Item> ItemToSpawn);
};
