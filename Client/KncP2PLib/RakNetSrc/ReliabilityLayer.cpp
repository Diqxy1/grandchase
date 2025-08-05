#include "stdafx.h"
/* -*- mode: c++; c-file-style: raknet; tab-always-indent: nil; -*- */
/**
 * @file 
 * @brief Reliability Layer Implementation 
 * 
 * This file is part of RakNet Copyright 2003, 2004 Rakkarsoft LLC and
 * Kevin Jenkins.
 *
 * Usage of Raknet is subject to the appropriate licence agreement.
 * "Shareware" Licensees with Rakkarsoft LLC are subject to the
 * shareware license found at
 * http://www.rakkarsoft.com/shareWareLicense.html which you agreed to
 * upon purchase of a "Shareware license" "Commercial" Licensees with
 * Rakkarsoft LLC are subject to the commercial license found at
 * http://www.rakkarsoft.com/sourceCodeLicense.html which you agreed
 * to upon purchase of a "Commercial license" All other users are
 * subject to the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * Refer  to  the  appropriate license  agreement  for  distribution,
 * modification, and warranty rights.
 */
#include "ReliabilityLayer.h"
//#include <assert.h>
#include "GetTime.h"
#include "SocketLayer.h"
#include "dbg/dbg.hpp"
// alloca
#ifdef _WIN32
#include <malloc.h>
#else
#include <stdlib.h>
#endif
//#include <algorithm>
//#include "MemoryManager.h"

// Defined in rand.cpp
extern void seedMT( unsigned long seed );
extern inline unsigned long randomMT( void );
extern inline float frandomMT( void );

static const int ACK_BIT_LENGTH = sizeof( PacketNumberType ) *8 + 1;
static const int MAXIMUM_WINDOW_SIZE = ( DEFAULT_MTU_SIZE - UDP_HEADER_SIZE ) *8 / ACK_BIT_LENGTH; // Sanity check - the most ack packets that could ever (usually) fit into a frame.
static const int MINIMUM_WINDOW_SIZE = 5; // how many packets can be sent unacknowledged before waiting for an ack

#ifdef _INTERNET_SIMULATOR 
// Lag

struct DataAndTime
{
	char data[ 2000 ];
	int length;
	unsigned long sendTime;
};

static BasicDataStructures::List<DataAndTime*> delayList;
#endif

DWORD ReliabilityLayer::ms_dwUID = 0;
PlayerID ReliabilityLayer::ms_kRelayServerID;

#define MAX_RESEND_COUNT   10

//-------------------------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------------------------
ReliabilityLayer::ReliabilityLayer() : updateBitStream( MAXIMUM_MTU_SIZE )   // preallocate the update bitstream so we can avoid a lot of reallocs at runtime
{
	InitializeVariables();
#ifdef __USE_IO_COMPLETION_PORTS
	
	readWriteSocket = INVALID_SOCKET;
#endif
	
	freeThreadedMemoryOnNextUpdate = false;
    m_vecUID.clear();
    m_bRelayMain = false;
    m_vecLastSendedUID.clear();

    m_iCntFailRead = 0;
    m_iCntFailReadCompressed = 0;
    m_iCntFailReadAlignedBytes = 0;
}

//-------------------------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------------------------
ReliabilityLayer::~ReliabilityLayer()
{
	FreeMemory( true ); // Free all memory immediately
    m_vecUID.clear();
    m_bRelayMain = false;
    m_vecLastSendedUID.clear();

#ifdef __USE_IO_COMPLETION_PORTS
	
	if ( readWriteSocket != INVALID_SOCKET )
		closesocket( readWriteSocket );
		
#endif
}

//-------------------------------------------------------------------------------------------------------
// Resets the layer for reuse
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::Reset( void )
{
	FreeMemory( false ); // False because Reset can be called by any thread
	InitializeVariables();
}

//-------------------------------------------------------------------------------------------------------
// Sets up encryption
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SetEncryptionKey( const unsigned char* key )
{
	if ( key )
		encryptor.SetKey( key );
	else
		encryptor.UnsetKey();
}

//-------------------------------------------------------------------------------------------------------
// Assign a socket for the reliability layer to use for writing
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SetSocket( SOCKET s )
{
#ifdef __USE_IO_COMPLETION_PORTS
	// If this hits I am probably using sequential ports while doing IO completion ports
	assert( s != INVALID_SOCKET );
	readWriteSocket = s;
#endif
}

//-------------------------------------------------------------------------------------------------------
// Get the socket held by the reliability layer
//-------------------------------------------------------------------------------------------------------
SOCKET ReliabilityLayer::GetSocket( void )
{
#ifdef __USE_IO_COMPLETION_PORTS
	return readWriteSocket;
#else
	
	return INVALID_SOCKET;
#endif
}

//-------------------------------------------------------------------------------------------------------
// Initialize the variables
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::InitializeVariables( void )
{
	memset( waitingForOrderedPacketReadIndex, 0, NUMBER_OF_ORDERED_STREAMS );
	memset( waitingForSequencedPacketReadIndex, 0, NUMBER_OF_ORDERED_STREAMS );
	memset( waitingForOrderedPacketWriteIndex, 0, NUMBER_OF_ORDERED_STREAMS );
	memset( waitingForSequencedPacketWriteIndex, 0, NUMBER_OF_ORDERED_STREAMS );
	memset( receivedPackets, 0, RECEIVED_PACKET_LOG_LENGTH * sizeof( unsigned long ) );
	memset( &statistics, 0, sizeof( statistics ) );
	statistics.connectionStartTime = RakNet::GetTime();
	splitPacketId = 0L;
	packetNumber = 0;
	// lastPacketSendTime=retransmittedFrames=sentPackets=sentFrames=receivedPacketsCount=bytesSent=bytesReceived=0;
	SetLostPacketResendDelay( 1000L );
	deadConnection = cheater = false;
	lastAckTime = 0;
	blockWindowIncreaseUntilTime = 0;
	// Windowing
	windowSize = MINIMUM_WINDOW_SIZE;
	lossyWindowSize = MAXIMUM_WINDOW_SIZE + 1; // Infinite
	lastWindowIncreaseSizeTime = 0;
	// lastPacketReceivedTime=0;
    m_vecUID.clear();
    m_bRelayMain = false;
    m_vecLastSendedUID.clear();
}

//-------------------------------------------------------------------------------------------------------
// Frees all allocated memory
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::FreeMemory( bool freeAllImmediately )
{
	if ( freeAllImmediately )
	{
		FreeThreadedMemory();
		FreeThreadSafeMemory();
	}
	
	else
	{
		FreeThreadSafeMemory();
		freeThreadedMemoryOnNextUpdate = true;
	}
	
}

void ReliabilityLayer::FreeThreadedMemory( void )
{
	unsigned i;
	InternalPacket *internalPacket;
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
	
	for ( i = 0; i < splitPacketList.size(); i++ )
	{
		delete [] splitPacketList[ i ] ->data;
		InternalPacketPool::Instance() ->ReleasePointer( splitPacketList[ i ] );
	}
	
	splitPacketList.clear();
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
	// }
	
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	//reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
	
	while ( outputQueue.size() > 0 )
	{
		internalPacket = outputQueue.pop();
		delete [] internalPacket->data;
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
	}
	
	outputQueue.clearAndForceAllocation( 512 );
	//reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
	// }
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	//reliabilityLayerMutexes[orderingList_MUTEX].Lock();
	
	for ( i = 0; i < orderingList.size(); i++ )
	{
		if ( orderingList[ i ] )
		{
			BasicDataStructures::LinkedList<InternalPacket*>* theList = orderingList[ i ];
			
			if ( theList )
			{
				while ( theList->size() )
				{
					internalPacket = orderingList[ i ] ->pop();
					delete [] internalPacket->data;
					InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
				}
				
				delete theList;
			}
		}
	}
	
	orderingList.clear();
	//reliabilityLayerMutexes[orderingList_MUTEX].Unlock();
	// }
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Lock();
	
	while ( acknowledgementQueue.size() > 0 )
		InternalPacketPool::Instance() ->ReleasePointer( acknowledgementQueue.pop() );
		
	acknowledgementQueue.clearAndForceAllocation( 64 );
	
	//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
	// }
	
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	//reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
	while ( resendQueue.size() )
	{
		// The resend Queue can have NULL pointer holes.  This is so we can deallocate blocks without having to compress the array
		internalPacket = resendQueue.pop();
		
		if ( internalPacket )
		{
			delete [] internalPacket->data;
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		}
	}
	
	resendQueue.clearAndForceAllocation( 512 );
	//reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
	// }
	
	
	
}

void ReliabilityLayer::FreeThreadSafeMemory( void )
{
	unsigned i, j;
	// InternalPacket *internalPacket;
	
	// if (bytesSent > 0 || bytesReceived > 0)
	// {
	
	for ( i = 0; i < NUMBER_OF_PRIORITIES; i++ )
	{
		j = 0;
		reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Lock();
		
		for ( ; j < sendQueue[ i ].size(); j++ )
		{
			delete [] ( sendQueue[ i ] ) [ j ] ->data;
			InternalPacketPool::Instance() ->ReleasePointer( ( sendQueue[ i ] ) [ j ] );
		}
		
		sendQueue[ i ].clearAndForceAllocation( 512 ); // Preallocate the send lists so we don't do a bunch of reallocations unnecessarily
		reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Unlock();
	}
	
	// }
	
	
#ifdef _INTERNET_SIMULATOR
	for ( i = 0; i < delayList.size(); i++ )
		delete delayList[ i ];
		
	delayList.clear();
	
#endif
	
}

//-------------------------------------------------------------------------------------------------------
// Packets are read directly from the socket layer and skip the reliability
//layer  because unconnected players do not use the reliability layer
// This function takes packet data after a player has been confirmed as
//connected.  The game should not use that data directly
// because some data is used internally, such as packet acknowledgement and
//split packets
//====================================================================================
// 2006.07.06 : Asirion
// dwUID = 보낸 사람의 UID
bool ReliabilityLayer::HandleSocketReceiveFromConnectedPlayer( char *buffer, int length, DWORD dwUID_ )
{
#ifdef _DEBUG
	assert( !( length <= 0 || buffer == 0 ) );
#endif
	
	if ( length <= 1 || buffer == 0 )   // Length of 1 is a connection request resend that we just ignore
		return true;
		
	int numberOfAcksInFrame = 0;
	
	unsigned long time;
	
	bool pushedPacket;
	
	int count;
	
	// bytesReceived+=length + UDP_HEADER_SIZE;
	
	// decode this whole chunk if the decoder is defined.
	if ( encryptor.IsKeySet() )
	{
		if ( encryptor.Decrypt( ( unsigned char* )( buffer ), length , ( unsigned char* ) buffer, &length ) == false )
		{
			statistics.bitsWithBadCRCReceived += ( length ) * 8;
			statistics.packetsWithBadCRCRecieved++;
			return false;
		}
	}
	
	statistics.bitsReceived += ( length ) * 8;
	statistics.packetsReceived++;
	
	RakNet::BitStream socketData( buffer , length , false ); // Convert the incoming data to a bitstream for easy parsing
	
	// time = lastPacketReceivedTime = RakNet::GetTime();
	time = RakNet::GetTime();
	
	//printf("In HandleSocketReceiveFromConnectedPlayer %i bytes\n",length);
	//for (int ass=0; ass < length && ass < 10; ass++)
	// printf("%i ", ((char*)(socketData.GetData()))[ass]);
	//printf("\n\n");
	
	// Parse the bitstream to create an internal packet
	InternalPacket* internalPacket = CreateInternalPacketFromBitStream( &socketData, time );
	
	while ( internalPacket )
	{
        //================================================================================
        // Ack가 온것이다. ResendQ에서 ACK를 삭제한다.
		if ( internalPacket->isAcknowledgement )
		{
			numberOfAcksInFrame++;
			if ( resendQueue.size() == 0 )
			{
				lastAckTime = 0; // Not resending anything so clear this var so we don't drop the connection on not getting any more acks
			}
			
			else
			{
				lastAckTime = time; // Just got an ack.  Record when we got it so we know the connection is alive
			}
			if ( RemovePacketFromResendQueueAndDeleteOlderReliableSequenced( internalPacket->packetNumber, dwUID_ ) == true )
			    InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		}
        //================================================================================
		
		else
		{
			// ACK가 아닌 데이터를 RelayMain이 받는것은 오류이다.
            if ( m_bRelayMain == true )
            {
                START_LOG( cerr, "Recv Wrong Packet that is not ACK" ) << BUILD_LOG( dwUID_ );
                return false;
            }
            if ( internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_ORDERED || internalPacket->reliability == RELIABLE )
			{
				SendAcknowledgementPacket( internalPacket->packetNumber, time  );
			}
			
            //================================================================================
            // 오류처리
			// ReliabilityLayer Line 921, 2189에 보면 cycle로 사용하도록 되어 있다.
			if ( internalPacket->packetNumber >= RECEIVED_PACKET_LOG_LENGTH )
			{
				statistics.invalidMessagesReceived++;
				
				delete [] internalPacket->data;
				InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
#ifdef _DEBUG
				
				printf( "Error!! internalPacket->packetNumber >= RECEIVED_PACKET_LOG_LENGTH\n" );
#endif
				
				return true;
			}
            //================================================================================
            // 오류쳐리.
			if ( receivedPackets[ internalPacket->packetNumber ] > time - TIMEOUT_TIME )
			{
#ifdef _DEBUG
				printf( "Warning - got duplicate packet (%i).  Did RECEIVED_PACKET_LOG_LENGTH overrun?\n", internalPacket->packetNumber );
#endif
				
				statistics.duplicateMessagesReceived++;
				
				// Duplicate packet
				delete [] internalPacket->data;
				InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
				goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
			}
            //================================================================================			
			
			statistics.messagesReceived++;
            // 이 패킷이 이 시간에 왔다라고 기록.			
			receivedPackets[ internalPacket->packetNumber ] = time;
			
            // UnReliable한 조각난 패킷들중 오래된것 삭제.
			if ( internalPacket->splitPacketCount > 0 )
				DeleteOldUnreliableSplitPackets( time );
				
			if ( internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == UNRELIABLE_SEQUENCED )
			{
#ifdef _DEBUG
				assert( internalPacket->orderingChannel < NUMBER_OF_ORDERED_STREAMS );
#endif
				
				if ( internalPacket->orderingChannel >= NUMBER_OF_ORDERED_STREAMS )
				{
					// Invalid packet
#ifdef _DEBUG
					printf( "Got invalid packet\n" );
#endif
					
					delete [] internalPacket->data;
					InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
					goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
				}
				
				if ( IsOlderOrderedPacket( internalPacket->orderingIndex, waitingForSequencedPacketReadIndex[ internalPacket->orderingChannel ] ) == false )
				{
					statistics.sequencedMessagesInOrder++;
					
					// Check for older packets in the output list. Delete any found
					// UPDATE:
					// Disabled.  We don't have enough info to consistently do this.  Sometimes newer data does supercede
					// older data such as with constantly declining health, but not in all cases.
					// For example, with sequenced unreliable sound packets just because you send a newer one doesn't mean you
					// don't need the older ones because the odds are they will still arrive in order
					/*
					  reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
					  DeleteSequencedPacketsInList(internalPacket->orderingChannel, outputQueue);
					  reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
					
					  // Check for older packets in the split packet list. Delete any found
					  reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
					  DeleteSequencedPacketsInList(internalPacket->orderingChannel, splitPacketList, internalPacket->splitPacketId);
					  reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
					*/ 
					// Is this a split packet?
					
					if ( internalPacket->splitPacketCount > 0 )
					{
						// Generate the split
						// Verify some parameters to make sure we don't get junk data
#ifdef _DEBUG
						assert( internalPacket->splitPacketIndex < internalPacket->splitPacketCount );
						assert( internalPacket->dataBitLength < MAXIMUM_MTU_SIZE * 8 );
						
						//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
						// Make sure this is not a duplicate insertion.  If this assert hits then splitPacketId overflowed into existing waiting split packets (i.e. more than rangeof(splitPacketId) waiting)
						
						for ( unsigned cnt = 0; cnt < splitPacketList.size(); cnt++ )
							assert ( !( splitPacketList[ cnt ] ->splitPacketIndex == internalPacket->splitPacketIndex && splitPacketList[ cnt ] ->splitPacketId == splitPacketId ) );
							
						int splitPacketListSize = splitPacketList.size() + 1;
						
						//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
#endif
						
						// Check for a rebuilt packet
						InsertIntoSplitPacketList( internalPacket );
						
						// Sequenced
						internalPacket = BuildPacketFromSplitPacketList( internalPacket->splitPacketId, time );
						
						if ( internalPacket )
						{
#ifdef _DEBUG
							//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
							assert( splitPacketList.size() == splitPacketListSize - internalPacket->splitPacketCount );
							//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
#endif
							// Update our index to the newest packet
							waitingForSequencedPacketReadIndex[ internalPacket->orderingChannel ] = internalPacket->orderingIndex + 1;
							
							// If there is a rebuilt packet, add it to the output queue
							//       reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
							outputQueue.push( internalPacket );
							//       reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
							internalPacket = 0;
						}
						
#ifdef _DEBUG
						else
						{
							//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
							assert( splitPacketList.size() == splitPacketListSize );
							//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
						}
						
#endif
						// else don't have all the parts yet
					}
					
					else
					{
						// Update our index to the newest packet
						waitingForSequencedPacketReadIndex[ internalPacket->orderingChannel ] = internalPacket->orderingIndex + 1;
						
						// Not a split packet. Add the packet to the output queue
						//      reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
						outputQueue.push( internalPacket );
						//      reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
						internalPacket = 0;
					}
				}
				
				else
				{
					statistics.sequencedMessagesOutOfOrder++;
					
					// Older sequenced packet. Discard it
					delete [] internalPacket->data;
					InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
				}
				
				goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
			}
			
			// Is this an unsequenced split packet?
			if ( internalPacket->splitPacketCount > 0 )
			{
				// An unsequenced split packet.  May be ordered though.
				
				// Verify some parameters to make sure we don't get junk data
#ifdef _DEBUG
				assert( internalPacket->splitPacketIndex < internalPacket->splitPacketCount );
				assert( internalPacket->dataBitLength < MAXIMUM_MTU_SIZE * 8 );
#endif
				
				// Check for a rebuilt packet
				
				if ( internalPacket->reliability != RELIABLE_ORDERED )
					internalPacket->orderingChannel = 255; // Use 255 to designate not sequenced and not ordered
					
#ifdef _DEBUG
				//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
				// Make sure this is not a duplicate insertion.  If this assert hits then splitPacketId overflowed into existing waiting split packets (i.e. more than rangeof(splitPacketId) waiting)
				for ( unsigned cnt = 0; cnt < splitPacketList.size(); cnt++ )
					assert ( !( splitPacketList[ cnt ] ->splitPacketIndex == internalPacket->splitPacketIndex && splitPacketList[ cnt ] ->splitPacketId == internalPacket->splitPacketId ) );
					
				int splitPacketListSize = splitPacketList.size() + 1;
				
				//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
#endif
				
				InsertIntoSplitPacketList( internalPacket );
				
				internalPacket = BuildPacketFromSplitPacketList( internalPacket->splitPacketId, time );
				
				if ( internalPacket == 0 )
				{
#ifdef _DEBUG
					//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
					assert( splitPacketList.size() == splitPacketListSize );
					//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
#endif
					
					// Don't have all the parts yet
					goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
				}
				
#ifdef _DEBUG
				else
				{
					//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
					assert( splitPacketList.size() == splitPacketListSize - internalPacket->splitPacketCount );
					//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
				}
				
#endif
				// else continue down to handle RELIABLE_ORDERED
			}
			
			if ( internalPacket->reliability == RELIABLE_ORDERED )
			{
#ifdef _DEBUG
				assert( internalPacket->orderingChannel < NUMBER_OF_ORDERED_STREAMS );
#endif
				
				if ( internalPacket->orderingChannel >= NUMBER_OF_ORDERED_STREAMS )
				{
					// Invalid packet
					delete [] internalPacket->data;
					InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
					goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
				}
				
				if ( waitingForOrderedPacketReadIndex[ internalPacket->orderingChannel ] == internalPacket->orderingIndex )
				{
					// Get the list to hold ordered packets for this stream
					BasicDataStructures::LinkedList<InternalPacket*> *orderingListAtOrderingStream;
					unsigned char orderingChannelCopy = internalPacket->orderingChannel;
					
					statistics.orderedMessagesInOrder++;
					
					// Push the packet for the user to read
					//     reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
					outputQueue.push( internalPacket );
					//     reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
					internalPacket = 0; // Don't reference this any longer since other threads access it
					
					// Wait for the next ordered packet in sequence
					waitingForOrderedPacketReadIndex[ orderingChannelCopy ] ++; // This wraps at 255
					
					//reliabilityLayerMutexes[orderingList_MUTEX].Lock();
					orderingListAtOrderingStream = GetOrderingListAtOrderingStream( orderingChannelCopy );
					
					if ( orderingListAtOrderingStream != 0 )
					{
						// There is a list for this ordering channel that may contain waiting packets. Pop them in order
						
						// Scan the list and pop packets that were delayed due to ordering in order
						
						while ( orderingListAtOrderingStream->size() > 0 )
						{
							orderingListAtOrderingStream->beginning();
							count = orderingListAtOrderingStream->size();
							pushedPacket = false;
							
							while ( count != 0 )
							{
								if ( orderingListAtOrderingStream->peek() ->orderingIndex == waitingForOrderedPacketReadIndex[ orderingChannelCopy ] )
								{
									// A packet was waiting, so output it in order.
									// This is already mutexed above
									
									// Push the packet for the user to read
									//         reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
									outputQueue.push( orderingListAtOrderingStream->pop() );
									//         reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
									
									// Wait for the next ordered packet in sequence
									waitingForOrderedPacketReadIndex[ orderingChannelCopy ] ++; // This wraps at 255
									
									pushedPacket = true;
									
									break;
								}
								
								( *orderingListAtOrderingStream ) ++;
								count--;
							}
							
							if ( pushedPacket == false )
							{
								break; // No more packets to pop
							}
						}
						
					}
					
					//reliabilityLayerMutexes[orderingList_MUTEX].Unlock();
					
					internalPacket = 0;
				}
				
				else
				{
					statistics.orderedMessagesOutOfOrder++;
					
					// This is a newer ordered packet than we are waiting for. Store it for future use
					AddToOrderingList( internalPacket );
				}
				
				goto CONTINUE_SOCKET_DATA_PARSE_LOOP;
			}
			
			// Nothing special about this packet.  Add it to the output queue
			//   reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
			outputQueue.push( internalPacket );
			
			//   reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
			
			// Output queue fill rate test
			//   if (outputQueue.size()%50==0)
			//    printf("outputQueue.size()=%i Time=%i\n", outputQueue.size(), RakNet::GetTime());
			
			internalPacket = 0;
		}
		
		// Used for a goto to jump to the next packet immediately
		
	CONTINUE_SOCKET_DATA_PARSE_LOOP:
		// Parse the bitstream to create an internal packet
		internalPacket = CreateInternalPacketFromBitStream( &socketData, time );
	}
	
	
	// numberOfAcksInFrame>=windowSize is almost never true
	if ( numberOfAcksInFrame >= windowSize && ( sendQueue[ SYSTEM_PRIORITY ].size() > 0 || sendQueue[ HIGH_PRIORITY ].size() > 0 || sendQueue[ MEDIUM_PRIORITY ].size() > 0 ) )
	{
	
		// reliabilityLayerMutexes[windowSize_MUTEX].Lock();
		
		if ( windowSize < lossyWindowSize || time - lastWindowIncreaseSizeTime > lostPacketResendDelay * 2 )   // Increases the window size slowly, testing for packetloss
		{
			// If we get a frame which clears out the resend queue after handling one or more acks, and we have packets waiting to go out,
			// and we didn't recently lose a packet then increase the window size by 1
			windowSize++;
			
			if ( time - lastWindowIncreaseSizeTime > lostPacketResendDelay * 2 )   // The increase is to test for packetloss
				lastWindowIncreaseSizeTime = time;
				
			// If the window is so large that we couldn't possibly fit any more packets into the frame, then just leave it alone
			if ( windowSize > MAXIMUM_WINDOW_SIZE )
				windowSize = MAXIMUM_WINDOW_SIZE;
				
			// SHOW - WINDOWING
			//else
			// printf("Increasing windowSize to %i.  Lossy window size = %i\n", windowSize, lossyWindowSize);
			
			// If we are more than 5 over the lossy window size, increase the lossy window size by 1
			if ( windowSize == MAXIMUM_WINDOW_SIZE || windowSize - lossyWindowSize > 5 )
				lossyWindowSize++;
		}
		// reliabilityLayerMutexes[windowSize_MUTEX].Unlock();
	}
	
	return true;
}

//-------------------------------------------------------------------------------------------------------
// This gets an end-user packet already parsed out. Returns number of BITS put into the buffer
//-------------------------------------------------------------------------------------------------------
int ReliabilityLayer::Receive( char **data )
{
	InternalPacket * internalPacket;
	
	// reliabilityLayerMutexes[outputQueue_MUTEX].Lock();
	
	if ( outputQueue.size() > 0 )
	{
		//  #ifdef _DEBUG
		//  assert(bitStream->GetNumberOfBitsUsed()==0);
		//  #endif
		internalPacket = outputQueue.pop();
		//  reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
		
		//printf("In Receive %i bytes\n",(internalPacket->dataBitLength-1)/8+1);
		//for (int ass=0; ass < (internalPacket->dataBitLength-1)/8+1 && ass < 10; ass++)
		// printf("%i ", ((char*)(internalPacket->data))[ass]);
		//printf("\n\n");
		
		//bitStream->SetData((const unsigned char*)internalPacket->data,internalPacket->dataBitLength);
		//delete [] internalPacket->data;
		int bitLength;
		*data = internalPacket->data;
		bitLength = internalPacket->dataBitLength;
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return bitLength;
	}
	
	else
	{
		// reliabilityLayerMutexes[outputQueue_MUTEX].Unlock();
		return 0;
	}
	
}

//-------------------------------------------------------------------------------------------------------
// Puts data on the send queue
// bitStream contains the data to send
// priority is what priority to send the data at
// reliability is what reliability to use
// ordering channel is from 0 to 255 and specifies what stream to use
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::Send( RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, unsigned char orderingChannel, bool makeDataCopy, int MTUSize, std::vector<DWORD> vecUID_  )
{
#ifdef _DEBUG
	assert( !( reliability > RELIABLE_SEQUENCED || reliability < 0 ) );
	assert( !( priority > NUMBER_OF_PRIORITIES || priority < 0 ) );
	assert( !( orderingChannel < 0 || orderingChannel >= NUMBER_OF_ORDERED_STREAMS ) );
	assert( bitStream->GetNumberOfBytesUsed() > 0 );
#endif
	
#ifdef __USE_IO_COMPLETION_PORTS
	
	if ( readWriteSocket == INVALID_SOCKET )
		return false;
		
#endif
		
	// Fix any bad parameters
	if ( reliability > RELIABLE_SEQUENCED || reliability < 0 )
		reliability = RELIABLE;
		
	if ( priority > NUMBER_OF_PRIORITIES || priority < 0 )
		priority = HIGH_PRIORITY;
		
	if ( orderingChannel >= NUMBER_OF_ORDERED_STREAMS )
		orderingChannel = 0;

	if ( bitStream->GetNumberOfBytesUsed() == 0 )
	{
#ifdef _DEBUG
		printf( "Error!! ReliabilityLayer::Send bitStream->GetNumberOfBytesUsed()==0\n" );
#endif		
		return false;
	}
	
	InternalPacket * internalPacket = InternalPacketPool::Instance() ->GetPointer();
#ifdef _DEBUG
	// Remove boundschecker accessing undefined memory error
    memset( internalPacket, 255, sizeof( InternalPacket ) - sizeof(std::vector<DWORD>) );

#endif
	//====================================================================================
    // 2006.07.05 : Asirion
    // 여기서 부터 제대로된 Send Logic이다.
	internalPacket->creationTime = RakNet::GetTime();
	
	if ( makeDataCopy )
	{
		internalPacket->data = new char [ bitStream->GetNumberOfBytesUsed() ];
		memcpy( internalPacket->data, bitStream->GetData(), bitStream->GetNumberOfBytesUsed() );
	}
	else
    {
        internalPacket->data = ( char* ) bitStream->GetData();
    }
		
	internalPacket->dataBitLength = bitStream->GetNumberOfBitsUsed();
	
	internalPacket->isAcknowledgement = false;
	
	internalPacket->nextActionTime = 0;
	
	reliabilityLayerMutexes[ packetNumber_MUTEX ].Lock();
	
	internalPacket->packetNumber = packetNumber;
	
	reliabilityLayerMutexes[ packetNumber_MUTEX ].Unlock();
	
	internalPacket->priority = priority;
	
	internalPacket->reliability = reliability;
	
	internalPacket->splitPacketCount = 0;

    internalPacket->resendCount = 0;
	
	// Calculate if I need to split the packet
	int headerLength = BITS_TO_BYTES( GetBitStreamHeaderLength( internalPacket ) );
	
	int maxDataSize = MTUSize - UDP_HEADER_SIZE - headerLength;
	
	if ( encryptor.IsKeySet() )
		maxDataSize -= 16; // Extra data for the encryptor
		
	bool splitPacket = bitStream->GetNumberOfBytesUsed() > maxDataSize;
	
	// If a split packet, we might have to upgrade the reliability
	if ( splitPacket )
		statistics.numberOfSplitMessages++;
	else
		statistics.numberOfUnsplitMessages++;
		
	reliabilityLayerMutexes[ packetNumber_MUTEX ].Lock();
	
	if ( ++packetNumber == RECEIVED_PACKET_LOG_LENGTH )
		packetNumber = 0;
		
	reliabilityLayerMutexes[ packetNumber_MUTEX ].Unlock();
	
    if ( internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == UNRELIABLE_SEQUENCED )
	{
		internalPacket->orderingChannel = orderingChannel;
		reliabilityLayerMutexes[ waitingForSequencedPacketWriteIndex_MUTEX ].Lock();
		internalPacket->orderingIndex = waitingForSequencedPacketWriteIndex[ orderingChannel ] ++;
		reliabilityLayerMutexes[ waitingForSequencedPacketWriteIndex_MUTEX ].Unlock();	
	}
	else
    {
        if ( internalPacket->reliability == RELIABLE_ORDERED )
        {
            internalPacket->orderingChannel = orderingChannel;
            reliabilityLayerMutexes[ waitingForOrderedPacketWriteIndex_MUTEX ].Lock();
            internalPacket->orderingIndex = waitingForOrderedPacketWriteIndex[ orderingChannel ] ++;
            reliabilityLayerMutexes[ waitingForOrderedPacketWriteIndex_MUTEX ].Unlock();
        }
    }
		
	if ( splitPacket )   // If it uses a secure header it will be generated here
	{
		// Must split the packet.  This will also generate the SHA1 if it is required. It also adds it to the send list.
		SplitPacketAndDeleteOriginal( internalPacket, MTUSize, vecUID_ );
		return true;
	}
    //====================================================================================
    // 2006.07.06 : Asirion
    // sendQueue에 InternalPacket을 만들어 Push함으로
    // UID vector를 채워서 넣어줘야 한다.
    if ( vecUID_.size() != 0 )
        internalPacket->vecRelayUID = vecUID_;
    else
        internalPacket->vecRelayUID.clear();
	reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + internalPacket->priority ].Lock();
	sendQueue[ internalPacket->priority ].push( internalPacket );
	reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + internalPacket->priority ].Unlock();
	
	return true;
}

//-------------------------------------------------------------------------------------------------------
// Run this once per game cycle.  Handles internal lists and actually does the send
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::Update( SOCKET s, PlayerID playerId, int MTUSize, unsigned long time )
{
#ifdef __USE_IO_COMPLETION_PORTS

	if ( readWriteSocket == INVALID_SOCKET )
		return ;
		
#endif
	// unsigned resendQueueSize;
	bool reliableDataSent;
	
	unsigned long lastAck;
	
	if ( freeThreadedMemoryOnNextUpdate )
	{
		freeThreadedMemoryOnNextUpdate = false;
		FreeThreadedMemory();
	}
	
	// Accuracy isn't important on this value, and since this is called so often the mutex is sometimes causing deadlock problems.
	// So it is presently disabled
	// reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
	// resendQueueSize=resendQueue.size();
	// reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
	
	// reliabilityLayerMutexes[lastAckTime_MUTEX].Lock();
	lastAck = lastAckTime;
	
	// reliabilityLayerMutexes[lastAckTime_MUTEX].Unlock();
	
	// Due to thread vagarities and the way I store the time to avoid slow calls to RakNet::GetTime
	// time may be less than lastAck
	//if ( resendQueue.size() > 0 && time > lastAck && lastAck && time - lastAck > TIMEOUT_TIME )
	//{
	//	// SHOW - dead connection
	//	// printf("The connection has been lost.\n");
	//	// We've waited a very long time for a reliable packet to get an ack and it never has
	//	deadConnection = true;
	//	return ;
	//}
	
	//if (outputWindowFullTime && RakNet::GetTime() > TIMEOUT_TIME + outputWindowFullTime)
	//{
	// // We've waited a long time with no data from the other system.  Assume the connection is lost
	// deadConnection=true;
	// return;
	//}
	
	// Not a frame but a packet actually.
	// However, in a sense it is a frame because we are filling multiple logical packets into one datagram
	//reliabilityLayerMutexes[updateBitStream_MUTEX].Lock();
	
	// Keep sending to available bandwidth
	while ( IsFrameReady( time ) )
	{
		updateBitStream.Reset();
        //====================================================================================
        // 2006.07.06 : Asirion
        // RelayServer를 통하는 ACK전용 ReliabilityLayer일 경우에는 
        // RelayServer Packet Header를 포함 시켜준다.
        if ( playerId == ms_kRelayServerID && m_bRelayMain == false )
        {
            updateBitStream.Write( (char*)&m_bRelayMain, sizeof(bool) );
            updateBitStream.Write( ms_dwUID );
            updateBitStream.Write( (unsigned char)m_vecUID.size() );
            for ( int i = 0; i < (int)m_vecUID.size(); i++ )
            {
                updateBitStream.Write( (DWORD)m_vecUID[i] );
            }
        }
		if ( GenerateFrame( &updateBitStream, MTUSize, &reliableDataSent, time, playerId ) == false ) 
            break;
		
		if ( updateBitStream.GetNumberOfBitsUsed() > 0 )
		{
#ifndef _INTERNET_SIMULATOR
			SendBitStream( s, playerId, &updateBitStream );
#else
			// Delay the send to simulate lag
			DataAndTime *dt;
			dt = new DataAndTime;
			memcpy( dt->data, updateBitStream.GetData(), updateBitStream.GetNumberOfBytesUsed() );
			dt->length = updateBitStream.GetNumberOfBytesUsed();
			dt->sendTime = time + 100 + ( randomMT() % 100 );
			delayList.insert( dt );
#endif
			
		}
		
		else
			break;
	}
	
	
#ifdef _INTERNET_SIMULATOR
	// Do any lagged sends
	unsigned i = 0;
	
	while ( i < delayList.size() )
	{
		if ( delayList[ i ] ->sendTime < time )
		{
			updateBitStream.Reset();
			updateBitStream.Write( delayList[ i ] ->data, delayList[ i ] ->length );
			// Send it now
			SendBitStream( s, playerId, &updateBitStream );
			
			delete delayList[ i ];
			delayList[ i ] = delayList[ delayList.size() - 1 ];
			delayList.del();
		}
		
		else
			i++;
	}
	
#endif
	
	//reliabilityLayerMutexes[updateBitStream_MUTEX].Unlock();
}

//-------------------------------------------------------------------------------------------------------
// Writes a bitstream to the socket
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SendBitStream( SOCKET s, PlayerID playerId, RakNet::BitStream *bitStream )
{

	int oldLength, length;	
	// Encode the whole bitstream if the encoder is defined.
	
	if ( encryptor.IsKeySet() )
	{
		length = bitStream->GetNumberOfBytesUsed();
		oldLength = length;
		
		encryptor.Encrypt( ( unsigned char* ) bitStream->GetData(), length, ( unsigned char* ) bitStream->GetData(), &length );
		statistics.encryptionBitsSent = ( length - oldLength ) * 8;
		assert( ( length % 16 ) == 0 );
	}
	else
	{
		length = bitStream->GetNumberOfBytesUsed();
	}
	
#ifdef __USE_IO_COMPLETION_PORTS
	if ( readWriteSocket == INVALID_SOCKET )
	{
		assert( 0 );
		return ;
	}
	
	statistics.packetsSent++;
	statistics.totalBitsSent += length * 8;
	SocketLayer::Instance() ->Write( readWriteSocket, ( const char* ) bitStream->GetData(), length );
#else
	
	statistics.packetsSent++;
	statistics.totalBitsSent += length * 8;
	//printf("total bits=%i length=%i\n", BITS_TO_BYTES(statistics.totalBitsSent), length);
	SocketLayer::Instance() ->SendTo( s, ( char* ) bitStream->GetData(), length, playerId.binaryAddress, playerId.port );
#endif // __USE_IO_COMPLETION_PORTS
	
	// lastPacketSendTime=time;
}

//-------------------------------------------------------------------------------------------------------
// Returns true if we can or should send a frame.  False if we should not
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::IsFrameReady( unsigned long time )
{
	if ( IsSendThrottled() == false )
		return true;
		
	//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Lock();
	
	// Any acknowledgement packets waiting?  We will send these even if the send is throttled.
	// Otherwise the throttle may never end
	if ( acknowledgementQueue.size() >= MINIMUM_WINDOW_SIZE
		// Try not waiting to send acks - will take more bandwidth but maybe less packetloss
		// || acknowledgementQueue.peek()->nextActionTime < time
	   )
	{
		//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
		return true;
	}
	
	// reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
	
	// Does the oldest packet need to be resent?  If so, send it.
	// Otherwise the throttle may never end
	// reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
//	if ( resendQueue.size() > 0 && resendQueue.peek() && resendQueue.peek() ->nextActionTime < time )
	if ( resendQueue.size() > 0 )
	{
		//  reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
		return true;
	}
	
	// reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
	
	// Send is throttled.  Don't send.
	return false;
}

//-------------------------------------------------------------------------------------------------------
// Generates a frame (coalesced packets)
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::WriteRelayPacketHeader( RakNet::BitStream* pkBitStream_, std::vector<DWORD>* pvecUID_ )
{    
    pkBitStream_->Write( (char*)&m_bRelayMain, sizeof(bool) );
    pkBitStream_->Write( ms_dwUID );
    pkBitStream_->Write( (unsigned char)pvecUID_->size() );
    for ( int i = 0; i < (int)pvecUID_->size(); i++ )
    {
        pkBitStream_->Write( (DWORD)((*pvecUID_)[i]) );
    }
}
bool ReliabilityLayer::GenerateFrame( RakNet::BitStream *output, int MTUSize, bool *reliableDataSent, unsigned long time, const PlayerID& kPlayerID_ )
{
	InternalPacket * internalPacket;
	int maxDataBitSize;
	int reliableBits = 0;
	int nextPacketBitLength;
	unsigned i;
	bool isReliable, onlySendUnreliable;
	bool acknowledgementPacketsSent;
	bool anyPacketsLost = false;
    bool bGenerateData = false;
	
	maxDataBitSize = MTUSize - UDP_HEADER_SIZE;
	
	if ( encryptor.IsKeySet() )
		maxDataBitSize -= 16; // Extra data for the encryptor
		
	maxDataBitSize <<= 3;
	
	acknowledgementPacketsSent = false;
	
	*reliableDataSent = false;
	
	
	if ( acknowledgementQueue.size() > 0 &&
		( acknowledgementQueue.size() >= MINIMUM_WINDOW_SIZE ||
		  acknowledgementQueue.peek() ->nextActionTime < time ) )
	{
        if ( m_bRelayMain == true )
            START_LOG( cerr, L"SendAck From RelayMain ReliabilityLayer" );
		do
		{
			internalPacket = acknowledgementQueue.pop();			
			statistics.acknowlegementsSent++;
            bGenerateData = true;
			statistics.acknowlegementBitsSent += WriteToBitStreamFromInternalPacket( output, internalPacket );
			acknowledgementPacketsSent = true;
			
			// Delete the acknowledgement
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			
			if ( output->GetNumberOfBitsUsed() + ACK_BIT_LENGTH > maxDataBitSize )
			{	
				statistics.packetsContainingOnlyAcknowlegements++;
				goto END_OF_GENERATE_FRAME;
			}
		}
		
		while ( acknowledgementQueue.size() > 0 );
	}
    
	while ( resendQueue.size() > 0 )
	{
		if ( resendQueue.peek() == 0 )
		{
			resendQueue.pop();
			continue; // This was a hole
		}
#ifdef _DEBUG
        if ( m_vecUID.size() != 0 && m_bRelayMain == false )
            START_LOG( cerr, "ReSendQ size is not zero, This reliabilityLayer is For ACK" );
#endif		
		if ( resendQueue.peek()->nextActionTime < time )
		{
            if ( bGenerateData == true && m_bRelayMain == true )
            {
                if ( m_vecLastSendedUID != resendQueue.peek()->vecRelayUID )
                {
                    return bGenerateData;
                }
            }

			internalPacket = resendQueue.pop(); 
            if ( internalPacket->resendCount >= MAX_RESEND_COUNT )
            {
                delete [] internalPacket->data;
                InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
                
                continue;
            }
            
            nextPacketBitLength = GetBitStreamHeaderLength( internalPacket ) + internalPacket->dataBitLength;
			if ( output->GetNumberOfBitsUsed() + nextPacketBitLength > maxDataBitSize )
			{
				resendQueue.pushAtHead( internalPacket ); // Not enough room to use this packet after all!
				
				if ( anyPacketsLost )
				{
					UpdatePacketloss( time );
				}
				
				goto END_OF_GENERATE_FRAME;
			}
			
#ifdef _DEBUG
			assert( internalPacket->priority >= 0 );
			assert( internalPacket->reliability >= 0 );			
#endif
			
			statistics.messageResends++;			
			statistics.messageDataBitsResent += internalPacket->dataBitLength;
            if ( kPlayerID_ == ms_kRelayServerID && bGenerateData == false )
            {
                WriteRelayPacketHeader( output, &internalPacket->vecRelayUID );
            }
			statistics.messagesTotalBitsResent += WriteToBitStreamFromInternalPacket( output, internalPacket );			
            bGenerateData = true;
			*reliableDataSent = true;		
			statistics.packetsContainingOnlyAcknowlegementsAndResends++;
			anyPacketsLost = true;
			internalPacket->nextActionTime = time + lostPacketResendDelay;
            internalPacket->resendCount++;
			InsertPacketIntoResendQueue( internalPacket, time );

            m_vecLastSendedUID = internalPacket->vecRelayUID;
		}		
		else
		{
			break;
		}
	}

	if ( anyPacketsLost )
	{
		UpdatePacketloss( time );
	}
	
	onlySendUnreliable = false;
	
	if ( IsSendThrottled() )
		return bGenerateData; // Don't send regular data if we are supposed to be waiting on the window
		
	for ( i = 0; i < NUMBER_OF_PRIORITIES; i++ )
	{
		if ( sendQueue[ i ].size() == 0 )
			continue;
#ifdef _DEBUG
        if ( m_vecUID.size() != 0 && m_bRelayMain == false )
            START_LOG( cerr, "SendQ size is not zero, This reliabilityLayer is For ACK" );
#endif
		reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Lock();
		
		while ( sendQueue[ i ].size() )
		{
            if ( bGenerateData == true && m_bRelayMain == true )
            {
                if ( m_vecLastSendedUID != sendQueue[ i ].peek()->vecRelayUID )
                {
                    reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Unlock();
                    return bGenerateData;
                }
            }
			internalPacket = sendQueue[ i ].pop();

			reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Unlock();
			
			nextPacketBitLength = GetBitStreamHeaderLength( internalPacket ) + internalPacket->dataBitLength;
			
			if ( output->GetNumberOfBitsUsed() + nextPacketBitLength > maxDataBitSize )
			{
				// This output won't fit.
				reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Lock();
				sendQueue[ i ].push( internalPacket ); // Push this back at the head so it is the next thing to go out
				break;
			}
			
			if ( internalPacket->reliability == RELIABLE || internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_ORDERED )
				isReliable = true;
			else
				isReliable = false;
				
			// Write to the output bitstream
			//   sentPackets++;
			statistics.messagesSent[ i ] ++;
			
			statistics.messageDataBitsSent[ i ] += internalPacket->dataBitLength;

            if ( kPlayerID_ == ms_kRelayServerID && bGenerateData == false )
            {
                WriteRelayPacketHeader( output, &internalPacket->vecRelayUID );
            }
			statistics.messageTotalBitsSent[ i ] += WriteToBitStreamFromInternalPacket( output, internalPacket );
            bGenerateData = true;

            m_vecLastSendedUID = internalPacket->vecRelayUID;
			if ( isReliable )
			{
				reliableBits += internalPacket->dataBitLength;
				internalPacket->nextActionTime = time + lostPacketResendDelay;
				InsertPacketIntoResendQueue( internalPacket, time );				
				*reliableDataSent = true;
			}
			
			else
			{
				delete [] internalPacket->data;
				InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			}
			
			reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Lock();
		}
		
		reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + i ].Unlock();
		
	}
	
	// Optimization - if we sent data but didn't send an acknowledgement packet previously then send them now
	if ( acknowledgementPacketsSent == false && output->GetNumberOfBitsUsed() > 0 )
	{
		if ( acknowledgementQueue.size() > 0 )
		{
            if ( m_bRelayMain == true )
                START_LOG( cerr, L"SendAck From RelayMain ReliabilityLayer" );

			while ( output->GetNumberOfBitsUsed() + ACK_BIT_LENGTH < maxDataBitSize && acknowledgementQueue.size() > 0 )
			{
				internalPacket = acknowledgementQueue.pop();
                bGenerateData = true;
				WriteToBitStreamFromInternalPacket( output, internalPacket );
				InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			}
		}
	}
	
END_OF_GENERATE_FRAME:
	;
    return bGenerateData;
}


//-------------------------------------------------------------------------------------------------------
// This will return true if we should not send at this time
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::IsSendThrottled( void )
{
	int win;
	
	// reliabilityLayerMutexes[windowSize_MUTEX].Lock();
	win = windowSize;
	// reliabilityLayerMutexes[windowSize_MUTEX].Unlock();
	
	return ( int ) resendQueue.size() >= win;
}

//-------------------------------------------------------------------------------------------------------
// We lost a packet
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::UpdatePacketloss( unsigned long time )
{
	// unsigned long time = RakNet::GetTime();
	/*
	  maximumWindowSize = (unsigned int)((double)maximumWindowSize * DECREASE_THROUGHPUT_DELTA);
	  if (maximumWindowSize < MINIMUM_THROUGHPUT)
	  {
	  maximumWindowSize = MINIMUM_THROUGHPUT;
	  }
	*/
	
	//printf("Lost packet. resendQueue.size()=%i sendQueue[0].size() = %i\n",resendQueue.size(), sendQueue[0].size());
	
	// reliabilityLayerMutexes[windowSize_MUTEX].Lock();
	
	
	// reliabilityLayerMutexes[windowSize_MUTEX].Unlock();
	// retransmittedFrames++;
	
	// The window size will decrease everytime we have to retransmit a frame
	//reliabilityLayerMutexes[windowSize_MUTEX].Lock();
	
	if ( --windowSize < MINIMUM_WINDOW_SIZE )
		windowSize = MINIMUM_WINDOW_SIZE;
		
	//reliabilityLayerMutexes[windowSize_MUTEX].Unlock();
	lossyWindowSize = windowSize;
	
	lastWindowIncreaseSizeTime = time; // This will block the window size from increasing immediately
	
	// SHOW - windowing
	// if (resendQueue.size()>0)
	//  printf("Frame lost.  New window size = %i.  Lossy window size = %i. Time=%i. Next send time=%i\n", windowSize, lossyWindowSize, RakNet::GetTime(),resendQueue.peek()->nextActionTime);
}

//-------------------------------------------------------------------------------------------------------
// Does what the function name says
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::RemovePacketFromResendQueueAndDeleteOlderReliableSequenced( PacketNumberType packetNumber, DWORD dwUID_ )
{
	InternalPacket * internalPacket;
	PacketReliability reliability; // What type of reliability algorithm to use with this packet
	unsigned char orderingChannel; // What ordering channel this packet is on, if the reliability type uses ordering channels
	unsigned char orderingIndex; // The ID used as identification for ordering channels
	
	// reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
	
	for ( unsigned i = 0; i < resendQueue.size(); i ++ )
	{
		if ( resendQueue[ i ] && packetNumber == resendQueue[ i ] ->packetNumber )
		{
			// Found what we wanted to ack
            //====================================================================================
            // 2006.07.06 : Asirion
            // RelayMain ReliabilityLayer일 경우에는 Ack를 처리 하는 방식을 달리한다.
            if ( m_bRelayMain == true )
            {
                internalPacket = resendQueue[i];
                std::vector<DWORD>::iterator iter = std::find( internalPacket->vecRelayUID.begin(), internalPacket->vecRelayUID.end(), dwUID_ );
                if ( iter != internalPacket->vecRelayUID.end() )
                {
                    internalPacket->vecRelayUID.erase( iter );
                }
                if ( internalPacket->vecRelayUID.size() != 0 )
                    return true;
            }

            statistics.acknowlegementsReceived++;		
            if ( i == 0 )
                internalPacket = resendQueue.pop();
            else
            {
                internalPacket = resendQueue[ i ];
                resendQueue.del( i );
            }

            //reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();

            // Save some of the data of the packet
            reliability = internalPacket->reliability;

            orderingChannel = internalPacket->orderingChannel;

            orderingIndex = internalPacket->orderingIndex;

            // Delete the packet
            delete [] internalPacket->data;

            InternalPacketPool::Instance() ->ReleasePointer( internalPacket );

            // If the deleted packet was reliable sequenced, also delete all older reliable sequenced resends on the same ordering channel.
            // This is because we no longer need to send these.
            if ( reliability == RELIABLE_SEQUENCED )
            {
                unsigned j = 0;

                //reliabilityLayerMutexes[resendQueue_MUTEX].Lock();

                while ( j < resendQueue.size() )
                {
                    internalPacket = resendQueue[ j ];

                    if ( internalPacket && internalPacket->reliability == RELIABLE_SEQUENCED && internalPacket->orderingChannel == orderingChannel && IsOlderOrderedPacket( internalPacket->orderingIndex, orderingIndex ) )
                    {
                        // Delete the packet
                        delete [] internalPacket->data;
                        InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
                        resendQueue[ j ] = 0; // Generate a hole
                    }
                    j++;
                }
            }
            //reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
            return true;
		}
	}
	
	//reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
	
	// Didn't find what we wanted to ack
	statistics.duplicateAcknowlegementsReceived++;
    return true;
}

//-------------------------------------------------------------------------------------------------------
// Acknowledge receipt of the packet with the specified packetNumber
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SendAcknowledgementPacket( PacketNumberType packetNumber, unsigned long time  )
{
	InternalPacket * internalPacket;
	
	// Disabled - never gets called anyway so just wastes CPU cycles
	/*
	// High load optimization - if there are over 100 acks waiting scan the list to make sure what we are adding isn't already scheduled to go out
	reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Lock();
	size = acknowledgementQueue.size();
	if (size>100)
	{
	for (i=0; i < size; i++)
	{
	internalPacket=acknowledgementQueue[i];
	if (internalPacket && internalPacket->packetNumber==packetNumber)
	{
	reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
	//printf("Eliminating duplicate ack. acknowledgementQueue.size()=%i\n",acknowledgementQueue.size());
	return; // No need to add it - it is already here
	}
	}
	}
	reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
	*/
	
	internalPacket = InternalPacketPool::Instance() ->GetPointer();
#ifdef _DEBUG
	// Remove boundschecker accessing undefined memory error
    memset( internalPacket, 255, sizeof( InternalPacket ) - sizeof(std::vector<DWORD>) );
#endif
	
	internalPacket->packetNumber = packetNumber;
	internalPacket->isAcknowledgement = true;
	
	internalPacket->creationTime = time;
	// We send this acknowledgement no later than 1/4 the time the remote
	//machine would send the original packet again
	// DEBUG
	internalPacket->nextActionTime = internalPacket->creationTime + ( lostPacketResendDelay >> 2 );
	//internalPacket->nextActionTime = internalPacket->creationTime;
	//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Lock();
	acknowledgementQueue.push( internalPacket );
	// printf("<Server>Adding ack at time %i. acknowledgementQueue.size=%i\n",RakNet::GetTime(), acknowledgementQueue.size());
	//reliabilityLayerMutexes[acknowledgementQueue_MUTEX].Unlock();
}

//-------------------------------------------------------------------------------------------------------
// Parse an internalPacket and figure out how many header bits would be
// written.  Returns that number
//-------------------------------------------------------------------------------------------------------
int ReliabilityLayer::GetBitStreamHeaderLength( const InternalPacket *const internalPacket )
{
#ifdef _DEBUG
	assert( internalPacket );
#endif
	
	int bitLength;
	
	if ( internalPacket->isAcknowledgement )
		return ACK_BIT_LENGTH;
		
	// Write if this packet has a security header (1 bit)
	//bitStream->Write(internalPacket->hasSecurityHeader);
	// -- bitLength+=1;
	bitLength = ACK_BIT_LENGTH;
	
	// Write the PacketReliability.  This is encoded in 3 bits
	//bitStream->WriteBits((unsigned char*)&(internalPacket->reliability), 3, true);
	bitLength += 3;
	
	// If the reliability requires an ordering channel and ordering index, we Write those.
	if ( internalPacket->reliability == UNRELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_ORDERED )
	{
		// ordering channel encoded in 5 bits (from 0 to 31)
		//bitStream->WriteBits((unsigned char*)&(internalPacket->orderingChannel), 5, true);
		// -- bitLength+=5;
		
		// ordering index is one byte
		//bitStream->Write(internalPacket->orderingIndex);
		// -- bitLength+=8;
		bitLength += 13;
	}
	
	// Write if this is a split packet (1 bit)
	bool isSplitPacket = internalPacket->splitPacketCount > 0;
	
	//bitStream->Write(isSplitPacket);
	bitLength += 1;
	
	if ( isSplitPacket )
	{
		// split packet indices are two bytes (so one packet can be split up to 65535
		// times - maximum packet size would be about 500 * 65535)
		//bitStream->Write(internalPacket->splitPacketId);
		//bitStream->WriteCompressed(internalPacket->splitPacketIndex);
		//bitStream->WriteCompressed(internalPacket->splitPacketCount);
		bitLength += 3 * 8 * 2;
	}
	
	// Write how many bits the packet data is. Stored in an unsigned short and
	// read from 16 bits
	//bitStream->WriteBits((unsigned char*)&(internalPacket->dataBitLength), 16, true);
	
	// Read how many bits the packet data is.  Stored in 16 bits
	bitLength += 16;
	
	// Byte alignment
	//bitLength += 8 - ((bitLength -1) %8 + 1);
	
	return bitLength;
}

//-------------------------------------------------------------------------------------------------------
// Parse an internalPacket and create a bitstream to represent this data
//-------------------------------------------------------------------------------------------------------
int ReliabilityLayer::WriteToBitStreamFromInternalPacket( RakNet::BitStream *bitStream, const InternalPacket *const internalPacket )
{
#ifdef _DEBUG
	assert( bitStream && internalPacket );
#endif
	
	int start = bitStream->GetNumberOfBitsUsed();
	
	// testing
	// if (internalPacket->reliability==UNRELIABLE)
	//  printf("Sending unreliable packet %i\n", internalPacket->packetNumber);
	// else if (internalPacket->reliability==RELIABLE_SEQUENCED || internalPacket->reliability==RELIABLE_ORDERED || internalPacket->reliability==RELIABLE)
	//  printf("Sending reliable packet number %i\n", internalPacket->packetNumber);
	
	//bitStream->AlignWriteToByteBoundary();
	
	// Write the packet number (2 bytes)
	bitStream->Write( internalPacket->packetNumber );
	
	// Write if this packet is an acknowledgement (1 bit)
	bitStream->Write( internalPacket->isAcknowledgement );
	// Acknowledgement packets have no more data than the packetnumber and whether it is an acknowledgement
	
	if ( internalPacket->isAcknowledgement )
	{
		return bitStream->GetNumberOfBitsUsed() - start;
	}
	
#ifdef _DEBUG
	assert( internalPacket->dataBitLength > 0 );
	
#endif
	
	// Write the PacketReliability.  This is encoded in 3 bits
	unsigned char reliability = ( unsigned char ) internalPacket->reliability;
	
	bitStream->WriteBits( ( unsigned char* ) ( &( reliability ) ), 3, true );
	
	// If the reliability requires an ordering channel and ordering index, we Write those.
	if ( internalPacket->reliability == UNRELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_ORDERED )
	{
		// ordering channel encoded in 5 bits (from 0 to 31)
		bitStream->WriteBits( ( unsigned char* ) & ( internalPacket->orderingChannel ), 5, true );
		
		// ordering index is one byte
		bitStream->Write( internalPacket->orderingIndex );
	}
	
	// Write if this is a split packet (1 bit)
	bool isSplitPacket = internalPacket->splitPacketCount > 0;
	
	bitStream->Write( isSplitPacket );
	
	if ( isSplitPacket )
	{
		// split packet indices are two bytes (so one packet can be split up to 65535 times - maximum packet size would be about 500 * 65535)
		bitStream->Write( internalPacket->splitPacketId );
		bitStream->WriteCompressed( internalPacket->splitPacketIndex );
		bitStream->WriteCompressed( internalPacket->splitPacketCount );
	}
	
	// Write how many bits the packet data is. Stored in 13 bits
#ifdef _DEBUG
	assert( BITS_TO_BYTES( internalPacket->dataBitLength ) < MAXIMUM_MTU_SIZE ); // I never send more than MTU_SIZE bytes
	
#endif
	
	unsigned short length = ( unsigned short ) internalPacket->dataBitLength; // Ignore the 2 high bytes for WriteBits
	
	bitStream->WriteCompressed( length );
	
	// Write the actual data.
	bitStream->WriteAlignedBytes( ( unsigned char* ) internalPacket->data, BITS_TO_BYTES( internalPacket->dataBitLength ) );
	//bitStream->WriteBits((unsigned char*)internalPacket->data, internalPacket->dataBitLength);
	
	return bitStream->GetNumberOfBitsUsed() - start;
}

//-------------------------------------------------------------------------------------------------------
// Parse a bitstream and create an internal packet to represent this data
//-------------------------------------------------------------------------------------------------------
// socket 에서 읽은 data를 raknet 패킷 형태로 바꾸어 주는 함수.
// deserialize라고 생각하면 된다.
InternalPacket* ReliabilityLayer::CreateInternalPacketFromBitStream( RakNet::BitStream *bitStream, unsigned long time )
{
#ifdef _DEBUG
    int iPacketSize = bitStream->GetNumberOfBytesUsed(); 
#endif
	bool bitStreamSucceeded;
	InternalPacket* internalPacket;
	
    // bitstream에 남아있는 data size 최소한의 packet type size( packetNumber는 PacketID라고 생각하면 된다. )보다도 작다면 return.
	if ( bitStream->GetNumberOfUnreadBits() < sizeof( internalPacket->packetNumber ) * 8 )
		return 0; // leftover bits
		
	internalPacket = InternalPacketPool::Instance() ->GetPointer();
	
#ifdef _DEBUG
	// Remove boundschecker accessing undefined memory error
    memset( internalPacket, 255, sizeof( InternalPacket ) - sizeof(std::vector<DWORD>) );

	
#endif
	
	internalPacket->creationTime = time;
	
	//bitStream->AlignReadToByteBoundary();
	
	// Read the packet number (2 bytes)
	bitStreamSucceeded = bitStream->Read( internalPacket->packetNumber );
	
#ifdef _DEBUG
	
	assert( bitStreamSucceeded );
	
#endif
	
	if ( bitStreamSucceeded == false )
	{
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	// Read if this packet is an acknowledgement (1 bit)
	bitStreamSucceeded = bitStream->Read( internalPacket->isAcknowledgement );
	
#ifdef _DEBUG
	
	assert( bitStreamSucceeded );
	
#endif
	
	if ( bitStreamSucceeded == false )
	{
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	// Acknowledgement packets have no more data than the packetnumber and whether it is an acknowledgement
	if ( internalPacket->isAcknowledgement )
		return internalPacket;
		
	// Read the PacketReliability. This is encoded in 3 bits
	unsigned char reliability;
	
	bitStreamSucceeded = bitStream->ReadBits( ( unsigned char* ) ( &( reliability ) ), 3 );
	
	internalPacket->reliability = ( PacketReliability ) reliability;
	
#ifdef _DEBUG
	
	assert( bitStreamSucceeded );
	
#endif
	
	if ( bitStreamSucceeded == false )
	{
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	// If the reliability requires an ordering channel and ordering index, we read those.
	if ( internalPacket->reliability == UNRELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_SEQUENCED || internalPacket->reliability == RELIABLE_ORDERED )
	{
		// ordering channel encoded in 5 bits (from 0 to 31)
		bitStreamSucceeded = bitStream->ReadBits( ( unsigned char* ) & ( internalPacket->orderingChannel ), 5 );
#ifdef _DEBUG
		
        assert( bitStreamSucceeded );
#endif
		
		if ( bitStreamSucceeded == false )
		{
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			return 0;
		}
		
		// ordering index is one byte
		bitStreamSucceeded = bitStream->Read( internalPacket->orderingIndex );
		
#ifdef _DEBUG
		
		assert( bitStreamSucceeded );
		
#endif
		
		if ( bitStreamSucceeded == false )
		{
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			return 0;
		}
	}
	
	// Read if this is a split packet (1 bit)
	bool isSplitPacket;

	bitStreamSucceeded = bitStream->Read( isSplitPacket );
	
#ifdef _DEBUG
    //////////////////////////////////////////////////////////////////////////
    //  문제발생
    //////////////////////////////////////////////////////////////////////////
	assert( bitStreamSucceeded );
	
#endif
	
	if ( bitStreamSucceeded == false )
	{
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	if ( isSplitPacket )
	{
		// split packet indices are one byte (so one packet can be split ufp to 65535 times - maximum packet size would be about 500 * 65535)
		bitStreamSucceeded = bitStream->Read( internalPacket->splitPacketId );
#ifdef _DEBUG
        if ( bitStreamSucceeded == false )
        {
            MessageBox( NULL, "허얼 잘못 되었다(Read)", NULL, NULL );
        }
		assert( bitStreamSucceeded );
#endif
		
		if ( bitStreamSucceeded == false )
		{
            m_iCntFailRead++;
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			return 0;
		}
		
		bitStreamSucceeded = bitStream->ReadCompressed( internalPacket->splitPacketIndex );
#ifdef _DEBUG
		
		assert( bitStreamSucceeded );
#endif
		
		if ( bitStreamSucceeded == false )
		{
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			return 0;
		}
		
		bitStreamSucceeded = bitStream->ReadCompressed( internalPacket->splitPacketCount );
#ifdef _DEBUG
		
		assert( bitStreamSucceeded );
#endif
		
		if ( bitStreamSucceeded == false )
		{
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			return 0;
		}
	}
	
	else
		internalPacket->splitPacketIndex = internalPacket->splitPacketCount = 0;
		
	// Optimization - do byte alignment here
	//unsigned char zero;
	//bitStream->ReadBits(&zero, 8 - (bitStream->GetNumberOfBitsUsed() %8));
	//assert(zero==0);
	
	
	unsigned short length = 0;
	
	bitStreamSucceeded = bitStream->ReadCompressed( length );
	
	// Read into an unsigned short.  Otherwise the data would be offset too high by two bytes
#ifdef _DEBUG
    if ( bitStreamSucceeded == false )
    {
        MessageBox( NULL, "허얼 잘못 되었다(ReadCompressed)", NULL, NULL );
    }	
	assert( bitStreamSucceeded );
	
#endif
	
	if ( bitStreamSucceeded == false )
	{
        m_iCntFailReadCompressed++;
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	internalPacket->dataBitLength = length;
#ifdef _DEBUG
	
	assert( internalPacket->dataBitLength > 0 && BITS_TO_BYTES( internalPacket->dataBitLength ) < MAXIMUM_MTU_SIZE );
#endif
	
	if ( ! ( internalPacket->dataBitLength > 0 && BITS_TO_BYTES( internalPacket->dataBitLength ) < MAXIMUM_MTU_SIZE ) )
	{
		delete [] internalPacket->data;
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
	// Allocate memory to hold our data
	internalPacket->data = new char [ BITS_TO_BYTES( internalPacket->dataBitLength ) ];
	
	// Set the last byte to 0 so if ReadBits does not read a multiple of 8 the last bits are 0'ed out
	internalPacket->data[ BITS_TO_BYTES( internalPacket->dataBitLength ) - 1 ] = 0;
	
	// Read the data the packet holds
	bitStreamSucceeded = bitStream->ReadAlignedBytes( ( unsigned char* ) internalPacket->data, BITS_TO_BYTES( internalPacket->dataBitLength ) );
	//bitStreamSucceeded = bitStream->ReadBits((unsigned char*)internalPacket->data, internalPacket->dataBitLength);
#ifdef _DEBUG
    if ( bitStreamSucceeded == false )
    {
        MessageBox( NULL, "허얼 잘못 되었다(REadAlignedBytes)", NULL, NULL );
    }		
	assert( bitStreamSucceeded );
	
	if ( bitStreamSucceeded == false )
	{
        m_iCntFailReadAlignedBytes++;
		delete [] internalPacket->data;
		InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		return 0;
	}
	
#endif
	
	// PRINTING UNRELIABLE STRINGS
	// if (internalPacket->data && internalPacket->dataBitLength>5*8)
	//  printf("Received %s\n",internalPacket->data);
	
	return internalPacket;
}

//-------------------------------------------------------------------------------------------------------
// Get the SHA1 code
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::GetSHA1( unsigned char * const buffer, unsigned long
				nbytes, char code[ SHA1_LENGTH ] )
{
	CSHA1 sha1;
	
	sha1.Reset();
	sha1.Update( ( unsigned char* ) buffer, nbytes );
	sha1.Final();
	memcpy( code, sha1.GetHash(), SHA1_LENGTH );
}

//-------------------------------------------------------------------------------------------------------
// Check the SHA1 code
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::CheckSHA1( char code[ SHA1_LENGTH ], unsigned char *
				  const buffer, unsigned long nbytes )
{
	char code2[ SHA1_LENGTH ];
	GetSHA1( buffer, nbytes, code2 );
	
	for ( int i = 0; i < SHA1_LENGTH; i++ )
		if ( code[ i ] != code2[ i ] )
			return false;
			
	return true;
}

//-------------------------------------------------------------------------------------------------------
// Search the specified list for sequenced packets on the specified ordering
// stream, optionally skipping those with splitPacketId, and delete them
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::DeleteSequencedPacketsInList( unsigned char orderingChannel, BasicDataStructures::List<InternalPacket*>&theList, int splitPacketId )
{
	unsigned i = 0;
	
	while ( i < theList.size() )
	{
		if ( ( theList[ i ] ->reliability == RELIABLE_SEQUENCED || theList[ i ] ->reliability == UNRELIABLE_SEQUENCED ) &&
			theList[ i ] ->orderingChannel == orderingChannel && ( splitPacketId == -1 || theList[ i ] ->splitPacketId != splitPacketId ) )
		{
			InternalPacket * internalPacket = theList[ i ];
			theList.del( i );
			delete [] internalPacket->data;
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
		}
		
		else
			i++;
	}
}

//-------------------------------------------------------------------------------------------------------
// Search the specified list for sequenced packets with a value less than orderingIndex and delete them
// Note - I added functionality so you can use the Queue as a list (in this case for searching) but it is less efficient to do so than a regular list
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::DeleteSequencedPacketsInList( unsigned char orderingChannel, BasicDataStructures::Queue<InternalPacket*>&theList )
{
	InternalPacket * internalPacket;
	int listSize = theList.size();
	int i = 0;
	
	while ( i < listSize )
	{
		if ( ( theList[ i ] ->reliability == RELIABLE_SEQUENCED || theList[ i ] ->reliability == UNRELIABLE_SEQUENCED ) && theList[ i ] ->orderingChannel == orderingChannel )
		{
			internalPacket = theList[ i ];
			theList.del( i );
			delete [] internalPacket->data;
			InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
			listSize--;
		}
		
		else
			i++;
	}
}

//-------------------------------------------------------------------------------------------------------
// Returns true if newPacketOrderingIndex is older than the waitingForPacketOrderingIndex
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::IsOlderOrderedPacket( unsigned char newPacketOrderingIndex, unsigned char waitingForPacketOrderingIndex )
{
	// Any number sequence within the last 128, given a max of 256, is bad
	
	if ( waitingForPacketOrderingIndex > 127 )
	{
		if ( newPacketOrderingIndex >= waitingForPacketOrderingIndex - 128 && newPacketOrderingIndex < waitingForPacketOrderingIndex )
		{
			// Within the last 128
			return true;
		}
	}
	
	else
		if ( newPacketOrderingIndex >= ( unsigned char ) ( waitingForPacketOrderingIndex - ( unsigned char ) 128 ) ||
			newPacketOrderingIndex < waitingForPacketOrderingIndex )
		{
			// Within the last 128
			return true;
		}
		
	// Old packet
	return false;
}

//-------------------------------------------------------------------------------------------------------
// Split the passed packet into chunks under MTU_SIZEbytes (including headers) and save those new chunks
// Optimized version
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SplitPacketAndDeleteOriginal( InternalPacket *internalPacket, int MTUSize, std::vector<DWORD> vecUID_ )
{
	// Doing all sizes in bytes in this function so I don't write partial bytes with split packets
	internalPacket->splitPacketCount = 1; // This causes GetBitStreamHeaderLength to account for the split packet header
	int headerLength = BITS_TO_BYTES( GetBitStreamHeaderLength( internalPacket ) );
	int dataByteLength = BITS_TO_BYTES( internalPacket->dataBitLength );
	int maxDataSize;
	int maximumSendBlock, byteOffset, bytesToSend;
	unsigned short splitPacketIndex;
	int i;
	InternalPacket **internalPacketArray;
	
	maxDataSize = MTUSize - UDP_HEADER_SIZE;
	
	if ( encryptor.IsKeySet() )
		maxDataSize -= 16; // Extra data for the encryptor
		
#ifdef _DEBUG
	// Make sure we need to split the packet to begin with
	assert( dataByteLength > maxDataSize - headerLength );
	
	// If this assert is hit the packet is so tremendous we need to widen the split packet types.  You should never send something that big anyway
	assert( ( dataByteLength - 1 ) / ( maxDataSize - headerLength ) + 1 < 65535 );
	
#endif
	
	// How much to send in the largest block
	maximumSendBlock = maxDataSize - headerLength;
	
	// Calculate how many packets we need to create
	internalPacket->splitPacketCount = ( unsigned short ) ( ( dataByteLength - 1 ) / ( maximumSendBlock ) + 1 );
	
	statistics.totalSplits += internalPacket->splitPacketCount;
	
	// Optimization
	// internalPacketArray = new InternalPacket*[internalPacket->splitPacketCount];
	internalPacketArray = ( InternalPacket** ) alloca( sizeof( InternalPacket* ) * internalPacket->splitPacketCount );
	
	for ( i = 0; i < ( int ) internalPacket->splitPacketCount; i++ )
	{
		internalPacketArray[ i ] = InternalPacketPool::Instance() ->GetPointer();
        memcpy( internalPacketArray[ i ], internalPacket, sizeof( InternalPacket ) - sizeof( std::vector<DWORD>));
	}
	
	// This identifies which packet this is in the set
	splitPacketIndex = 0;
	
	// Do a loop to send out all the packets
	do
	{
		byteOffset = splitPacketIndex * maximumSendBlock;
		bytesToSend = dataByteLength - byteOffset;
		
		if ( bytesToSend > maximumSendBlock )
			bytesToSend = maximumSendBlock;
			
		// Copy over our chunk of data
		internalPacketArray[ splitPacketIndex ] ->data = new char[ bytesToSend ];
		
		memcpy( internalPacketArray[ splitPacketIndex ] ->data, internalPacket->data + byteOffset, bytesToSend );
		
		if ( bytesToSend != maximumSendBlock )
			internalPacketArray[ splitPacketIndex ] ->dataBitLength = internalPacket->dataBitLength - splitPacketIndex * ( maximumSendBlock << 3 );
		else
			internalPacketArray[ splitPacketIndex ] ->dataBitLength = bytesToSend << 3;
			
		internalPacketArray[ splitPacketIndex ] ->splitPacketIndex = splitPacketIndex;
		
		internalPacketArray[ splitPacketIndex ] ->splitPacketId = splitPacketId;
		
		internalPacketArray[ splitPacketIndex ] ->splitPacketCount = internalPacket->splitPacketCount;
		
		if ( splitPacketIndex > 0 )   // For the first split packet index we keep the packetNumber already assigned
		{
			// For every further packet we use a new packetNumber.
			// Note that all split packets are reliable
			reliabilityLayerMutexes[ packetNumber_MUTEX ].Lock();
			internalPacketArray[ splitPacketIndex ] ->packetNumber = packetNumber;
			
			if ( ++packetNumber == RECEIVED_PACKET_LOG_LENGTH )
				packetNumber = 0;
				
			reliabilityLayerMutexes[ packetNumber_MUTEX ].Unlock();
		}
		
		// Add the new packet to send list at the correct priority
		//  reliabilityLayerMutexes[sendQueue_MUTEX].Lock();
		//  sendQueue[internalPacket->priority].insert(newInternalPacket);
		//  reliabilityLayerMutexes[sendQueue_MUTEX].Unlock();
		// SHOW SPLIT PACKET GENERATION
		// if (splitPacketIndex % 100 == 0)
		//  printf("splitPacketIndex=%i\n",splitPacketIndex);
		//} while(++splitPacketIndex < internalPacket->splitPacketCount);
	}
	
	while ( ++splitPacketIndex < internalPacket->splitPacketCount );
	
	splitPacketId++; // It's ok if this wraps to 0
	
	// Copy all the new packets into the split packet list
	reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + internalPacket->priority ].Lock();
	//====================================================================================
    // 2006.07.06 : Asirion
    // sendQueue에 InternalPacket을 만들어 Push함으로
    // UID vector를 채워서 넣어줘야 한다.
	for ( i = 0; i < ( int ) internalPacket->splitPacketCount; i++ )
    {
        if ( vecUID_.size() != 0 )
            internalPacketArray[ i ]->vecRelayUID = vecUID_;
        else
            internalPacketArray[ i ]->vecRelayUID.clear();
		sendQueue[ internalPacket->priority ].push( internalPacketArray[ i ] );
    }	
	reliabilityLayerMutexes[ sendQueueSystemPriority_MUTEX + internalPacket->priority ].Unlock();
	
	// Delete the original
	delete [] internalPacket->data;
	
	InternalPacketPool::Instance() ->ReleasePointer( internalPacket );
	
	//delete [] internalPacketArray;
}

//-------------------------------------------------------------------------------------------------------
// Insert a packet into the split packet list
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::InsertIntoSplitPacketList( InternalPacket * internalPacket )
{
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
	splitPacketList.insert( internalPacket );
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
}

//-------------------------------------------------------------------------------------------------------
// Take all split chunks with the specified splitPacketId and try to
//reconstruct a packet.  If we can, allocate and return it.  Otherwise return 0
// Optimized version
//-------------------------------------------------------------------------------------------------------
InternalPacket * ReliabilityLayer::BuildPacketFromSplitPacketList( unsigned long splitPacketId, unsigned long time )
{
	int i, j, size;
	// How much data all blocks but the last hold
	int maxDataSize;
	int numParts;
	int bitlength;
	int *indexList;
	int indexListIndex;
	
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Lock();
	size = splitPacketList.size();
	
	for ( i = 0; i < size; i++ )
	{
		if ( splitPacketList[ i ] ->splitPacketId == splitPacketId )
		{
			// Is there enough elements in the list to have all the parts?
			
			if ( splitPacketList[ i ] ->splitPacketCount > splitPacketList.size() - i )
			{
				//   if (splitPacketList.size() % 100 == 0 || splitPacketList[i]->splitPacketCount-splitPacketList.size()<100)
				//    printf("%i out of %i\n", splitPacketList.size(), splitPacketList[i]->splitPacketCount);
				//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
				return 0;
			}
			
			//  printf("%i out of %i\n", splitPacketList.size(), splitPacketList[i]->splitPacketCount);
			// Keep track of the indices of the elements through our first scan so we don't have to rescan to find them
			indexListIndex = 0;
			
			numParts = 1;
			
			bitlength = splitPacketList[ i ] ->dataBitLength;
			
			// indexList = new int[splitPacketList[i]->splitPacketCount];
			indexList = ( int* ) alloca( sizeof( int ) * splitPacketList[ i ] ->splitPacketCount );
			
			indexList[ indexListIndex++ ] = i;
			
			maxDataSize = BITS_TO_BYTES( splitPacketList[ i ] ->dataBitLength );
			
			// Are all the parts there?
			for ( j = i + 1; j < size; j++ )
			{
				if ( splitPacketList[ j ] ->splitPacketId == splitPacketId )
				{
					indexList[ indexListIndex++ ] = j;
					numParts++;
					bitlength += splitPacketList[ j ] ->dataBitLength;
					
					if ( ( int ) BITS_TO_BYTES( splitPacketList[ j ] ->dataBitLength ) > maxDataSize )
						maxDataSize = BITS_TO_BYTES( splitPacketList[ j ] ->dataBitLength );
				}
			}
			
			if ( numParts == splitPacketList[ i ] ->splitPacketCount )
			{
				// All the parts are here
				InternalPacket * internalPacket = CreateInternalPacketCopy( splitPacketList[ i ], 0, 0, time );
				internalPacket->data = new char[ BITS_TO_BYTES( bitlength ) ];
#ifdef _DEBUG
				
				internalPacket->splitPacketCount = splitPacketList[ i ] ->splitPacketCount;
#endif
				
				// Add each part to internalPacket
				j = 0;
				
				while ( j < indexListIndex )
				{
					if ( splitPacketList[ indexList[ j ] ] ->splitPacketCount - 1 == splitPacketList[ indexList[ j ] ] ->splitPacketIndex )
					{
						// Last split packet
						// If this assert fails,
						// then the total bit length calculated by adding the last block to the maximum block size * the number of blocks that are not the last block
						// doesn't match the amount calculated from traversing the list
#ifdef _DEBUG
						assert( BITS_TO_BYTES( splitPacketList[ indexList[ j ] ] ->dataBitLength ) + splitPacketList[ indexList[ j ] ] ->splitPacketIndex * maxDataSize == ( bitlength - 1 ) / 8 + 1 );
#endif
						
						memcpy( internalPacket->data + splitPacketList[ indexList[ j ] ] ->splitPacketIndex * maxDataSize, splitPacketList[ indexList[ j ] ] ->data, BITS_TO_BYTES( splitPacketList[ indexList[ j ] ] ->dataBitLength ) );
					}
					
					else
					{
						// Not last split packet
						memcpy( internalPacket->data + splitPacketList[ indexList[ j ] ] ->splitPacketIndex * maxDataSize, splitPacketList[ indexList[ j ] ] ->data, maxDataSize );
					}
					
					internalPacket->dataBitLength += splitPacketList[ indexList[ j ] ] ->dataBitLength;
					InternalPacket *temp;
					
					temp = splitPacketList[ indexList[ j ] ];
					delete [] temp->data;
					InternalPacketPool::Instance() ->ReleasePointer( temp );
					splitPacketList[ indexList[ j ] ] = 0;
					
#ifdef _DEBUG
					
					numParts--;
#endif
					
					j++;
					size--;
				}
				
#ifdef _DEBUG
				assert( numParts == 0 ); // Make sure the correct # of elements was removed from the list
				
#endif
				
				j = 0;
				
				while ( ( unsigned ) j < splitPacketList.size() )
					if ( splitPacketList[ j ] == 0 )
					{
						// Since order doesn't matter, swap from the tail to the current element.
						splitPacketList[ j ] = splitPacketList[ splitPacketList.size() - 1 ];
						splitPacketList[ splitPacketList.size() - 1 ] = 0;
						// Then just delete the tail (just changes a counter)
						splitPacketList.del( splitPacketList.size() - 1 );
					}
					
					else
						j++;
						
				//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
				
				// delete [] indexList;
				
				return internalPacket;
			}
			
			// delete [] indexList;
			break;
		}
	}
	
	//reliabilityLayerMutexes[splitPacketList_MUTEX].Unlock();
	
	return 0;
}

// Delete any unreliable split packets that have long since expired
void ReliabilityLayer::DeleteOldUnreliableSplitPackets( unsigned long time )
{
	unsigned size, i, orderingIndexToDelete;
	unsigned long newestUnreliableSplitPacket;
	bool found;
	InternalPacket *temp;
	
	// Scan through the list for split packets that were sent unreliably.
	// If the newest unreliable split packet for a particular ID is more than 3000 ms old, then
	// delete all of them of that id
	
	size = splitPacketList.size();
	newestUnreliableSplitPacket = 0L;
	found = false;
	
	for ( i = 0; i < size; i++ )
	{
		if ( ( splitPacketList[ i ] ->reliability == UNRELIABLE || splitPacketList[ i ] ->reliability == UNRELIABLE_SEQUENCED ) &&
			splitPacketList[ i ] ->creationTime >= newestUnreliableSplitPacket )
		{
			orderingIndexToDelete = splitPacketList[ i ] ->orderingIndex;
			newestUnreliableSplitPacket = splitPacketList[ i ] ->creationTime;
			found = true;
		}
	}
	
	if ( found && time - newestUnreliableSplitPacket > 5000 )
	{
		// Delete all split packets that use orderingIndexToDelete
		i = 0;
		
		while ( i < splitPacketList.size() )
		{
			if ( splitPacketList[ i ] ->orderingIndex == orderingIndexToDelete )
			{
				temp = splitPacketList[ i ];
				splitPacketList[ i ] = splitPacketList[ splitPacketList.size() - 1 ];
				splitPacketList.del(); // Removes the last element
				InternalPacketPool::Instance() ->ReleasePointer( temp );
			}
			
			else
				i++;
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// Creates a copy of the specified internal packet with data copied from the original starting at dataByteOffset for dataByteLength bytes.
// Does not copy any split data parameters as that information is always generated does not have any reason to be copied
//-------------------------------------------------------------------------------------------------------
InternalPacket * ReliabilityLayer::CreateInternalPacketCopy( InternalPacket *original, int dataByteOffset, int dataByteLength, unsigned long time )
{
	InternalPacket * copy = InternalPacketPool::Instance() ->GetPointer();
#ifdef _DEBUG
	// Remove boundschecker accessing undefined memory error
    memset( copy, 255, sizeof( InternalPacket ) - sizeof(std::vector<DWORD>) );

#endif
	// Copy over our chunk of data
	
	if ( dataByteLength > 0 )
	{
		copy->data = new char[ dataByteLength ];
		memcpy( copy->data, original->data + dataByteOffset, dataByteLength );
	}
	
	else
		copy->data = 0;
		
	copy->dataBitLength = dataByteLength << 3;
	
	copy->creationTime = time;
	
	copy->isAcknowledgement = original->isAcknowledgement;
	
	copy->nextActionTime = 0;
	
	copy->orderingIndex = original->orderingIndex;
	
	copy->orderingChannel = original->orderingChannel;
	
	copy->packetNumber = original->packetNumber;
	
	copy->priority = original->priority;
	
	copy->reliability = original->reliability;

    copy->vecRelayUID = original->vecRelayUID;

    copy->resendCount = original->resendCount;

	return copy;
}

//-------------------------------------------------------------------------------------------------------
// Get the specified ordering list
// LOCK THIS WHOLE BLOCK WITH reliabilityLayerMutexes[orderingList_MUTEX].Unlock();
//-------------------------------------------------------------------------------------------------------
BasicDataStructures::LinkedList<InternalPacket*> *ReliabilityLayer::GetOrderingListAtOrderingStream( unsigned char orderingChannel )
{
	if ( orderingChannel >= orderingList.size() )
		return 0;
		
	return orderingList[ orderingChannel ];
}

//-------------------------------------------------------------------------------------------------------
// Add the internal packet to the ordering list in order based on order index
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::AddToOrderingList( InternalPacket * internalPacket )
{
#ifdef _DEBUG
	assert( internalPacket->orderingChannel < NUMBER_OF_ORDERED_STREAMS );
#endif
	
	if ( internalPacket->orderingChannel >= NUMBER_OF_ORDERED_STREAMS )
		return ;
		
	//reliabilityLayerMutexes[orderingList_MUTEX].Lock();
	if ( internalPacket->orderingChannel >= orderingList.size() || orderingList[ internalPacket->orderingChannel ] == 0 )
	{
		// Need a linked list in this index
		orderingList.replace( new BasicDataStructures::LinkedList<InternalPacket*>, 0, internalPacket->orderingChannel );
		
		( orderingList[ internalPacket->orderingChannel ] ) ->add ( internalPacket )
		
		;
		//reliabilityLayerMutexes[orderingList_MUTEX].Unlock();
		return ;
	}
	
	else
	{
		// Have a linked list in this index
		
		if ( orderingList[ internalPacket->orderingChannel ] ->size() == 0 )
		{
			// The linked list is empty
			
			( orderingList[ internalPacket->orderingChannel ] ) ->add ( internalPacket )
			
			;
		}
		
		else
		{
			BasicDataStructures::LinkedList<InternalPacket*> *theList;
			theList = GetOrderingListAtOrderingStream( internalPacket->orderingChannel );
			// Add this packet in no particular order
			theList->beginning();
			theList->insert( internalPacket );
		}
		
		//reliabilityLayerMutexes[orderingList_MUTEX].Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------
// Inserts a packet into the resend list in order
// THIS WHOLE FUNCTION SHOULD BE LOCKED WITH
// reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::InsertPacketIntoResendQueue( InternalPacket *internalPacket, unsigned long time )
{
	//reliabilityLayerMutexes[lastAckTime_MUTEX].Lock();
	
	if ( lastAckTime == 0 || resendQueue.size() == 0 )
		lastAckTime = time; // Start the timer for the ack of this packet if we aren't already waiting for an ack
		
	//reliabilityLayerMutexes[lastAckTime_MUTEX].Unlock();
	
	//reliabilityLayerMutexes[resendQueue_MUTEX].Lock();
	resendQueue.push( internalPacket );
	
	//reliabilityLayerMutexes[resendQueue_MUTEX].Unlock();
}

//-------------------------------------------------------------------------------------------------------
// If Read returns -1 and this returns true then a modified packet was detected
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::IsCheater( void ) const
{
	return cheater;
}

//-------------------------------------------------------------------------------------------------------
//  Were you ever unable to deliver a packet despite retries?
//-------------------------------------------------------------------------------------------------------
bool ReliabilityLayer::IsDeadConnection( void ) const
{
	return deadConnection;
}

//-------------------------------------------------------------------------------------------------------
// How long to wait between packet resends
//-------------------------------------------------------------------------------------------------------
void ReliabilityLayer::SetLostPacketResendDelay( unsigned long i )
{
	if ( i > 0 )
		lostPacketResendDelay = i;
		
	if ( lostPacketResendDelay < 150 )   // To avoid unnecessary packetloss, this value should be UPDATE_THREAD_UPDATE_TIME + UPDATE_THREAD_POLL_TIME at a minimum
		lostPacketResendDelay = 150;
}

//-------------------------------------------------------------------------------------------------------
// Statistics
//-------------------------------------------------------------------------------------------------------
RakNetStatisticsStruct * const ReliabilityLayer::GetStatistics( void )
{
	int i;
	
	for ( i = 0; i < NUMBER_OF_PRIORITIES; i++ )
	{
		statistics.messageSendBuffer[ i ] = sendQueue[ i ].size();
	}
	
	statistics.acknowlegementsPending = acknowledgementQueue.size();
	statistics.messagesWaitingForReassembly = splitPacketList.size();
	statistics.internalOutputQueueSize = outputQueue.size();
	statistics.windowSize = windowSize;
	statistics.lossySize = lossyWindowSize == MAXIMUM_WINDOW_SIZE + 1 ? 0 : lossyWindowSize;
	statistics.messagesOnResendQueue = resendQueue.size();
	
	return &statistics;
}