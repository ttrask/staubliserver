using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace TCPServerUI
{
    public partial class Form1 : Form
    {
        private static TcpListener tcpListener;
        private static Thread listenThread;
        static NetworkStream clientStream;
        static string textFromClient = "";
        //private string endl = "\r\n";

        public delegate void MessageReceivedHandler(string message);
        public event MessageReceivedHandler MessageReceived;

        public Form1()
        {
            InitializeComponent();
            initiateListen();
        }


        private void initiateListen()
        {
            tcpListener = new TcpListener(IPAddress.Parse("127.0.0.1"), 3000);
            listenThread = new Thread(new ThreadStart(ListenForClients));
            listenThread.Start();
        }


        /// <summary>
        /// Listens for client connections
        /// </summary>
        private static void ListenForClients()
        {
            tcpListener.Start();

            while (true)
            {
                try
                {
                    // Blocks until a client has connected to the server
                    TcpClient client = tcpListener.AcceptTcpClient();

                    // Create a thread to handle communication 
                    // with connected client
                    Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                    clientThread.Start(client);
                }
                catch (Exception)
                {

                }
            }
        }



        private void buttonStartListening_Click(object sender, EventArgs e)
        {
            initiateListen();
        }

        /// <summary>
        /// Handles client connections
        /// </summary>
        /// <param name="client"></param>
        private static void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;
            //NetworkStream clientStream = tcpClient.GetStream();
            clientStream = tcpClient.GetStream();

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
                textFromClient = tcpClient.Client.LocalEndPoint + " " + tcpClient.Client.RemoteEndPoint + encoder.GetString(message, 0, bytesRead);

                //Console.WriteLine("To: " + tcpClient.Client.LocalEndPoint);
                //Console.WriteLine("From: " + tcpClient.Client.RemoteEndPoint);
                //Console.WriteLine(encoder.GetString(message, 0, bytesRead));

            } while (clientStream.DataAvailable);

            // Release connections
            // clientStream.Close();

            //tcpClient.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            textBoxMessage.Text = textFromClient;
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            textFromClient = "";
            textBoxMessage.Clear();
        }

        private void buttonSendToClient_Click(object sender, EventArgs e)
        {

            ASCIIEncoding encoder = new ASCIIEncoding();
            byte[] buffer = encoder.GetBytes("Hello Server!");

            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
        }
    }
}

