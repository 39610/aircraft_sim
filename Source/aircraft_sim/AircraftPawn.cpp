#include "AircraftPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AAircraftPawn::AAircraftPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    AircraftMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AircraftMesh"));
    RootComponent = AircraftMesh;

    PitchSpeed = 45.f; // Derece/saniye
    RollSpeed = 50.f; // Derece/saniye
    YawSpeed = 50.f; // derece/saniye

    ThrustAmount = 0.f;
    ThrustStep = 500.f;     // W/S ile saniyede değişim hızı
    MaxThrust = 3000.f;

    // Spring Arm (kamera kolu)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f;  // Kamera ile uçak arasındaki mesafe
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 5.f;
    SpringArm->bUsePawnControlRotation = false;

    // Kamera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

}

void AAircraftPawn::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay çalıştı"));
}

void AAircraftPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ThrustAmount > 0.f)
    {
        FVector Movement = GetActorForwardVector() * ThrustAmount * DeltaTime;
        AddActorLocalOffset(Movement, true);
    }

    UE_LOG(LogTemp, Warning, TEXT("Thrust = %f"), ThrustAmount);
}

void AAircraftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("YawInput", this, &AAircraftPawn::Yaw);
    PlayerInputComponent->BindAxis("Roll", this, &AAircraftPawn::Roll);
    PlayerInputComponent->BindAxis("PitchInput", this, &AAircraftPawn::AdjustPitch);
    PlayerInputComponent->BindAxis("Thrust", this, &AAircraftPawn::AdjustThrust);
}

void AAircraftPawn::AdjustThrust(float Value)
{
    ThrustAmount = FMath::Clamp(
        ThrustAmount + Value * ThrustStep * GetWorld()->GetDeltaSeconds(),
        0.f,
        MaxThrust
    );

    UE_LOG(LogTemp, Warning, TEXT("AdjustThrust çağrıldı: %f"), ThrustAmount);
}

void AAircraftPawn::AdjustPitch(float Value)
{
    if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        FRotator CurrentRotation = GetActorRotation();
        CurrentRotation.Pitch += Value * PitchSpeed * GetWorld()->GetDeltaSeconds();
        SetActorRotation(CurrentRotation);
    }
}

void AAircraftPawn::Roll(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    FRotator DeltaRotation = FRotator(0.f, 0.f, Value * RollSpeed * GetWorld()->GetDeltaSeconds());
    AddActorLocalRotation(DeltaRotation);
}

void AAircraftPawn::Yaw(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    FRotator DeltaRotation = FRotator(0.f, Value * YawSpeed * GetWorld()->GetDeltaSeconds(), 0.f);
    AddActorLocalRotation(DeltaRotation);
}

