#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AircraftPawn.generated.h"

UCLASS()
class AIRCRAFT_SIM_API AAircraftPawn : public APawn
{
    GENERATED_BODY()

public:
    AAircraftPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere)
    class USkeletalMeshComponent* AircraftMesh;

    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

    float ThrustAmount;
    float ThrustStep;
    float MaxThrust;
    float PitchSpeed;
    float RollSpeed;
    float YawSpeed;

    void AdjustThrust(float Value);
    void AdjustPitch(float Value);
    void Roll(float Value);
    void Yaw(float Value);
};
