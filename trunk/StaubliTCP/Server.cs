using System;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace TCPServer
{
    public class Server
    {
        private TcpListener tcpListener;
        private Thread listenThread;

        private string response = "Nuh Uh.  For real?";

        public Server()
        {
            this.tcpListener = new TcpListener(IPAddress.Any, 3000);
            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();
        }

        private void ListenForClients()
        {

            this.tcpListener.Start();

            while (true)
            {
                //blocks until a client has connected to the server
                TcpClient client = this.tcpListener.AcceptTcpClient();

                //create a thread to handle communication 
                //with connected client
                Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                clientThread.Start(client);
            }
        }

        private void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;


            Console.WriteLine("Client found at: " + tcpClient.Client.RemoteEndPoint.ToString());

            NetworkStream clientStream = tcpClient.GetStream();

            byte[] message = new byte[4096];
            int bytesRead;

            while (true)
            {
                bytesRead = 0;

                try
                {
                    //blocks until a client sends a message
                    bytesRead = clientStream.Read(message, 0, 4096);
                }
                catch
                {
                    //a socket error has occured
                    break;
                }

                if (bytesRead == 0)
                {
                    //the client has disconnected from the server
                    break;
                }

                //message has successfully been received
                ASCIIEncoding encoder = new ASCIIEncoding();
                string recievedMessage = encoder.GetString(message, 0, bytesRead);
                System.Diagnostics.Debug.WriteLine(recievedMessage);
                Console.WriteLine("Message Recieved: " + recievedMessage);
                
                byte[] data = System.Text.Encoding.ASCII.GetBytes(response);
                Console.WriteLine("Message Sent: " + response);
                clientStream.Write(data, 0, data.Length);
            }

            tcpClient.Close();
        }


    }
}
