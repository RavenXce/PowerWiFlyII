using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using Microsoft.Win32;
using System.Runtime.InteropServices;

namespace PowerWiFlyServer
{
    public partial class UI : Form
    {

        ServerThread serverThread;

        public UI()
        {
            InitializeComponent();
            serverThread = new ServerThread(2005);
            Debug.WriteLine("start program");

            TinyAlertView.SetUI(this);
            TinyAlertView.SetTiming(5);

            outputBox.InitializeWithSettings(7);
            EventHandlers.UpdateOutputBoxHandler += UpdateOutputBox;
            EventHandlers.ButtonStateHandler += ButtonStateUpdate;
        }

        private void startButton_Click(object sender, EventArgs e)
        {
            int port = Convert.ToInt32(PortText.Text);
            serverThread.SetPort(port);
            serverThread.StartServer();
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            serverThread.StopServer();
        }

        //Allows resizing of borderless form
        #region Resizing
        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd,
                         int Msg, int wParam, int lParam);
        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();
        private void UI_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
        }

        #endregion

        //Creates rounded edge
        #region Rounded Edge
        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn
        (
            int nLeftRect, // x-coordinate of upper-left corner
            int nTopRect, // y-coordinate of upper-left corner
            int nRightRect, // x-coordinate of lower-right corner
            int nBottomRect, // y-coordinate of lower-right corner
            int nWidthEllipse, // height of ellipse
            int nHeightEllipse // width of ellipse
        );
        #endregion

        //Creates Shadow
        #region Shadow

        private const int CS_DROPSHADOW = 0x20000;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle |= CS_DROPSHADOW;
                return cp;
            }
        }

        #endregion

        private void UI_Resize(object sender, EventArgs e)
        {
            Region = System.Drawing.Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 20, 20));
            TinyAlertView.SetLocation();
        }

        private void UI_Move(object sender, EventArgs e)
        {
            TinyAlertView.SetLocation();
        }


        private void button1_Click_1(object sender, EventArgs e)
        {
            //TinyAlertView.Show(TinyAlertView.StateTinyAlert.SUCCESS, "Testing tiny alert");
            outputBox.DisplayCommand("input", "response");
        }

        private void UpdateOutputBox(object sender, EventArgs args)
        {
            string x = Convert.ToString(sender);
            outputBox.DisplayCommand(x, "none");
            /*
            bool onTop = Convert.ToBoolean(sender);
            this.TopMost = onTop;
            UserInputBox.OnTop(onTop);
            FontBox.OnTop(onTop);
            AlertBox.OnTop(onTop);
             * */
        }

        private void ButtonStateUpdate(object sender, EventArgs args)
        {
            int state = Convert.ToInt32(sender);
            switch (state)
            {
                case 0:
                    startButton.Enabled = false;
                    stopButton.Enabled = false;
                    break;

                case 1:
                    startButton.Enabled = true;
                    stopButton.Enabled = false;
                    break;

                case 2:
                    startButton.Enabled = false;
                    stopButton.Enabled = true;
                    break;

                default:
                    break;
            }
        }
    }
}
