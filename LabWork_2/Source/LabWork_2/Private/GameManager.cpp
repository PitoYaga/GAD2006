// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "TBPlayerController.h"
#include "Command/Command.h"
#include "Command/MoveCommand.h"
#include "Misc/LowLevelTestAdapter.h"

// Sets default values
AGameManager::AGameManager():

	CurrentLevel(0),
	GameGrid(nullptr),
	ThePlayer(nullptr)


{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::OnActorClicked(AActor* Actor, FKey* Key)
{
	if(CurrentCommand.IsValid() && CurrentCommand->IsExecuting()) return;

	AGameSlot* Slot = Cast<AGameSlot>(Actor);

	if(!Slot) return;

	if (!ThePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected!"))
		return;
	}

	if (Slot->Unit == nullptr)
	{
		TShaderRef<MoveCommand> Cmd = MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, Slot->GridPosition);
		CommandPool.Add(Cmd);
		Cmd->Execute();
		CurrentCommand = Cmd;
	}
}

void AGameManager::CreateLevelActors(FSLevelInfo& LevelInfo)
{
	ThePlayer = nullptr;

	for (auto UnitInfo : LevelInfo.Units)
	{
		
		if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
		{
			Slot->SpawnUnitHere(UnitInfo.UnitClass);
			if (Slot->Unit && Slot->Unit->IsControlledByPlayer())
			{
				ThePlayer = Slot->Unit;
			}
		}
	}
	
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->GameManager = this;
	}

	if (Levels.IsValidIndex(CurrentLevel))
	{
		CreateLevelActors(Levels[CurrentLevel]);
	}
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCommand.IsValid())
	{
		CurrentCommand->Update(DeltaTime);
	}
	
}

