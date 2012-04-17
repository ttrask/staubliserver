using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.Windows.Interop;
using Application = System.Windows.Application;
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

        _3DxMouse._3DxMouse my3DxMouse;
        Message message = new Message();

        static TcpClient client;
        static int _p5FilteredPointCount = 6 ;
        static int _puckTimerDelay;
        static int _puckTimerDeaySize = 5 ;
        //outbound scaling factor. Used as a percentage of actual motion (0.1 = 10%)  
        //make larger for massive damage to the weak spot.
        static double p5OutputScalingFactor = 0.5;
        static double puckOutputScalingFactor = .1;
        static int _p5TimerLength = 20; //16ms, 60Hz
        static int _p5MovementThreshhold = 2000;
        static int garbageFilter=20;
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

            p5Timer.Interval = _p5TimerLength;

            initPuck();

        }


        public void initPuck()
        {
            IntPtr hwnd = IntPtr.Zero;
            

            try
            {
                hwnd = this.Handle;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }

            //Get the Hwnd source   
            HwndSource source = HwndSource.FromHwnd(hwnd);
            //Win32 queue sink
            //source.AddHook(new HwndSourceHook(WndProc));

            // Connect to Raw Input & find devices
            my3DxMouse = new _3DxMouse._3DxMouse(hwnd);
            int NumberOf3DxMice = my3DxMouse.EnumerateDevices();

            // Setup event handlers to be called when something happens
            
            my3DxMouse.MotionEvent += new _3DxMouse._3DxMouse.MotionEventHandler(MotionEvent);
            my3DxMouse.ButtonEvent += new _3DxMouse._3DxMouse.ButtonEventHandler(ButtonEvent);

            // Add devices to device list comboBox
            foreach (System.Collections.DictionaryEntry listEntry in my3DxMouse.deviceList)
            {
                _3DxMouse._3DxMouse.DeviceInfo devInfo = (_3DxMouse._3DxMouse.DeviceInfo)listEntry.Value;
            }
            if (my3DxMouse.deviceList.Count > 0)
            {
            }

        }

        protected override void WndProc(ref Message message)
        {
            if (my3DxMouse != null)
            {
                // I could have done one of two things here.
                // 1. Use a Message as it was used before.
                // 2. Changes the ProcessMessage method to handle all of these parameters(more work).
                //    I opted for the easy way.

                //Note: Depending on your application you may or may not want to set the handled param.


                my3DxMouse.ProcessMessage(message);
            }

            base.WndProc(ref message);
            
            //return IntPtr.Zero;
        }

        private void MotionEvent(object sender, _3DxMouse._3DxMouse.MotionEventArgs e)
        {
            Vector3D tv = new Vector3D();
            Vector3D rv = new Vector3D();

            // Change the device?
            _puckTimerDelay++;


            // Currently Translations and Rotations arrive separately (T first, then R)
            // but there is a firmware version that will deliver both together

            
            // Translation Vector?
            if (_puckTimerDelay>_puckTimerDeaySize && (e.TranslationVector != null || e.RotationVector != null))
            {

                double X = 0, Y = 0, Z = 0, rX = 0, rY = 0, rZ = 0;

                if (e.TranslationVector != null)
                {
                    X = e.TranslationVector.Y;
                    Y = -e.TranslationVector.X;
                    Z = e.TranslationVector.Z;
                }
                
                if (e.RotationVector != null)
                {
                    rX = -e.RotationVector.X;
                    rY = -e.RotationVector.X;
                    rZ = -e.RotationVector.Z;
                }

                lblX.Text = X.ToString();
                lblY.Text = Y.ToString();
                lblZ.Text = Z.ToString();

                // Swap axes from HID orientation to a right handed coordinate system that matches WPF model space
                //tv.X = e.TranslationVector.X;
                //tv.Y = -e.TranslationVector.Z;
                //tv.Z = e.TranslationVector.Y;
                TCPSendMotionCoordinates(String.Format("{0:0.00} {1:0.00} {2:0.00} {3:0.00} {4:0.00} {5:0.00}{6}", X * puckOutputScalingFactor, Y * puckOutputScalingFactor, Z * puckOutputScalingFactor, rX * puckOutputScalingFactor, rY * puckOutputScalingFactor, rZ * puckOutputScalingFactor, System.Environment.NewLine));

                _puckTimerDelay = 0;
            }

            // Rotation Vector?
            if (e.RotationVector != null)
            {

                //lblX.Text = e.RotationVector.X.ToString();
                //lblY.Text = e.RotationVector.Y.ToString();
                //lblZ.Text = e.RotationVector.Z.ToString();

               //Swap axes from HID orientation to a right handed coordinate system that matches WPF model space
                
                //TCPSendMotionCoordinates(String.Format("0 0 0 {0:0.00} {1:0.00} {2:0.00}{3}", X * puckOutputScalingFactor, Y * puckOutputScalingFactor, Z * puckOutputScalingFactor, System.Environment.NewLine));
            }

            //UpdateCube(tv, rv);
        }

        private void ButtonEvent(object sender, _3DxMouse._3DxMouse.ButtonEventArgs e)
        {
            //// Change the device?
            
            // Show the buttons that are pressed
            string gripper = "";

            switch(e.ButtonMask.Pressed.ToString("X")){
                case "1":
                    gripper = "grip";
                    break;
                case "2":
                case "3":
                    gripper = "release";
                    break;              
                   
            }

            if (gripper.Length > 0)
            {
                TCPSendMotionCoordinates(gripper);

                this.ButtonLabel.Text = gripper;
            }
            //ResetCube();

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

        public Boolean TCPSendMotionCoordinates(String message)
        {

            ASCIIEncoding encoder = new ASCIIEncoding();
            String response="";
            Int32 responseSize = 0;

            if (!message.EndsWith(System.Environment.NewLine))
            {
                message += System.Environment.NewLine;
            }

            try
            {
                byte[] buffer = encoder.GetBytes("");
                clientStream = client.GetStream();
                //clientStream.Write(buffer, 0, buffer.Length);

                buffer = encoder.GetBytes(message);
                txtTCPString.Text = buffer.ToString();
                clientStream.Write(buffer, 0, buffer.Length);
                clientStream.Flush();
                
                //responseSize = clientStream.Read(buffer, 0, buffer.Length);

                response = System.Text.Encoding.ASCII.GetString(buffer, 0, responseSize);
            }
            catch
            {
            }

            AppendServerMessage(response);

            AppendClientMessage(message);
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
                
                p5ConnectTimer.Start();

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

            txtClientMessage.Text += "\nClient Disconnected";
        }

        private void p5Timer_Tick(object sender, EventArgs e)
        {
            
            try
            {
                if(!p5DLL.Connected)
                    p5Timer.Stop();

                if (p5DLL.Connected)
                {
                    
                    grpMotion.Enabled = true;

                    txtP5Connect.Text = "P5 Connected";
                    
                    P5Data data = p5DLL.GetData();
                    
                    double x, y, z;

                    int yScaler = 1;

                    y = data.Z * yScaler;
                    x = data.Y;
                    z = -data.X ;

                    lblX.Text = String.Format("{0:0.00}", x);
                    lblY.Text = String.Format("{0:0.00}", y);
                    lblZ.Text = String.Format("{0:0.00}", z);
                    p5Points.Add(new ThreeDOFPoint() {x=x,y=y, z=z});

                    if (currentPosition == null)
                    {
                        currentPosition = new ThreeDOFPoint() { x = data.Z, y = data.Y, z = data.X };
                    }

                    if (p5Points.Count >= _p5FilteredPointCount)
                    {
                        p5Timer.Stop();


                        try
                        {


                             x = p5Points.Select(p => p.y).Last();
                             y = p5Points.Select(p => p.z).Last();
                             z = p5Points.Select(p => p.x).Last();



                            lblX.Text = String.Format("{0:0.00}", x);
                            lblY.Text = String.Format("{0:0.00}", y);
                            lblZ.Text = String.Format("{0:0.00}", z);


                            ThreeDOFPoint moveVector = new ThreeDOFPoint();
                            moveVector.x = currentPosition.x - x;
                            moveVector.y = currentPosition.y - y;
                            moveVector.z = currentPosition.z - z;

                            if (Math.Abs(moveVector.x) < _p5MovementThreshhold && Math.Abs(moveVector.y) < _p5MovementThreshhold && Math.Abs(moveVector.z) < _p5MovementThreshhold)
                            {
                                if (Math.Abs(moveVector.x) > garbageFilter || Math.Abs(moveVector.y) > garbageFilter || Math.Abs(moveVector.z) > garbageFilter)
                                {
                                    moveVector.x *= p5OutputScalingFactor;
                                    moveVector.y *= p5OutputScalingFactor;
                                    moveVector.z *= p5OutputScalingFactor;

                                    txtP5Message.Text += String.Format("{0}{4}Position Moved: x={1:000.00}\t   y={2:000.00}\t   z={3:000.00}", System.Environment.NewLine, moveVector.x, moveVector.y, moveVector.z, DateTime.Now.ToString("mm:ss:ffff"));
                                    txtP5Message.SelectionStart = txtP5Message.Text.Length;
                                    txtP5Message.ScrollToCaret();

                                    // if (ClientConnected && ServerConnected)
                                        TCPSendMotionCoordinates(String.Format("{0:0.00} {1:0.00} {2:0.00} 0 0 0{3}", moveVector.x, moveVector.y, moveVector.z, System.Environment.NewLine));

                                    currentPosition.x  = p5Points.Select(p => p.y).Last();
                                    currentPosition.y = p5Points.Select(p => p.z).Last();
                                    currentPosition.z = p5Points.Select(p => p.x).Last();

                                    p5Points.Clear();

                                }
                            }

                            
                            p5Timer.Start();
                        }
                        catch
                        {
                            if(!p5Timer.Enabled)
                                p5Timer.Start();
                        }
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

            try
            {
                txtClientMessage.Text += String.Format("[{1}]: {2}", System.Environment.NewLine, DateTime.Now.ToString("HH:mm:ss"), value);
                txtClientMessage.SelectionStart = txtClientMessage.Text.Length;
                txtClientMessage.ScrollToCaret();
            }
            catch{}
        }

        public void AppendServerMessage(string value)
        {

            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendServerMessage), new object[] { value });
                return;
            }

            txtServerMessage.Text += String.Format("{0} [{1}]: C {2}", System.Environment.NewLine, DateTime.Now.ToString("HH:mm:ss"), value);
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
                //ret = avg;
            }
            return ret;
        }

        private void p5ConnectTimer_Tick(object sender, EventArgs e)
        {
            
            if(!p5DLL.Connected){
                p5DLL.Connect();
            }
            else{
                if(!p5Timer.Enabled){

                    p5ConnectTimer.Stop();
                    
                    p5Timer.Start();
                }
            }

            
        }

        private void btnResetP5_Click(object sender, EventArgs e)
        {
            p5DLL.Close();
            p5ConnectTimer.Start();
        }

        private void ClientForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            TCPSendMotionCoordinates("exit");
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
