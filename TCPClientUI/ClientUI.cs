using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
//using System.Linq;
//using System.Linq.Expressions;
using System.Reflection;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace TCPClientUI
{
    public partial class ClientForm : Form
    {
        static TcpClient client;
        IPEndPoint serverEndPoint;
        NetworkStream clientStream;
        private static string _textFromServer;

        static String textFromServer
        {
            get;
            set; 

        }

        public ClientForm()
        {
            InitializeComponent();
        }

        public void iniTCP()
        {
            client = new TcpClient();

            IPAddress ip;
            int port;

            try
            {
                if (!IPAddress.TryParse(txtIP.Text, out ip))
                {
                    txtServerMessage.AppendText("Invalid IP Address");
                    throw new Exception();
                }

                if (!Int32.TryParse(txtPort.Text, out port))
                {
                    txtServerMessage.AppendText("Invalid Port!");
                    throw new Exception();
                }

                serverEndPoint = new IPEndPoint(ip, port);
            }
            catch{
            }
        }


     

        public void connectTCP()
        {
            try
            {
                client.Connect(serverEndPoint);
                clientStream = client.GetStream();
                txtServerMessage.AppendText("Connected to Server: " + client.Client.RemoteEndPoint.ToString());
                // Create a thread to handle communication 
                // with connected client
                Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                clientThread.Start(client);
                lblClientIP.Text = client.Client.LocalEndPoint.ToString();
                lblHostIP.Text = client.Client.RemoteEndPoint.ToString();
            }
            catch
            {
                client.Close();
            }
        }

        public void sendMsg()
        {
            ASCIIEncoding encoder = new ASCIIEncoding();

            byte[] buffer = encoder.GetBytes("Testing TCP/IP connection");
            clientStream = client.GetStream();
            clientStream.Write(buffer, 0, buffer.Length);
            buffer = encoder.GetBytes(txtClientMessage.Text);
            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
            //clientStream.Close();
        }

        //private void button1_Click(object sender, EventArgs e)
        //{
        //    txtServerMessage.Text = Connect("127.0.0.1", 3000, 
        //}

        private void buttonSendMessage_Click(object sender, EventArgs e)
        {
            sendMsg();
        }

        /// <summary>
        /// Handles client connections
        /// </summary>
        /// <param name="client"></param>
        private void HandleClientComm(object client)
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
                textFromServer = String.Format("{0}Server Response:{3}",
                                    System.Environment.NewLine,
                                    tcpClient.Client.LocalEndPoint,
                                    tcpClient.Client.RemoteEndPoint,
                                    encoder.GetString(message, 0, bytesRead));

                this.Invoke((MethodInvoker)delegate
                {
                    txtServerMessage.AppendText(textFromServer); // runs on UI thread
                });

            } while (true);//(clientStream.DataAvailable);


        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            // Release connections
            clientStream.Close();
            //tcpClient.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            iniTCP();
            connectTCP();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }


     

    }
}
