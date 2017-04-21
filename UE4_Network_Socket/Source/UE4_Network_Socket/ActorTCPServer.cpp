// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_Network_Socket.h"
#include "ActorTCPServer.h"


// Sets default values
AActorTCPServer::AActorTCPServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorTCPServer::BeginPlay()
{
	Super::BeginPlay();
	
	Launch();
}

// Called every frame
void AActorTCPServer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AActorTCPServer::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("Actor Begin Destroy "));

	if (ConnectionSocket)
	{
		ConnectionSocket->Close();
	}

	if (ListenerSocket)
	{
		ListenerSocket->Close();
	}
}


void AActorTCPServer::Launch()
{
	//IP = 127.0.0.1, Port = 8890 for my Python test case
	if (!StartTCPReceiver(TEXT("RamaSocketListener"), TEXT("127.0.0.1"), 8891))
	{
		UE_LOG(LogTemp, Log, TEXT("TCP Socket Listener Failed.	"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Start TCP Receiver Created!"));
}

/************************************************************************/
// Start TCP Receiver
/************************************************************************/
bool AActorTCPServer::StartTCPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		UE_LOG(LogTemp, Warning, TEXT("StartTCPReceiver >> Listen socket could not be created!~> %s %d"), *TheIP, ThePort);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Come here 1 "));
	//Start the Listener! //thread this eventually
	GetWorldTimerManager().SetTimer(MyTimerHandler2, this, &AActorTCPServer::TCPConnectionListener, 0.01, true);

	UE_LOG(LogTemp, Log, TEXT("Come here 2 "));


	return true;
}

//Rama's Create TCP Connection Listener
FSocket* AActorTCPServer::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	FIPv4Address ip;
	if (FIPv4Address::Parse(TheIP, ip))
	{
		//Create Socket
		FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
			.AsReusable()
			.BoundToAddress(ip)
			.BoundToPort(ThePort)
			.Listening(8);

		//Set Buffer Size
		int32 NewSize = 0;
		ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

		UE_LOG(LogTemp, Log, TEXT("Done!!!!"));
		//Done!
		return ListenSocket;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid IP! Expecting 4 parts separated by ."));
	}

	return false;



}

//Rama's TCP Connection Listener
void AActorTCPServer::TCPConnectionListener()
{
	if (!ListenerSocket) return;


	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	// handle incoming connections
	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		UE_LOG(LogTemp, Log, TEXT("Anybody try to connect ?! "));

		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}



		UE_LOG(LogTemp, Log, TEXT("Start to Accept:.... "));


		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		if (ConnectionSocket != NULL)
		{

			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			UE_LOG(LogTemp, Log, TEXT("Remote connect! "));


			//UE_LOG "Accepted Connection! WOOOHOOOO!!!";

			//can thread this too
			GetWorldTimerManager().SetTimer(MyTimerHandler, this, &AActorTCPServer::TCPSocketListener, 0.01, true);
		}
	}
}

//Rama's String From Binary Array
FString AActorTCPServer::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	BinaryArray.Add(0); // Add 0 termination. Even if the string is already 0-terminated, it doesn't change the results.
						// Create a string from a byte array. The string is expected to be 0 terminated (i.e. a byte set to 0).
						// Use UTF8_TO_TCHAR if needed.
						// If you happen to know the data is UTF-16 (USC2) formatted, you do not need any conversion to begin with.
						// Otherwise you might have to write your own conversion algorithm to convert between multilingual UTF-16 planes.
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

//Rama's TCP Socket Listener
void AActorTCPServer::TCPSocketListener()
{
	//UE_LOG(LogTemp, Log, TEXT("Call the TCP Socket Listener "));


	if (!ConnectionSocket) return;


	//Binary Array!
	TArray<uint8> ReceivedData;

	uint8 default_data = 0;
	uint32 Size;

	//UE_LOG(LogTemp, Log, TEXT("Hanldint the message....."));

	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(default_data, FMath::Min(Size, 65507u));

		int32 Read = 0;

		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));

		UE_LOG(LogTemp, Log, TEXT("Data Read! %d"), ReceivedData.Num());

	}

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));


	//	Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));

	UE_LOG(LogTemp, Log, TEXT("I got your message: %s"), *ReceivedUE4String);

	// Time to send msg back!
	int32 Send = 0;
	if (ConnectionSocket->Send(ReceivedData.GetData(), ReceivedData.Num(), Send))
	{
		UE_LOG(LogTemp, Log, TEXT("I Send back successfully: %s"));
	}
}
