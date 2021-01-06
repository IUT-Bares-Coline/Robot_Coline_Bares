#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    // Fonction prenant entree la trame et sa longueur pour calculerle checksum

    //c# :
    char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);


    //for(int i=0; i<sizeof(msgPayload)); i++)      <-- beautiful [petit smiley papillon]
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;

}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    // Fonction d?encodage et d?envoi d?un message

    unsigned char message[6 + msgPayloadLength];
    int pos = 0;
    message[pos++] = 0xFE;
    message[pos++] = (unsigned char) (msgFunction >> 8);
    message[pos++] = (unsigned char) (msgFunction >> 0);
    message[pos++] = (unsigned char) (msgPayloadLength >> 8);
    message[pos++] = (unsigned char) (msgPayloadLength >> 0);

    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        message[pos++] = msgPayload[i];
    }

    message[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

    SendMessage(message, msgPayloadLength + 6);
    //serialPort1.Write(message, 0, pos);
}


#define MAX_MSG_PAYLOAD_LENGTH 128

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[MAX_MSG_PAYLOAD_LENGTH];
int msgDecodedPayloadIndex = 0;
StateReception rcvState;

void UartDecodeMessage(unsigned char c) {
    // Fonction prenant en entree un octet et servant a reconstituer les trames

    switch (rcvState) {
        case Waiting:

            if (c == 0xFE) rcvState = FunctionMSB;
            break;

        case FunctionMSB:

            msgDecodedFunction = (short) (c << 8);
            rcvState = FunctionLSB;
            break;

        case FunctionLSB:

            msgDecodedFunction += c;
            rcvState = PayloadLengthMSB;
            break;

        case PayloadLengthMSB:

            msgDecodedPayloadLength = (short) (c << 8);
            rcvState = PayloadLengthLSB;
            break;

        case PayloadLengthLSB:

            msgDecodedPayloadLength += c;
            if (msgDecodedPayloadLength == 0) {
                rcvState = CheckSum;
            } else if (msgDecodedPayloadLength < MAX_MSG_PAYLOAD_LENGTH) {
                rcvState = Payload;
                msgDecodedPayloadIndex = 0;
            } else {
                rcvState = Waiting;
            }
            break;

        case Payload:
            msgDecodedPayload[msgDecodedPayloadIndex++] = c;
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                rcvState = CheckSum;
            break;

        case CheckSum:
        {
            unsigned char calculatedChecksum = UartCalculateChecksum((short) msgDecodedFunction, (short) msgDecodedPayloadLength, msgDecodedPayload);
            unsigned char receivedChecksum = c;

            if (calculatedChecksum == receivedChecksum) {
                // Success , on a un message valide
                ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                //robot.MessageQueue.Enqueue(new Message(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload));

            } else {
                //printf("Erreur de décodage de message");  //????
            }
            rcvState = Waiting;
        }

            break;

        default:
            rcvState = Waiting;
            break;
    }
}
/*
void UartProcessDecodedMessage (unsigned char function , unsigned char payloadLength , unsigned char payload [ ] )
{
// Fonction éappele après le décodage pour excuter l?action
// c o r r e s p o n d a n t au message çreu
switch(msgFunction)
{
case SET_ROBOT_STATE:
SetRobotState( msgPayload [ 0 ] ) ;
break ;
case SET_ROBOT_MANUAL_CONTROL:
SetRobotAutoControlState ( msgPayload [ 0 ] ) ;
break ;
default :
break ;
}
}
*/

