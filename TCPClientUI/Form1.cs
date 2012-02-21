using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace TCPClientUI
{
    public partial class Form1 : Form
    {
        static TcpClient client;
        IPEndPoint serverEndPoint;
        NetworkStream clientStream;
        static String textFromServer;

        public Form1()
        {
            InitializeComponent();
            iniTCP();
        }

        public void iniTCP()
        {
            client = new TcpClient();
            serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 3000);


        }

        public void connectTCP()
        {
            client.Connect(serverEndPoint);
            clientStream = client.GetStream();

            // Create a thread to handle communication 
            // with connected client
            Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
            clientThread.Start(client);

        }

        public void sendMsg()
        {
            ASCIIEncoding encoder = new ASCIIEncoding();

            byte[] buffer = encoder.GetBytes("Testing TCP/IP connection");
            clientStream = client.GetStream();
            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
            //clientStream.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            connectTCP();
        }

        private void buttonSendMessage_Click(object sender, EventArgs e)
        {
            sendMsg();
        }

        /// <summary>
        /// Handles client connections
        /// </summary>
        /// <param name="client"></param>
        private static void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;
            NetworkStream clientStream = tcpClient.GetStream();

            byte[] message = new byte[4096];
            int bytesRead;

            do
            {
                bytesRead = 0;

                try
                {
                    // Blocks until a client sends a message                    
                    bytesRead = clientStream.Read(message, 0, 4096);
                }
                catch (Exception)
                {
                    // A socket error has occured
                    break;
                }

                if (bytesRead == 0)
                {
                    // The client has disconnected from the server
                    break;
                }

                // Message has successfully been received
                ASCIIEncoding encoder = new ASCIIEncoding();

                // Output message
                //textFromServer = tcpClient.Client.LocalEndPoint + " " + tcpClient.Client.RemoteEndPoint + encoder.GetString(message, 0, bytesRead);
                textFromServer = tcpClient.Client.LocalEndPoint + " " + tcpClient.Client.RemoteEndPoint + encoder.GetString(message, 0, bytesRead);

            } while (clientStream.DataAvailable);


        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            // Release connections
            clientStream.Close();
            //tcpClient.Close();
        }
    }
}
