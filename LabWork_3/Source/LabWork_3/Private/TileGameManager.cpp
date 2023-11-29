// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"

#include "TilePlayerController.h"
#include "GameFramework/PlayerInput.h"

// Sets default values
ATileGameManager::ATileGameManager():

GridSize(100),
GridOffset(0,0,0.5f),
MapExtendsIntGrids(25)

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	GridSelection=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection ->SetupAttachment(RootComponent);

	HoloMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoloMesh"));
	HoloMesh->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridDisplayMaterial(TEXT("Material'/Game/Materials/M_Atlas.M_Atlas'"));
	
	GridSelection ->SetStaticMesh(PlaneMesh.Object);
	GridSelection ->SetMaterial(0,GridMaterial.Object);
	GridSelection ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	HoloMesh->SetMaterial(0, GridDisplayMaterial.Object);
	HoloMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<ATilePlayerController>(GWorld -> GetFirstPlayerController()))
	{
		PlayerController ->GameManager = this;
	}
	HoloMesh->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
	
}

// Called every frame
void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	if (TileTypes.Num() == 0 )return;

	FVector GridLoc =GridOffset;
	GridLoc.X +=FMath :: GridSnap(Location.X,GridSize);
	GridLoc.Y +=FMath :: GridSnap(Location.Y,GridSize);
	GridLoc.Z +=Location.Z;

	UPlayerInput* Input = GWorld ->GetFirstPlayerController() ->PlayerInput;
	
	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX =GridLoc.X /GridSize+ MapExtendsIntGrids;
		int GridY =GridLoc.Y /GridSize+ MapExtendsIntGrids;

		if (GridX < 0 || GridY < 0 || GridX >= MapExtendsIntGrids*2 || GridY >=MapExtendsIntGrids*2 )
		{
			return;
		}

		if (Map[GridX][GridY] != nullptr)
		{
			return;
		}

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase*SelectedTile=TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;
			
			FTransform TileTransform(GridLoc+GridOffset);
			TileTransform = FTransform(FRotator(0, RotationIndex * 90, 0), TileTransform.GetLocation(), TileTransform.GetScale3D());
			SelectedTile->InstancedMesh->AddInstance(SelectedTile->InstancedMesh->GetRelativeTransform()*TileTransform,true);
		}

		UE_LOG(LogTemp , Warning, TEXT("Hit: %s - %f,%f,%f"), HitActor ? *HitActor ->GetActorLabel():TEXT("None"),Location.X, Location.Y, Location.Z);
		
	}
	
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		CurrentTileIndex=(CurrentTileIndex - 1) % TileTypes.Num();
		if (CurrentTileIndex<0)
		{
			CurrentTileIndex = TileTypes.Num() - 1;
		}
		
		HoloMesh->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
		if (CurrentTileIndex == TileTypes.Num())
		{
			CurrentTileIndex = 0;
		}
		HoloMesh->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
	}
	else
	{
		GridSelection ->SetWorldLocation(GridLoc +GridOffset);
		HoloMesh->SetWorldLocation(GridLoc + GridOffset);
	}


	
	if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		AddActorLocalRotation(FRotator(0,90,0), false, 0, ETeleportType::None);
		RotationIndex++;
		if (RotationIndex == 4) RotationIndex = 0;
	}
	
}