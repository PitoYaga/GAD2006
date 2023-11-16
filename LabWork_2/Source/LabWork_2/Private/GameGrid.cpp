// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"

AGameGrid * AGameGrid::GameGrid = nullptr;

// Sets default values
AGameGrid::AGameGrid():

NumCols(10),
NumRows(10)


{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	GameGrid = this;
}

void AGameGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);


	
	for(auto Grid : GridActors)
	{
		Grid->DestroyComponent();
	}
	GridActors.Empty();

	if (!GridClass-> IsValidLowLevel()) return;

	AGameSlot * Slot = GridClass->GetDefaultObject<AGameSlot>();

	if (Slot == nullptr) return;

	FVector Extends = Slot->Box->GetScaledBoxExtent() * 2;

	for (int r = 0; r < NumRows; r++)
	{
		for (int c = 0; c < NumCols; c++)
		{
			FName GridNAme(FString::Printf(TEXT("Slot%dx%d"), r, c));
			auto Grid = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), GridNAme);
			Grid->RegisterComponent();
			Grid->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Grid->SetRelativeLocation
				(
					FVector((NumRows - r - 1) * Extends.X - (NumRows * 0.5f - 0.5f) * Extends.X,
					c * Extends.Y - (NumCols * 0.5f - 0.5f) * Extends.Y,
					0)
				);
			GridActors.Add(Grid);
			Grid->SetChildActorClass(GridClass);
			AGameSlot * GameSlot = Cast<AGameSlot>(Grid->GetChildActor());

			GameSlot->SetActorLabel(GridNAme.ToString());
			GameSlot->GridPosition.Col = c;
			GameSlot->GridPosition.Row = r;
		}
	}
		
	
	
	

	
}

AGameSlot* AGameGrid::GetSlot(FSGridPosition& Position)
{
	int GridIndex = Position.Row * NumCols + Position.Row;
	if (GridActors.IsValidIndex(GridIndex))
	{
		return Cast<AGameSlot>(GridActors[GridIndex]->GetChildActor());
	}
	return nullptr;
}

AGameSlot* AGameGrid::FindSlot(FSGridPosition Position)
{
	if (GameGrid)
	{
		return  GameGrid->GetSlot(Position);
	}
	return nullptr;
}



// Called when the game starts or when spawned
void AGameGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

