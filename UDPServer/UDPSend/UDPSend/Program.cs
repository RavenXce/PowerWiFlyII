using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;


namespace UDPSend
{
    class Program
    {
        static void Main(string[] args)
        {
            UdpClient udpClient = new UdpClient(5500);
            //udpClient.Connect("raaj.homeip.net", 2005);
            udpClient.Connect("192.168.1.8", 2005);
            Byte[] sendBytes = Encoding.ASCII.GetBytes("hello?");
            udpClient.Send(sendBytes, sendBytes.Length);

            
            //IPEndPoint object will allow us to read datagrams sent from any source.
            IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);

            // Blocks until a message returns on this socket from a remote host.
            Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
            string returnData = Encoding.ASCII.GetString(receiveBytes);

            // Uses the IPEndPoint object to determine which of these two hosts responded.
            Console.WriteLine("This is the message you received " +
                                         returnData.ToString());
            Console.WriteLine("This message was sent from " +
                                     RemoteIpEndPoint.Address.ToString() +
                                     " on their port number " +
                                     RemoteIpEndPoint.Port.ToString());

            udpClient.Close();
            Console.ReadLine();
             
        }
    }
}
