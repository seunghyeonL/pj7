// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
// ī�޶�, ������ �� ���� ������ �ʿ��� ���� include
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASpartaCharacter::ASpartaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// (1) ������ �� ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// ������ ���� ��Ʈ ������Ʈ (CapsuleComponent)�� ����
	SpringArmComp->SetupAttachment(RootComponent);
	// ĳ���Ϳ� ī�޶� ������ �Ÿ� �⺻�� 300���� ����
	SpringArmComp->TargetArmLength = 300.0f;
	// ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
	SpringArmComp->bUsePawnControlRotation = true;

	// (2) ī�޶� ������Ʈ ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// ������ ���� ���� ��ġ�� ī�޶� ����
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// ī�޶�� ������ ���� ȸ���� �����Ƿ� PawnControlRotation�� ����
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// Called when the game starts or when spawned
void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent�� ĳ����
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{ 
			if (PlayerController->MoveAction)
			{
				// IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				// IA_Jump �׼� Ű�� "Ű�� ������ �ִ� ����" StartJump() ȣ��
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump
				);

				// IA_Jump �׼� Ű���� "���� �� ����" StopJump() ȣ��
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				// IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				// IA_Sprint �׼� Ű�� "������ �ִ� ����" StartSprint() ȣ��
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint
				);
				// IA_Sprint �׼� Ű���� "���� �� ����" StopSprint() ȣ��
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
				);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	// ��Ʈ�ѷ��� �־�� ���� ����� ����
	if (!Controller) return;

	// Value�� Axis2D�� ������ IA_Move�� �Է°� (WASD)�� ��� ����
// ��) (X=1, Y=0) �� ���� / (X=-1, Y=0) �� ���� / (X=0, Y=1) �� ������ / (X=0, Y=-1) �� ����
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// ĳ���Ͱ� �ٶ󺸴� ����(����)���� X�� �̵�
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// ĳ������ ������ �������� Y�� �̵�
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	// Jump �Լ��� Character�� �⺻ ����
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	// StopJumping �Լ��� Character�� �⺻ ����
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	// ���콺�� X, Y �������� 2D ������ ������
	FVector2D LookInput = value.Get<FVector2D>();

	// X�� �¿� ȸ�� (Yaw), Y�� ���� ȸ�� (Pitch)
	// �¿� ȸ��
	AddControllerYawInput(LookInput.X);
	// ���� ȸ��
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	// Shift Ű�� ���� ���� �� �Լ��� ȣ��ȴٰ� ����
	// ������Ʈ �ӵ��� ����
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	// Shift Ű�� �� ���� �� �Լ��� ȣ��
	// ���� �ӵ��� ����
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}