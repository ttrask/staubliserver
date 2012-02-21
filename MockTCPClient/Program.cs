using System;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Configuration;

namespace MockTCPClient
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpClient client = new TcpClient();

            String serverAddress = ConfigurationManager.AppSettings["TCPServerAddress"].ToString();

            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(serverAddress.Split(',')[0]), (Int32.Parse(serverAddress.Split(',')[1])));

            client.Connect(serverEndPoint);

            NetworkStream clientStream = client.GetStream();

            ASCIIEncoding encoder = new ASCIIEncoding();
            byte[] buffer = encoder.GetBytes("Hello Server!");

            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();

        }
    }
}
