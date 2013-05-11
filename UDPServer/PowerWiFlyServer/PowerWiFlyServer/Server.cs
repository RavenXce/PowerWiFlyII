using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;
using System.Windows.Forms;

namespace PowerWiFlyServer
{
    class Server
    {
        private volatile bool _shouldStop;  //used to stop thread
        private bool serverStatus;          //server status
        private byte[] data;                //data buffer
        private IPEndPoint ipep;            //receiving information
        private UdpClient newsock;          //UDP socket
        private IPEndPoint sender;          //sender information
        
        public Server(int port)
        {
            serverStatus = false;
            data = new byte[1024];
            ipep = new IPEndPoint(IPAddress.Any, port);
            sender = new IPEndPoint(IPAddress.Any, 0);
        }

        public void SetPort(int port)
        {
            ipep = new IPEndPoint(IPAddress.Any, port);
        }

        public bool GetStatus()
        {
            return serverStatus;
        }

        public void StartServer()
        {
            serverStatus = true;
            Debug.WriteLine("Listening now..");
            //TinyAlertView.Show(TinyAlertView.StateTinyAlert.SUCCESS, "Listening Now");
            newsock = new UdpClient(ipep);
            while(serverStatus)
                ListeAndRespond();
        }

        public void StopServer()
        {
            newsock.Close();
            serverStatus = false;
            _shouldStop = true;
            Debug.WriteLine("Stopped");
            //TinyAlertView.Show(TinyAlertView.StateTinyAlert.FAILURE, "Stopped");
        }

        public int ListeAndRespond()
        {
            try
            {
                data = newsock.Receive(ref sender);
                Console.WriteLine("Message received from {0}:", sender.ToString());
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.WARNING, String.Format("Message received from {0}:", sender.ToString()));

                Console.WriteLine(Encoding.ASCII.GetString(data, 0, data.Length));
                MessageBox.Show(Encoding.ASCII.GetString(data, 0, data.Length));
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.WARNING, Encoding.ASCII.GetString(data, 0, data.Length));

                newsock.Send(data, data.Length, sender);
                string welcome = "Welcome to my test server";
                data = Encoding.ASCII.GetBytes(welcome);
                newsock.Send(data, data.Length, sender);
            }  
            catch (SocketException socketEx)
            {
                //newsock.Close();
            }

            return 0;
        }

    }
}
