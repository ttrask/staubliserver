namespace TCPClientUI
{
    partial class ClientForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label2 = new System.Windows.Forms.Label();
            this.btnConnectToServer = new System.Windows.Forms.Button();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.txtP5Connect = new System.Windows.Forms.Label();
            this.lblX = new System.Windows.Forms.Label();
            this.lblY = new System.Windows.Forms.Label();
            this.lblZ = new System.Windows.Forms.Label();
            this.txtP5Message = new System.Windows.Forms.TextBox();
            this.p5Timer = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnStartServer = new System.Windows.Forms.Button();
            this.lblServerIP = new System.Windows.Forms.Label();
            this.txtServerIP = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.grpServerMessages = new System.Windows.Forms.GroupBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.grpClient = new System.Windows.Forms.GroupBox();
            this.txtClientMessage = new System.Windows.Forms.TextBox();
            this.btnSendMessage = new System.Windows.Forms.Button();
            this.grpClientMessages = new System.Windows.Forms.GroupBox();
            this.txtServerMessage = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.lblClientIP = new System.Windows.Forms.Label();
            this.lblClientStatus = new System.Windows.Forms.Label();
            this.grpMotion = new System.Windows.Forms.GroupBox();
            this.grpStatus = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.grpServerMessages.SuspendLayout();
            this.grpClient.SuspendLayout();
            this.grpClientMessages.SuspendLayout();
            this.grpMotion.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(290, -38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 14);
            this.label2.TabIndex = 4;
            this.label2.Text = "Response";
            // 
            // btnConnectToServer
            // 
            this.btnConnectToServer.Location = new System.Drawing.Point(271, 29);
            this.btnConnectToServer.Name = "btnConnectToServer";
            this.btnConnectToServer.Size = new System.Drawing.Size(146, 48);
            this.btnConnectToServer.TabIndex = 5;
            this.btnConnectToServer.Text = "Connect";
            this.btnConnectToServer.UseVisualStyleBackColor = true;
            this.btnConnectToServer.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(71, 54);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(162, 20);
            this.txtPort.TabIndex = 9;
            this.txtPort.Text = "1042";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(10, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 16);
            this.label3.TabIndex = 8;
            this.label3.Text = "Port";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(7, 33);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 16);
            this.label4.TabIndex = 7;
            this.label4.Text = "Server IP";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(71, 29);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(162, 20);
            this.txtIP.TabIndex = 6;
            this.txtIP.Text = "172.31.0..17";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(16, 15);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 14);
            this.label7.TabIndex = 14;
            this.label7.Text = "X";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(16, 48);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(15, 14);
            this.label8.TabIndex = 15;
            this.label8.Text = "Y";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(16, 79);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(14, 14);
            this.label9.TabIndex = 16;
            this.label9.Text = "Z";
            // 
            // txtP5Connect
            // 
            this.txtP5Connect.AutoSize = true;
            this.txtP5Connect.Location = new System.Drawing.Point(16, 106);
            this.txtP5Connect.Name = "txtP5Connect";
            this.txtP5Connect.Size = new System.Drawing.Size(78, 14);
            this.txtP5Connect.TabIndex = 17;
            this.txtP5Connect.Text = "Not Connected";
            // 
            // lblX
            // 
            this.lblX.AutoSize = true;
            this.lblX.Location = new System.Drawing.Point(57, 16);
            this.lblX.Name = "lblX";
            this.lblX.Size = new System.Drawing.Size(14, 14);
            this.lblX.TabIndex = 18;
            this.lblX.Text = "X";
            // 
            // lblY
            // 
            this.lblY.AutoSize = true;
            this.lblY.Location = new System.Drawing.Point(57, 48);
            this.lblY.Name = "lblY";
            this.lblY.Size = new System.Drawing.Size(14, 14);
            this.lblY.TabIndex = 19;
            this.lblY.Text = "X";
            // 
            // lblZ
            // 
            this.lblZ.AutoSize = true;
            this.lblZ.Location = new System.Drawing.Point(57, 79);
            this.lblZ.Name = "lblZ";
            this.lblZ.Size = new System.Drawing.Size(14, 14);
            this.lblZ.TabIndex = 20;
            this.lblZ.Text = "X";
            // 
            // txtP5Message
            // 
            this.txtP5Message.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtP5Message.BackColor = System.Drawing.Color.White;
            this.txtP5Message.Location = new System.Drawing.Point(6, 19);
            this.txtP5Message.Multiline = true;
            this.txtP5Message.Name = "txtP5Message";
            this.txtP5Message.ReadOnly = true;
            this.txtP5Message.Size = new System.Drawing.Size(370, 136);
            this.txtP5Message.TabIndex = 21;
            // 
            // p5Timer
            // 
            this.p5Timer.Enabled = true;
            this.p5Timer.Interval = 20;
            this.p5Timer.Tick += new System.EventHandler(this.p5Timer_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.SystemColors.Control;
            this.groupBox1.Controls.Add(this.grpServerMessages);
            this.groupBox1.Controls.Add(this.txtServerIP);
            this.groupBox1.Controls.Add(this.lblServerIP);
            this.groupBox1.Controls.Add(this.btnStartServer);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.groupBox1.Location = new System.Drawing.Point(17, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(427, 227);
            this.groupBox1.TabIndex = 25;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Server Control";
            // 
            // btnStartServer
            // 
            this.btnStartServer.Location = new System.Drawing.Point(272, 13);
            this.btnStartServer.Name = "btnStartServer";
            this.btnStartServer.Size = new System.Drawing.Size(145, 48);
            this.btnStartServer.TabIndex = 25;
            this.btnStartServer.Text = "Start Server";
            this.btnStartServer.UseVisualStyleBackColor = true;
            this.btnStartServer.Click += new System.EventHandler(this.btnStartServer_Click);
            // 
            // lblServerIP
            // 
            this.lblServerIP.AutoSize = true;
            this.lblServerIP.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblServerIP.Location = new System.Drawing.Point(7, 32);
            this.lblServerIP.Name = "lblServerIP";
            this.lblServerIP.Size = new System.Drawing.Size(34, 16);
            this.lblServerIP.TabIndex = 26;
            this.lblServerIP.Text = "My IP";
            // 
            // txtServerIP
            // 
            this.txtServerIP.AutoSize = true;
            this.txtServerIP.Location = new System.Drawing.Point(47, 32);
            this.txtServerIP.Name = "txtServerIP";
            this.txtServerIP.Size = new System.Drawing.Size(32, 14);
            this.txtServerIP.TabIndex = 27;
            this.txtServerIP.Text = "My IP";
            // 
            // textBox1
            // 
            this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox1.CausesValidation = false;
            this.textBox1.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(6, 20);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(400, 83);
            this.textBox1.TabIndex = 28;
            // 
            // grpServerMessages
            // 
            this.grpServerMessages.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.grpServerMessages.Controls.Add(this.button1);
            this.grpServerMessages.Controls.Add(this.textBox1);
            this.grpServerMessages.Controls.Add(this.textBox2);
            this.grpServerMessages.Location = new System.Drawing.Point(10, 83);
            this.grpServerMessages.Name = "grpServerMessages";
            this.grpServerMessages.Size = new System.Drawing.Size(411, 138);
            this.grpServerMessages.TabIndex = 29;
            this.grpServerMessages.TabStop = false;
            this.grpServerMessages.Text = "Messages";
            // 
            // textBox2
            // 
            this.textBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox2.Location = new System.Drawing.Point(6, 110);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(300, 20);
            this.textBox2.TabIndex = 26;
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(312, 110);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(95, 22);
            this.button1.TabIndex = 27;
            this.button1.Text = "Send Message";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // grpClient
            // 
            this.grpClient.BackColor = System.Drawing.SystemColors.Control;
            this.grpClient.Controls.Add(this.lblClientStatus);
            this.grpClient.Controls.Add(this.grpClientMessages);
            this.grpClient.Controls.Add(this.btnConnectToServer);
            this.grpClient.Controls.Add(this.txtIP);
            this.grpClient.Controls.Add(this.label4);
            this.grpClient.Controls.Add(this.label3);
            this.grpClient.Controls.Add(this.txtPort);
            this.grpClient.Location = new System.Drawing.Point(17, 246);
            this.grpClient.Name = "grpClient";
            this.grpClient.Size = new System.Drawing.Size(427, 265);
            this.grpClient.TabIndex = 26;
            this.grpClient.TabStop = false;
            this.grpClient.Text = "Client Control";
            // 
            // txtClientMessage
            // 
            this.txtClientMessage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtClientMessage.Location = new System.Drawing.Point(6, 116);
            this.txtClientMessage.Multiline = true;
            this.txtClientMessage.Name = "txtClientMessage";
            this.txtClientMessage.Size = new System.Drawing.Size(300, 21);
            this.txtClientMessage.TabIndex = 12;
            this.txtClientMessage.Text = "Oh Snap!";
            // 
            // btnSendMessage
            // 
            this.btnSendMessage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSendMessage.Location = new System.Drawing.Point(312, 116);
            this.btnSendMessage.Name = "btnSendMessage";
            this.btnSendMessage.Size = new System.Drawing.Size(99, 20);
            this.btnSendMessage.TabIndex = 10;
            this.btnSendMessage.Text = "Send Message";
            this.btnSendMessage.UseVisualStyleBackColor = true;
            // 
            // grpClientMessages
            // 
            this.grpClientMessages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.grpClientMessages.Controls.Add(this.btnSendMessage);
            this.grpClientMessages.Controls.Add(this.txtClientMessage);
            this.grpClientMessages.Controls.Add(this.txtServerMessage);
            this.grpClientMessages.Controls.Add(this.label2);
            this.grpClientMessages.Controls.Add(this.label5);
            this.grpClientMessages.Controls.Add(this.lblClientIP);
            this.grpClientMessages.Location = new System.Drawing.Point(6, 121);
            this.grpClientMessages.Name = "grpClientMessages";
            this.grpClientMessages.Size = new System.Drawing.Size(411, 144);
            this.grpClientMessages.TabIndex = 13;
            this.grpClientMessages.TabStop = false;
            this.grpClientMessages.Text = "Messages";
            // 
            // txtServerMessage
            // 
            this.txtServerMessage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtServerMessage.BackColor = System.Drawing.Color.White;
            this.txtServerMessage.CausesValidation = false;
            this.txtServerMessage.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.txtServerMessage.Enabled = false;
            this.txtServerMessage.Location = new System.Drawing.Point(6, 26);
            this.txtServerMessage.Multiline = true;
            this.txtServerMessage.Name = "txtServerMessage";
            this.txtServerMessage.ReadOnly = true;
            this.txtServerMessage.Size = new System.Drawing.Size(399, 84);
            this.txtServerMessage.TabIndex = 0;
            this.txtServerMessage.TextChanged += new System.EventHandler(this.txtServerMessage_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(291, -155);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 14);
            this.label5.TabIndex = 10;
            this.label5.Text = "My IP";
            // 
            // lblClientIP
            // 
            this.lblClientIP.AutoSize = true;
            this.lblClientIP.Location = new System.Drawing.Point(356, -155);
            this.lblClientIP.Name = "lblClientIP";
            this.lblClientIP.Size = new System.Drawing.Size(68, 14);
            this.lblClientIP.TabIndex = 12;
            this.lblClientIP.Text = "<Undefined>";
            // 
            // lblClientStatus
            // 
            this.lblClientStatus.AutoSize = true;
            this.lblClientStatus.Font = new System.Drawing.Font("Arial Narrow", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblClientStatus.Location = new System.Drawing.Point(7, 93);
            this.lblClientStatus.Name = "lblClientStatus";
            this.lblClientStatus.Size = new System.Drawing.Size(37, 16);
            this.lblClientStatus.TabIndex = 14;
            this.lblClientStatus.Text = "Status";
            // 
            // grpMotion
            // 
            this.grpMotion.BackColor = System.Drawing.SystemColors.Control;
            this.grpMotion.Controls.Add(this.groupBox2);
            this.grpMotion.Controls.Add(this.txtP5Connect);
            this.grpMotion.Controls.Add(this.label7);
            this.grpMotion.Controls.Add(this.label8);
            this.grpMotion.Controls.Add(this.label9);
            this.grpMotion.Controls.Add(this.lblZ);
            this.grpMotion.Controls.Add(this.lblX);
            this.grpMotion.Controls.Add(this.lblY);
            this.grpMotion.Location = new System.Drawing.Point(467, 13);
            this.grpMotion.Name = "grpMotion";
            this.grpMotion.Size = new System.Drawing.Size(394, 310);
            this.grpMotion.TabIndex = 27;
            this.grpMotion.TabStop = false;
            this.grpMotion.Text = "Motion Control";
            // 
            // grpStatus
            // 
            this.grpStatus.BackColor = System.Drawing.SystemColors.Control;
            this.grpStatus.Location = new System.Drawing.Point(467, 329);
            this.grpStatus.Name = "grpStatus";
            this.grpStatus.Size = new System.Drawing.Size(394, 182);
            this.grpStatus.TabIndex = 28;
            this.grpStatus.TabStop = false;
            this.grpStatus.Text = "Status";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.txtP5Message);
            this.groupBox2.Location = new System.Drawing.Point(6, 133);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(382, 162);
            this.groupBox2.TabIndex = 22;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Messages";
            // 
            // ClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlDark;
            this.ClientSize = new System.Drawing.Size(912, 531);
            this.Controls.Add(this.grpStatus);
            this.Controls.Add(this.grpMotion);
            this.Controls.Add(this.grpClient);
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "ClientForm";
            this.Text = "TCP Client";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.grpServerMessages.ResumeLayout(false);
            this.grpServerMessages.PerformLayout();
            this.grpClient.ResumeLayout(false);
            this.grpClient.PerformLayout();
            this.grpClientMessages.ResumeLayout(false);
            this.grpClientMessages.PerformLayout();
            this.grpMotion.ResumeLayout(false);
            this.grpMotion.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnConnectToServer;
        private System.Windows.Forms.TextBox txtPort;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label txtP5Connect;
        private System.Windows.Forms.Label lblX;
        private System.Windows.Forms.Label lblY;
        private System.Windows.Forms.Label lblZ;
        private System.Windows.Forms.TextBox txtP5Message;
        private System.Windows.Forms.Timer p5Timer;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnStartServer;
        private System.Windows.Forms.Label txtServerIP;
        private System.Windows.Forms.Label lblServerIP;
        private System.Windows.Forms.GroupBox grpServerMessages;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox grpClient;
        private System.Windows.Forms.TextBox txtClientMessage;
        private System.Windows.Forms.Button btnSendMessage;
        private System.Windows.Forms.GroupBox grpClientMessages;
        private System.Windows.Forms.TextBox txtServerMessage;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lblClientIP;
        private System.Windows.Forms.Label lblClientStatus;
        private System.Windows.Forms.GroupBox grpMotion;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox grpStatus;

    }
}

