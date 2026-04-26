# SDL_net 3.0

This includes a list of functions that are useful for TCP and UDP. More information 
at [https://wiki.libsdl.org/SDL3_net/FrontPage](https://wiki.libsdl.org/SDL3_net/FrontPage).



# Common

## Data Types

- NET_Address

## Functions

- NET_Init
- NET_Quit
- NET_ResolveHostname
- NET_WaitUntilResolved
- NET_UnrefAddress
- NET_RefAddress
- NET_WaitUntilInputAvailable
- NET_GetAddressString
- NET_GetLocalAddresses
- NET_FreeLocalAddresses
- NET_CompareAddresses
- NET_GetAddressStatus
- NET_SimulateAddressResolutionLoss

# TCP

## Data Types 

- NET_StreamSocket

## Functions

- NET_CreateServer
- NET_DestroyServer
- NET_AcceptClient
- NET_CreateClient
- NET_ReadFromStreamSocket
- NET_WriteToStreamSocket
- NET_DestroyStreamSocket
- NET_GetConnectionStatus
- NET_SimulateStreamPacketLoss


# UDP

## Data Types 

- NET_DatagramSocket
- NET_Datagram

## Functions

- NET_CreateDatagramSocket
- NET_SendDatagram
- NET_ReceiveDatagram
- NET_DestroyDatagram
- NET_DestroyDatagramSocket
- NET_SimulateDatagramPacketLoss
