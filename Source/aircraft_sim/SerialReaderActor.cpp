#include "SerialReaderActor.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include <Kismet/GameplayStatics.h>

ASerialReaderActor::ASerialReaderActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASerialReaderActor::BeginPlay()
{
    Super::BeginPlay();

    FString FullPortPath = TEXT("\\\\.\\") + PortName;
    SerialHandle = CreateFileW(*FullPortPath,
        GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, 0, NULL);

    if (SerialHandle == INVALID_HANDLE_VALUE)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to open %s"), *PortName);
        return;
    }

    // Set communication parameters
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(SerialHandle, &dcbSerialParams))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to get serial parameters"));
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(SerialHandle, &dcbSerialParams))
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Failed to set serial parameters"));
        return;
    }

    // Set non-blocking read
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 1;          // 1 ms between bytes
    timeouts.ReadTotalTimeoutConstant = 1;     // constant timeout
    timeouts.ReadTotalTimeoutMultiplier = 0;

    SetCommTimeouts(SerialHandle, &timeouts);

    UE_LOG(LogTemp, Warning, TEXT("✅ Connected to %s"), *PortName);
}

void ASerialReaderActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ReadSerialData();
}

void ASerialReaderActor::ReadSerialData()
{
    if (SerialHandle == INVALID_HANDLE_VALUE) return;

    char buffer[128] = { 0 };
    DWORD bytesRead = 0;

    if (ReadFile(SerialHandle, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        FString Chunk = ANSI_TO_TCHAR(buffer);
        SerialLineBuffer += Chunk;

        // Process all complete lines in buffer
        FString Line;
        while (SerialLineBuffer.Split(TEXT("\n"), &Line, &SerialLineBuffer, ESearchCase::CaseSensitive, ESearchDir::FromStart))
        {
            // Remove \r if present
            Line = Line.Replace(TEXT("\r"), TEXT(""));

            // Now parse as before
            TArray<FString> Values;
            Line.ParseIntoArray(Values, TEXT(","), true);
            if (Values.Num() >= 5)
            {
                int32 X = FCString::Atoi(*Values[0]);
                int32 Y = FCString::Atoi(*Values[1]);
                int32 PotValue = FCString::Atoi(*Values[2]);

                // Ignore nonsense readings
                if (X < 0 || X > 4096 || Y < 0 || Y > 4096)
                    return;

                RawPotValue = PotValue;
                deltaX = X;
                deltaY = Y;
                
                UE_LOG(LogTemp, Warning, TEXT("Yaw: %f, Pitch: %f, Pot: %d, Raw: %s"),
                    X, Y, PotValue, *Line);

                
            }
        }
    }
}

void ASerialReaderActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Safely close the serial handle if open
    if (SerialHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(SerialHandle);
        SerialHandle = INVALID_HANDLE_VALUE;
        UE_LOG(LogTemp, Warning, TEXT("🔌 Serial port closed (%s)"), *PortName);
    }

    Super::EndPlay(EndPlayReason);
}