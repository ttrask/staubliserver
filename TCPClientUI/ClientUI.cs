using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
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
using TCPClient;
using Zion.Input;

namespace TCPClientUI
{
    public partial class ClientForm : Form
    {
        static TcpClient client;
        static int _p5FilteredPointCount = 15 ;

        static int _p5MovementThreshhold = 60;

        IPEndPoint serverEndPoint;
        NetworkStream clientStream;
        private static string _textFromServer;
        private P5Dll p5DLL;
        private P5Info p5info;

        private static ThreeDOFPoint currentPosition;

        static List<ThreeDOFPoint> p5Points = new List<ThreeDOFPoint>();


        private bool _serverStarted = false;

        private bool _clientConnected = false;

        protected bool ServerConnected
        {
            get
            {
                return _serverStarted;
            }
            set
            {
                btnStartServer.Text = value ? "Close Server" : "Start Server";

                _serverStarted = value;
            }
        }
        
        protected bool ClientConnected{
            get{
                return _clientConnected;
            }
            set{
                btnConnectToServer.Text = value ? "Disconnect" : "Connect";
                

                btnSendMessage.Enabled = value;

                _clientConnected = value;
            }
        }


        static string myIP;
        static string myHost;

        static String textFromServer
        {
            get;
            set; 

        }

        public ClientForm()
        {
            InitializeComponent();

            ClientConnected = false;

            txtIP.Text = System.Configuration.ConfigurationSettings.AppSettings["ClientIP"];
            txtPort.Text = System.Configuration.ConfigurationSettings.AppSettings["ClientPort"];
            txtServerPort.Text = System.Configuration.ConfigurationSettings.AppSettings["ServerPort"];

            SetHostInfo();

        }

        public void SetHostInfo(){
            
            myHost = System.Net.Dns.GetHostName();
            myIP = null;

            for (int i = 0; i <= System.Net.Dns.GetHostEntry(myHost).AddressList.Length - 1; i++)
            {
                if (System.Net.Dns.GetHostEntry(myHost).AddressList[i].AddressFamily != AddressFamily.InterNetworkV6)
                {
                    myIP = System.Net.Dns.GetHostEntry(myHost).AddressList[i].ToString();
                    break;
                }
            }

            txtServerIP.Text = myIP;

            //txtIP.Text = myIP;
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
                    txtClientMessage.AppendText("Invalid IP Address");
                    throw new Exception();
                }

                if (!Int32.TryParse(txtPort.Text, out port))
                {
                    txtClientMessage.AppendText("Invalid Port!");
                    throw new Exception();
                }

                serverEndPoint = new IPEndPoint(ip, port);
            }
            catch{
            }
        }


     

        public Boolean connectTCP()
        {
            try
            {
                client.Connect(serverEndPoint);
                clientStream = client.GetStream();
                txtClientMessage.Text = "Connected to Server: " + client.Client.RemoteEndPoint.ToString();
                // Create a thread to handle communication 
                // with connected client
                Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                clientThread.Start(client);
                lblClientIP.Text = client.Client.LocalEndPoint.ToString();
                //lblHostIP.Text = client.Client.RemoteEndPoint.ToString();

                ClientConnected = true;

                return true;
            }
            catch
            {
                client.Close();
                ClientConnected = false;
                return false;
            }
        }

        public void sendMsg()
        {
            ASCIIEncoding encoder = new ASCIIEncoding();

            byte[] buffer = encoder.GetBytes("");
            clientStream = client.GetStream();
            clientStream.Write(buffer, 0, buffer.Length);
            buffer = encoder.GetBytes(txtClientSendMessage.Text);
            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
            //clientStream.Close();
        }

        public Boolean SendP5Coordinates(string message)
        {

            ASCIIEncoding encoder = new ASCIIEncoding();

            byte[] buffer = encoder.GetBytes("");
            clientStream = client.GetStream();
            //clientStream.Write(buffer, 0, buffer.Length);
            
            buffer = encoder.GetBytes(message);
            txtTCPString.Text = buffer.ToString();
            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();

            return true;
        }

        //private void button1_Click(object sender, EventArgs e)
        //{
        //    txtServerMessage.Text = Connect("127.0.0.1", 3000, 
        //}

        private void btnSendClientMessage_Click(object sender, EventArgs e)
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
                    txtClientMessage.AppendText(textFromServer); // runs on UI thread
                });

            } while (true);//(clientStream.DataAvailable);

            //tcpClient.Close();
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            // Release connections
            clientStream.Close();
            //tcpClient.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                p5DLL = new P5Dll();
                p5DLL.SetMouseState(false);
                p5Timer.Start();

                try
                {
                    if (!p5DLL.Connected)
                        p5DLL.Connect();

                    if (p5DLL.Connected)
                    {
                        txtP5Connect.Text = "P5 Connected";
                        p5info = p5DLL.GetInfo();
                    }
                    else
                    {
                        txtP5Connect.Text = "P5 Not Connected";
                    }
                }
                catch (EntryPointNotFoundException ex)
                {
                    txtP5Message.Text = ex.Message;
                }
                catch (TypeLoadException ex)
                {
                    txtP5Message.Text = ex.Message;
                }
            }
            catch (Exception ex)
            {
                txtP5Message.Text = ex.Message;
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (!ClientConnected)
            {
                iniTCP();
                if (connectTCP())
                    ClientConnected = true;
                else
                    ClientConnected = false;
            }
            else
            {
                CloseClient();

            }
        }

        private void CloseClient()
        {
            client.Close();
            ClientConnected = false;

            txtClientMessage.Text = "Client Disconnected";
        }

        private void p5Timer_Tick(object sender, EventArgs e)
        {
            
            try
            {
                if(!p5DLL.Connected)
                    p5DLL.Connect();

                if (p5DLL.Connected)
                {
                    grpMotion.Enabled = true;

                    txtP5Connect.Text = "P5 Connected";
                    
                    P5Data data = p5DLL.GetData();
                    lblX.Text = String.Format("{0:0.00}", data.X);
                    lblY.Text = String.Format("{0:0.00}", data.Y);
                    lblZ.Text = String.Format("{0:0.00}", data.Z);
                    p5Points.Add(new ThreeDOFPoint() { x = data.X, y = data.Y, z = data.Z });

                    if (currentPosition == null)
                    {
                        currentPosition = new ThreeDOFPoint() { x = data.X, y = data.Y, z = data.Z };
                    }

                    if (p5Points.Count == _p5FilteredPointCount)
                    {
                        p5Timer.Stop();

                        double x = CalculateStdDev(p5Points.Select(p=>p.x));
                        double y = CalculateStdDev(p5Points.Select(p=>p.y));
                        double z = CalculateStdDev(p5Points.Select(p=>p.z));



                        lblX.Text = String.Format("{0:0.00}", x);
                        lblY.Text = String.Format("{0:0.00}", y);
                        lblZ.Text = String.Format("{0:0.00}", z);

                        
                        ThreeDOFPoint moveVector = new ThreeDOFPoint() { x = currentPosition.x - x, y = currentPosition.y - y, z = currentPosition.z - z };

                        if (Math.Abs(moveVector.x) < _p5MovementThreshhold && Math.Abs(moveVector.y) < _p5MovementThreshhold && Math.Abs(moveVector.z) < _p5MovementThreshhold)
                        {
                            txtP5Message.Text += String.Format("{0}Position Moved: x={1:0.00} y={2:0.00} z={3:0.00}",System.Environment.NewLine, moveVector.x, moveVector.y, moveVector.z);
                            txtP5Message.SelectionStart = txtP5Message.Text.Length;
                            txtP5Message.ScrollToCaret();

                            if(ClientConnected)
                                SendP5Coordinates(String.Format("{0:0} {1:0} {2:0} 0 0 0{3}", moveVector.x, moveVector.y, moveVector.z, System.Environment.NewLine));

                            currentPosition = new ThreeDOFPoint() { x = x, y = y, z = z };
                        }

                        p5Points.Clear();
                        
                        p5Timer.Start();
                    }
                }
                else
                {
                    grpMotion.Enabled = false;
                    txtP5Connect.Text = "P5 Not Connected";
                }
            }
            catch (EntryPointNotFoundException ex)
            {
                txtP5Message.Text = ex.Message;
            }
            catch (TypeLoadException ex)
            {
                txtP5Message.Text = ex.Message;
            }
        }


#region Server code
         private static TcpListener tcpListener;
        private static Thread listenThread;
        static NetworkStream serverStream;
        static string textFromClient = "";
        //private string endl = "\r\n";

        public delegate void MessageReceivedHandler(string message);
        public event MessageReceivedHandler MessageReceived;


        private void initiateListen()
        {
            try
            {
                int port;

                if (!Int32.TryParse(txtServerPort.Text, out port))
                    port = 3000;

                tcpListener = new TcpListener(port);
                listenThread = new Thread(new ThreadStart(ListenForClients));

                listenThread.Start();

                txtServerMessage.Text += System.Environment.NewLine + "Server listening at: " + myIP;

                //txtIP.Text = myIP;
                //txtPort.Text = port.ToString();

                ServerConnected = true;
            }
            catch
            {
            }
        }


        /// <summary>
        /// Listens for client connections
        /// </summary>
        private void ListenForClients()
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
                    Thread clientThread = new Thread(new ParameterizedThreadStart(HandleServerComm));
                    clientThread.Start(client);
                }
                catch (Exception)
                {

                }
            }
        }



       

        /// <summary>
        /// Handles client connections
        /// </summary>
        /// <param name="client"></param>
        private void HandleServerComm(object client)
        {
            
            TcpClient tcpClient = (TcpClient)client;
            //NetworkStream serverStream = tcpClient.GetStream();
            serverStream = tcpClient.GetStream();

            byte[] message = new byte[4096];
            int bytesRead;

            do
            {
                bytesRead = 0;

                try
                {
                    // Blocks until a client sends a message                    
                    bytesRead = serverStream.Read(message, 0, 4096);
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
                textFromClient =  encoder.GetString(message, 0, bytesRead);

                AppendServerMessage(textFromClient);

                //Console.WriteLine("To: " + tcpClient.Client.LocalEndPoint);
                //Console.WriteLine("From: " + tcpClient.Client.RemoteEndPoint);
                //Console.WriteLine(encoder.GetString(message, 0, bytesRead));

            } while (serverStream.DataAvailable);

            // Release connections
            // serverStream.Close();

            //tcpClient.Close();
        }



        public void AppendClientMessage(string value)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendClientMessage), new object[] { value });
                return;
            }

            txtClientMessage.Text += String.Format("{0} [{1}]: C {2}", System.Environment.NewLine, DateTime.Now.ToString("HH:mm:ss"), textFromClient) ;
            txtClientMessage.SelectionStart = txtClientMessage.Text.Length;
            txtClientMessage.ScrollToCaret();

        }

        public void AppendServerMessage(string value)
        {

            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendServerMessage), new object[] { value });
                return;
            }

            txtServerMessage.Text += String.Format("{0} [{1}]: C {2}", System.Environment.NewLine, DateTime.Now.ToString("HH:mm:ss"), textFromClient);
            txtServerMessage.SelectionStart = txtServerMessage.Text.Length;
            txtServerMessage.ScrollToCaret();

        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            textFromClient = "";
            txtClientMessage.Clear();
        }

        private void buttonSendToClient_Click(object sender, EventArgs e)
        {

            ASCIIEncoding encoder = new ASCIIEncoding();
            byte[] buffer = encoder.GetBytes("Hello Server!");

            serverStream.Write(buffer, 0, buffer.Length);
            serverStream.Flush();
        }

        private void CloseServer(){
            try
            {
                tcpListener.Stop();
                txtServerMessage.Text += System.Environment.NewLine + "Server stopped.";
                ServerConnected = false;
            }
            catch
            {
            }
        }

        private void btnStartServer_Click(object sender, EventArgs e)
        {
            if (!ServerConnected)
            {
                initiateListen();
            }
            else
            {
                CloseServer();
             
            }
        }

        private void ClientForm_FormClosing(object sender, FormClosingEventArgs e)
        {
        }


        private double CalculateStdDev(IEnumerable<double> values)
        {
            double ret = 0;
            if (values.Count() > 0)
            {
                //Compute the Average      
                double avg = values.Average();
                //Perform the Sum of (value-avg)_2_2      
                double sum = values.Sum(d => Math.Pow(d - avg, 2));
                //Put it all together      
                ret = Math.Sqrt((sum) / (values.Count() - 1));
            }
            return ret;
        }

    }

#endregion


    class ThreeDOFPoint{

        public double x;
        public double y;
        public double z;
        public double rx;
        public double ry;
        public double rz;
    }
    }
