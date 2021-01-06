/* 
 * File:   UART_Protocol.h
 * Author: GEII Robot
 *
 * Created on 20 novembre 2020, 12:10
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H


typedef enum{
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
} StateReception;

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void ProcessDecodedMessage(short function, short payloadLength, unsigned char* payload);

#endif	/* UART_PROTOCOL_H */

