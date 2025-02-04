// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

// Sets default values
AMyPawn::AMyPawn() :
	NormalSpeed{ 600.0f },
	SprintSpeedMultiplier{ 1.5f },
	CurrentMoveInput{ 0.0f, 0.0f, 0.0f }/*,
	CurrentLookInput{ 0.0f, 0.0f }*/
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
}

void AMyPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	FVector2D CurrentMoveInput2D = value.Get<FVector2D>().GetSafeNormal();
	CurrentMoveInput.X = CurrentMoveInput2D.X;
	CurrentMoveInput.Y = CurrentMoveInput2D.Y;
}

void AMyPawn::StartJump(const FInputActionValue& value)
{
}

void AMyPawn::StopJump(const FInputActionValue& value)
{
}

void AMyPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;
	FVector2D CurrentLookInput = value.Get<FVector2D>();
	AddControllerYawInput(CurrentLookInput.X);
	AddControllerPitchInput(CurrentLookInput.Y);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = Controller->GetControlRotation().Yaw;
	SetActorRotation(NewRotation);
}

void AMyPawn::StartSprint(const FInputActionValue& value)
{
}

void AMyPawn::StopSprint(const FInputActionValue& value)
{
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentMoveInput.IsNearlyZero()) 
	{
		FVector MoveVector = CurrentMoveInput * NormalSpeed * DeltaTime;
		AddActorLocalOffset(MoveVector);
		CurrentMoveInput.Set(0.0f, 0.0f, 0.0f);
	}

	//if (!CurrentLookInput.IsNearlyZero()) 
	//{
	//	FVector2D LookVector2D = CurrentLookInput * NormalSpeed * DeltaTime;
	//	AddActorLocalRotation({ LookVector2D.X, LookVector2D.Y, 0.0f });
	//	CurrentLookInput.Set(0.0f, 0.0f);
	//}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}

