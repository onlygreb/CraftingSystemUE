// Fill out your copyright notice in the Description page of Project Settings.


#include "CT_Item.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPCraftingCharacter.h"

// Sets default values
ACT_Item::ACT_Item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemGeneratedID = CurrentIDIndex;
	CurrentIDIndex++;

	bIsAttached = false;
}

// Called when the game starts or when spawned
void ACT_Item::BeginPlay()
{
	Super::BeginPlay();

	AFPCraftingCharacter* Character = Cast<AFPCraftingCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(UserMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			DropActionBinding = &EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this,
			                                                        &ACT_Item::DropItem);


			GrabActionBinding = &EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this,
			                                                        &ACT_Item::GrabItem);
		}
	}
}

// Called every frame
void ACT_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	DistanceFromPlayer = FVector::Distance(GetActorLocation(), PlayerPosition);

	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	AFPCraftingCharacter* Character = Cast<AFPCraftingCharacter>(Player);

	if (bIsAttached == true)
	{
		SetActorLocation(Player->GetActorLocation() + (Player->GetActorForwardVector() * 180));
	}
	else
	{
		if (DistanceFromPlayer <= 350)
		{
			CurrentDraggableItemID = ItemGeneratedID;
		}
	}
}

void ACT_Item::GrabItem()
{
	if (bIsAttached == true) return;

	if (CurrentDraggableItemID != ItemGeneratedID) return;
	
	if (DistanceFromPlayer > 350) return;

	AFPCraftingCharacter* Character = Cast<AFPCraftingCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character != nullptr)
	{
		if (Character->bIsHoldingAnItem == true) return;

		bIsAttached = true;
	}
}

void ACT_Item::DropItem()
{
	bIsAttached = false;
	if (CurrentDraggableItemID != ItemGeneratedID) return;
	AFPCraftingCharacter* Character = Cast<AFPCraftingCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Character->bIsHoldingAnItem = false;
}
