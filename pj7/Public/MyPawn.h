// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;

struct FInputActionValue;

UCLASS()
class PJ7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Copmponents")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Copmponents")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Copmponents")
	UCameraComponent* CameraComp;

	
	// �̵� �ӵ� ���� ������Ƽ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPawn|Constants")
	float NormalSpeed; // �⺻ �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyPawn|Constants")
	float SprintSpeedMultiplier;  // "�⺻ �ӵ�" ��� �� ��� ������ �޸��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Constants")
	float SprintSpeed; 	// ���� ������Ʈ �ӵ�

	// �Է� ������Ƽ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Constants")
	FVector CurrentMoveInput;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyPawn|Constants")
	//FVector2D CurrentLookInput;

	// IA_Move�� IA_Jump ���� ó���� �Լ� ����
	// Enhanced Input���� �׼� ���� FInputActionValue�� ���޵˴ϴ�.
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
