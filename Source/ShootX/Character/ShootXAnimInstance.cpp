// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootXAnimInstance.h"
#include "ShootXCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShootX/Weapon/Weapon.h"

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
	bWeaponEquipped = ShootXCharacter->IsWeaponEquipped();
	EquippedWeapon = ShootXCharacter->GetEquippedWeapon();
	bIsCrouched = ShootXCharacter->bIsCrouched;
	bAiming = ShootXCharacter->IsAiming();
	TurningInPlace = ShootXCharacter->GetTurningInPlace();

	// Offset Yaw for Strafing
	FRotator AimRotation = ShootXCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShootXCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;
	
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ShootXCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = ShootXCharacter->GetAO_Yaw();
	AO_Pitch = ShootXCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ShootXCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		ShootXCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}
