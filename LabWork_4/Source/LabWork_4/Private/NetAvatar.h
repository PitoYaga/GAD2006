// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetAvatar.generated.h"


UCLASS()

class ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()

public:
	ANetAvatar();
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	float UpdateSpeed;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerSpeedChanged)
	float Speed;

	UFUNCTION()
	void OnRep_PlayerSpeedChanged();

	UFUNCTION(Server, Reliable)
	void SetSpeedInServer(float speedFloat);

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void RunPressed();
	void RunRelased();
};
