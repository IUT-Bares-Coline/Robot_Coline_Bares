/* 
 * File:   UART_Protocol.h
 * Author: GEII Robot
 *
 * Created on 20 novembre 2020, 12:10
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength , unsigned char* payload);

#endif	/* UART_PROTOCOL_H */

