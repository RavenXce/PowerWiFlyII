using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.ComponentModel;
using System.Net.Sockets;
using System.Diagnostics;
using System.Net;

namespace PowerWiFlyServer
{
    class ServerThread
    {
        BackgroundWorker bw = new BackgroundWorker();
        private bool serverStatus;          //server status
        private byte[] data;                //data buffer
        private IPEndPoint ipep;            //receiving information
        private UdpClient newsock;          //UDP socket
        private IPEndPoint sender;          //sender information

        public ServerThread(int port)
        {
            bw.WorkerSupportsCancellation = true;
            bw.WorkerReportsProgress = true;
            bw.DoWork +=new DoWorkEventHandler(bw_DoWork);
            bw.ProgressChanged +=new ProgressChangedEventHandler(bw_ProgressChanged);
            bw.RunWorkerCompleted +=new RunWorkerCompletedEventHandler(bw_RunWorkerCompleted);

            serverStatus = false;
            data = new byte[1024];
            ipep = new IPEndPoint(IPAddress.Any, port);
            sender = new IPEndPoint(IPAddress.Any, 0);
        }

        ///////////////////////////////////////////////
        private void bw_DoWork(object senderx, DoWorkEventArgs e)
        {
            BackgroundWorker worker = senderx as BackgroundWorker;
            worker.ReportProgress(3, "Server started");
            while(true)
            {
                Debug.WriteLine("run");
                if ((worker.CancellationPending == true))
                {
                    Debug.WriteLine("cancel pending");
                    e.Cancel = true;
                    newsock.Close();
                    serverStatus = false;
                    break;
                }
                else
                {
                    try
                    {
                        
                        Debug.WriteLine("waiting for reponse..");
                        data = newsock.Receive(ref sender);
                        Console.WriteLine("Message received from {0}:", sender.ToString());
                        Console.WriteLine(Encoding.ASCII.GetString(data, 0, data.Length));
                        worker.ReportProgress(0,"data received");
                        worker.ReportProgress(1, Encoding.ASCII.GetString(data, 0, data.Length));

                        newsock.Send(data, data.Length, sender);
                        string welcome = "Welcome to my test server";
                        data = Encoding.ASCII.GetBytes(welcome);
                        newsock.Send(data, data.Length, sender);
                    }
                    catch (SocketException socketEx)
                    {
                        //newsock.Close();
                    }
                    
                }
            }
        }

        private void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //this.tbProgress.Text = (e.ProgressPercentage.ToString() + "%");
            //string x = e.ProgressPercentage.ToString() + "%";
            if (e.ProgressPercentage == 0)
            {
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.WARNING, e.UserState as String);
            }
            else if (e.ProgressPercentage == 1)
            {
                EventHandlers.UpdateOutputBox(e.UserState as String);
            }
            else if (e.ProgressPercentage == 3)
            {
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.SUCCESS, e.UserState as String);
                EventHandlers.UpdateButtonState(2);
            }
            
        }

        private void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if ((e.Cancelled == true))
            {
                //this.tbProgress.Text = "Canceled!";
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.FAILURE, "Server Stopped");
                EventHandlers.UpdateButtonState(1);
            }

            else if (!(e.Error == null))
            {
                //this.tbProgress.Text = ("Error: " + e.Error.Message);
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.SUCCESS, "Error");
            }

            else
            {
                //this.tbProgress.Text = "Done!";
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.SUCCESS, "Done");
            }
        }
        ///////////////////////////////////////////////

        public int ListeAndRespond()
        {
            return 0;
        }

        public void SetPort(int port) 
        {
            ipep = new IPEndPoint(IPAddress.Any, port);
        }

        public void StartServer()
        {
            serverStatus = true;
            Debug.WriteLine("Listening now..");
            newsock = new UdpClient(ipep);
            //newsock.Client.ReceiveTimeout = 5;
            newsock.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 5000);

            if (bw.IsBusy != true)
            {
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.WARNING, "Starting..");
                EventHandlers.UpdateButtonState(0);
                bw.RunWorkerAsync();
            }

        }

        public void StopServer()
        {
            //If server has started, stop it

            if (bw.WorkerSupportsCancellation == true)
            {
                TinyAlertView.Show(TinyAlertView.StateTinyAlert.WARNING, "Stopping..");
                Debug.WriteLine("attempting cancel");
                EventHandlers.UpdateButtonState(0);
                bw.CancelAsync();
            }
            
        }

        public bool GetStatus()
        {
            return serverStatus;
        }


    }
}
