/* 
 * File:   UART_Protocol.h
 * Author: GEII Robot
 *
 * Created on 20 novembre 2020, 12:10
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H


#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052

/*
typedef enum{
    SET_ROBOT_STATE,
    SET_ROBOT_MANUAL_CONTROL
}SetRobot;
 */

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
void ProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload);
int GetDecodedFunction(void);
#endif	/* UART_PROTOCOL_H */

//void SetRobotState(int controle);
//void SetRobotAutoControlState(int controle);