// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ANetAvatar::ANetAvatar():
UpdateSpeed(5.0f),
Speed(150.0f),
MovementScale(1.0f)


{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ANetAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ANetAvatar::RunRelased);
}

void ANetAvatar::OnRep_PlayerSpeedChanged()
{
}

void ANetAvatar::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetAvatar, Speed);
}

void ANetAvatar::SetSpeedInServer_Implementation(float speedFloat)
{
	Speed = speedFloat;
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}


void ANetAvatar::MoveForward(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementScale *Scale);
}

void ANetAvatar::MoveRight(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, MovementScale *Scale);
}

void ANetAvatar::RunPressed()
{
	Speed = Speed * UpdateSpeed;
	SetSpeedInServer(Speed);
}

void ANetAvatar::RunRelased()
{
	Speed = Speed / UpdateSpeed;
	SetSpeedInServer(Speed);
}

