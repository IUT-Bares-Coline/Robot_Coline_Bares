#include <xc.h>
#include "UART_Protocol.h"


unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
// Fonction prenant entree la trame et sa longueur pour calculerle checksum
/*
 c# :
         private byte CalculateChecksum(ushort msgFunction, ushort msgPayloadLength, byte[] msgPayload)
        {

            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);

            foreach (byte b in msgPayload)
            {
                checksum ^= b;
            }
            return checksum;
        }
 */
}


void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
// Fonction d?encodage et d?envoi d?un message
/*
 c# :
 public void UartEncodeAndSendMessage(ushort msgFunction, ushort msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[6 + msgPayloadLength];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }

            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

            serialPort1.Write(message, 0, pos);
        }
 */
    
}


int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload [ 1 2 8 ];
int msgDecodedPayloadIndex = 0 ;

/*
void UartDecodeMessage(unsigned char c )
{
// F onc ti on p ren an t en e n t r e e un o c t e t e t s e r v a n t a r e c o n s t i t u e r l e s trames
. . .
}


void UartProcessDecodedMessage ( int function, int payloadLength , unsigned char* payload)
{
// F onc ti on a p p el e e a p r e s l e decodage pour e x e c u t e r l ? a c ti o n
// c o r r e s p o n d a n t au message r e c u
. . .
}
*/

// ????????????????????????????????????????????????????????????????????????? /
// F o n c ti o n s c o r r e s p o n d a n t aux me s sage s
// ????????????????????????????????????????????????????????????????????????? /
