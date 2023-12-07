// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NetAvatar.generated.h"


UCLASS()

class ANetAvatar : public ANetBaseCharacter
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float RunSpeed;

	UPROPERTY(EditAnywhere)
	bool bRunKeyPressed;

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void RunPressed();
	void RunRelased();
	void UpdateMovementParams();
};
