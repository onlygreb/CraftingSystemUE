// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"
#include "CT_Item.generated.h"

UCLASS()
class FPCRAFTING_API ACT_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACT_Item();

	UPROPERTY(EditAnywhere)
	int ItemTypeID;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* UserMappingContext;
	
	UPROPERTY(EditAnywhere)
	class UInputAction* DropAction;
	
	UPROPERTY(EditAnywhere)
	class UInputAction* GrabAction;

	static inline int CurrentDraggableItemID;
	static inline int CurrentIDIndex = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsAttached;

	FEnhancedInputActionEventBinding* DropActionBinding;
	FEnhancedInputActionEventBinding* GrabActionBinding;

	float DistanceFromPlayer;
	int ItemGeneratedID;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void GrabItem();

	virtual void DropItem();
};
