#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SerialReaderActor.generated.h"

UCLASS()
class FIGHTJET_API ASerialReaderActor : public AActor
{
    GENERATED_BODY()

public:
    ASerialReaderActor();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    void ReadSerialData();

    void* SerialHandle;
    FString PortName = TEXT("COM3"); // Change this to your Arduino port (e.g., COM4)
};
