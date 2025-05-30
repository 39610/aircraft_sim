#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SerialReaderActor.generated.h"

UCLASS(Blueprintable)
class AIRCRAFT_SIM_API ASerialReaderActor : public AActor
{
    GENERATED_BODY()

public:
    ASerialReaderActor();
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
    virtual void BeginPlay() override;

private:
    void ReadSerialData();

    void* SerialHandle;

public:
    
    FString SerialLineBuffer;
    FString PortName = TEXT("COM7");

    UPROPERTY(BlueprintReadOnly, Category = "Serial")
    int32 RawPotValue = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Serial")
    float deltaX ;
    UPROPERTY(BlueprintReadOnly, Category = "Serial")
    float deltaY ;
};
