using System;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Configuration;
using TCPClient;
using Zion.Input;

namespace MockTCPClient
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpClient client = new TcpClient();

            String serverAddress = "127.0.0.1" ;

            //IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(serverAddress), 3000);

            //client.Connect(serverEndPoint);

            //NetworkStream clientStream = client.GetStream();

            //ASCIIEncoding encoder = new ASCIIEncoding();
            //byte[] buffer = encoder.GetBytes("Hello Server!");

            //clientStream.Write(buffer, 0, buffer.Length);
            //clientStream.Flush();
            while (true)
            {
                Console.WriteLine("Press Enter to send message");
                Console.ReadLine();
                new TCPClient.Client().Connect(serverAddress, 3000, "Oh Snap");


            }

         
        }

      

    }
}
