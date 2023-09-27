// Fill out your copyright notice in the Description page of Project Settings.


#include "CT_CraftingMachine.h"

#include "CT_Item.h"
#include "EnhancedInputSubsystems.h"
#include "FPCraftingCharacter.h"

// Sets default values
ACT_CraftingMachine::ACT_CraftingMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACT_CraftingMachine::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	hasShownUIOpenMachineText = false;

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
			&EnhancedInputComponent->BindAction(OpenMachineAction, ETriggerEvent::Completed, this,
			                                    &ACT_CraftingMachine::OpenMachine);
		}
	}
}

// Called every frame
void ACT_CraftingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Result;
	OverlapComponent->GetOverlappingActors(Result, ACT_Item::StaticClass());

	if (Result.Num() != ItemsInProximity.Num())
	{
		ItemsInProximity.Empty();

		for (AActor* Item : Result)
		{
			if (ACT_Item* cItem = Cast<ACT_Item>(Item))
			{
				ItemsInProximity.Add(cItem);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Count: %d"), ItemsInProximity.Num());

	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	float DistanceFromPlayer = FVector::Distance(GetActorLocation(), PlayerPosition);

	if (DistanceFromPlayer <= 400 && !hasShownUIOpenMachineText)
	{
		ToggleShowOpenMachineText(true);
		hasShownUIOpenMachineText = true;
	}
	else if (DistanceFromPlayer > 400 && hasShownUIOpenMachineText)
	{
		ToggleShowOpenMachineText(false);
		hasShownUIOpenMachineText = false;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Count: %d"), Result.Num());
}

void ACT_CraftingMachine::OpenMachine()
{
	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	float DistanceFromPlayer = FVector::Distance(GetActorLocation(), PlayerPosition);

	if (DistanceFromPlayer > 400) return;

	TArray<int> ItemsInProximityID;
	ItemsInProximityID.Empty();

	for (ACT_Item* Item : ItemsInProximity)
	{
		ItemsInProximityID.Add(Item->ItemTypeID);
	}

	OnOpenMachine(RecipesID, ItemsInProximityID);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	// PlayerController->bEnableClickEvents = true;
	// PlayerController->bEnableMouseOverEvents = true;
}

ACT_CraftingMachine* ACT_CraftingMachine::CraftItem(TSubclassOf<ACT_Item> ItemToSpawn,
                                                    TArray<int> ItemsInProximityToRemoveID)
{
	TArray<ACT_Item*> ItemsInProximityToCheck = ItemsInProximity;
	TArray<ACT_Item*> ItemsInProximityToRemove;

	for (int ItemID : ItemsInProximityToRemoveID)
	{
		for (ACT_Item* cItem : ItemsInProximityToCheck)
		{
			if (ItemID == cItem->ItemTypeID)
			{
				ItemsInProximityToRemove.Add(cItem);
				ItemsInProximityToCheck.Remove(cItem);
				break;
			}
		}
	}

	if (ItemsInProximityToRemoveID.Num() != ItemsInProximityToRemove.Num()) return this;

	for (ACT_Item* cItem : ItemsInProximityToRemove)
	{
		cItem->Destroy();
	}

	ItemsInProximity.Empty();
	ItemsInProximity = ItemsInProximityToCheck;

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<ACT_Item>(ItemToSpawn, locationToSpawnItems, GetActorRotation(), spawnParameters);

	return this;
}

ACT_CraftingMachine* ACT_CraftingMachine::CraftItemWithoutExpendingItems(TSubclassOf<ACT_Item> ItemToSpawn)
{
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<ACT_Item>(ItemToSpawn, locationToSpawnItems, GetActorRotation(), spawnParameters);

	return this;
}
