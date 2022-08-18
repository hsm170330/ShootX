// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootXAnimInstance.h"
#include "ShootXCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShootXAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShootXCharacter = Cast<AShootXCharacter>(TryGetPawnOwner());
}

void UShootXAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ShootXCharacter == nullptr)
	{
		ShootXCharacter = Cast<AShootXCharacter>(TryGetPawnOwner());
	}

	if (ShootXCharacter == nullptr) return;

	FVector Velocity = ShootXCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ShootXCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ShootXCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;


}
