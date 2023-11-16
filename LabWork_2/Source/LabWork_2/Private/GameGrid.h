// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"
#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"

#include "GameGrid.generated.h"



UCLASS()
class AGameGrid : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	AGameGrid();

	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameSlot>GridClass;

	UPROPERTY(EditAnywhere)
	int NumCols;

	UPROPERTY(EditAnywhere)
	int NumRows;

	UPROPERTY(VisibleAnywhere)
	TArray<UChildActorComponent*> GridActors;

	AGameSlot * GetSlot(FSGridPosition& Position);
	static AGameSlot * FindSlot(FSGridPosition Position);


private:
	static AGameGrid* GameGrid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
