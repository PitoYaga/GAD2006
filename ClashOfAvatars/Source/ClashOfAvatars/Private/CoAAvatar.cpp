// Fill out your copyright notice in the Description page of Project Settings.


#include "CoAAvatar.h"



ACoAAvatar::ACoAAvatar():

RunSpeed(500.0f),
Stamina(0.0f),
MaxStamina(100.0f),
StaminaDrainRate(40.0f),
StaminaGainRate(20.0f)

{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 300.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mSpringArm->bUsePawnControlRotation = true;
	mCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;
}


void ACoAAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void ACoAAvatar::OnConstruction(const FTransform& Transform)
{
	Stamina = MaxStamina;
	bRunKeyPressed = false;
}

void ACoAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);

/*
	
	if (bRunKeyPressed)
	{
		if (Stamina <= 0)
		{
			bStaminaDrained = true;
			RunRelased();
		}
		else
		{
			Stamina -= StaminaDrainRate * DeltaTime;
		}
	}
	else if(bStaminaDrained && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
	{
		Stamina += StaminaGainRate * DeltaTime;
	}


	if (Stamina >= MaxStamina)
	{
		bStaminaDrained = false;
		Stamina = MaxStamina;
	}
	else if(!bRunKeyPressed && Stamina <= MaxStamina)
	{
		Stamina += StaminaGainRate * DeltaTime;
	}

	*/






	if(GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		if (!bStaminaDrained && bRunKeyPressed)
		{
			if (!GetCharacterMovement()->Velocity.IsNearlyZero(0.01f) )
			{
				Stamina = FMath::Max(0 , Stamina - StaminaDrainRate * DeltaTime);
				if (Stamina <= 0)
				{
					bStaminaDrained = true;
					UpdateMovementParams();
				}
			}
		}
		else
		{
			Stamina = FMath::Min(MaxStamina , Stamina + StaminaGainRate * DeltaTime);
			if (Stamina >= MaxStamina)
			{
				bStaminaDrained = false;
				UpdateMovementParams();
			}
		}
	}
	

	GEngine -> AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, FString::Printf(TEXT("%f"), Stamina));
}




void ACoAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACoAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACoAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACoAAvatar::RunRelased);
}

void ACoAAvatar::MoveForward(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(FRotator(0, ControlRotation.Yaw, 0)).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, Amount);
}

void ACoAAvatar::MoveRight(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector RightdDirection = FRotationMatrix(FRotator(0, ControlRotation.Yaw, 0)).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightdDirection, Amount);
}




void ACoAAvatar::RunPressed()
{
	bRunKeyPressed = true;
	UpdateMovementParams();
}

void ACoAAvatar::RunRelased()
{
	bRunKeyPressed = false;
	UpdateMovementParams();
}

void ACoAAvatar::UpdateMovementParams()
{
	GetCharacterMovement()->MaxWalkSpeed =
		bRunKeyPressed && !bStaminaDrained ? RunSpeed : WalkSpeed;
}




void ACoAAvatar::SetStamina(float NewStamina)
{
	Stamina = NewStamina;
}

float ACoAAvatar::GetStamina()
{
	return Stamina;
}



