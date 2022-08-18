// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootXCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AShootXCharacter::AShootXCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


}

void AShootXCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShootXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShootXCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootXCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AShootXCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShootXCharacter::LookUp);
}

void AShootXCharacter::MoveForward(float Value) 
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AShootXCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AShootXCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AShootXCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AShootXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



