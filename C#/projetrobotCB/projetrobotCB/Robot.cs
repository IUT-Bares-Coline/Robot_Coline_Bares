using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



namespace projetrobotCB
{
    class Robot
    {
        public string receivedText = "";
        public ConcurrentQueue<Message> MessageQueue = new ConcurrentQueue<Message>();
        public float positionXOdo ;
    }

    //class message 
    //queue differente --> changer code
    class Message
    {
        public UInt16 PayloadLength;
        public UInt16 Function;
        public byte[] Payload;

        public Message(UInt16 function, UInt16 payloadLength, byte[] payload)
        {
            PayloadLength = payloadLength;
            Function = function;
            Payload = payload;
        }
    }
}
