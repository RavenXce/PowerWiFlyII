namespace PowerWiFlyServer
{
    partial class UI
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
            this.startButton = new System.Windows.Forms.Button();
            this.stopButton = new System.Windows.Forms.Button();
            this.flatTabControl1 = new FlatTabControl.FlatTabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.PortText = new System.Windows.Forms.TextBox();
            this.outputBox = new PowerWiFlyServer.OutputBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.flatTabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.SuspendLayout();
            // 
            // startButton
            // 
            this.startButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.startButton.Location = new System.Drawing.Point(6, 6);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(79, 28);
            this.startButton.TabIndex = 0;
            this.startButton.Text = "Start Server";
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // stopButton
            // 
            this.stopButton.Enabled = false;
            this.stopButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.stopButton.Location = new System.Drawing.Point(91, 6);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(87, 28);
            this.stopButton.TabIndex = 1;
            this.stopButton.Text = "Stop Server";
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // flatTabControl1
            // 
            this.flatTabControl1.Controls.Add(this.tabPage1);
            this.flatTabControl1.Controls.Add(this.tabPage2);
            this.flatTabControl1.Location = new System.Drawing.Point(12, 12);
            this.flatTabControl1.myBackColor = System.Drawing.Color.Gainsboro;
            this.flatTabControl1.Name = "flatTabControl1";
            this.flatTabControl1.SelectedIndex = 0;
            this.flatTabControl1.Size = new System.Drawing.Size(260, 238);
            this.flatTabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.Gainsboro;
            this.tabPage1.Controls.Add(this.PortText);
            this.tabPage1.Controls.Add(this.outputBox);
            this.tabPage1.Controls.Add(this.startButton);
            this.tabPage1.Controls.Add(this.stopButton);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(252, 209);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Main";
            // 
            // PortText
            // 
            this.PortText.BackColor = System.Drawing.SystemColors.Menu;
            this.PortText.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.PortText.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PortText.Location = new System.Drawing.Point(184, 6);
            this.PortText.Name = "PortText";
            this.PortText.Size = new System.Drawing.Size(62, 28);
            this.PortText.TabIndex = 4;
            // 
            // outputBox
            // 
            this.outputBox.BackColor = System.Drawing.Color.LightGray;
            this.outputBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.outputBox.Location = new System.Drawing.Point(6, 40);
            this.outputBox.Name = "outputBox";
            this.outputBox.ReadOnly = true;
            this.outputBox.Size = new System.Drawing.Size(243, 163);
            this.outputBox.TabIndex = 2;
            this.outputBox.Text = "";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.Gainsboro;
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(252, 209);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Devices";
            // 
            // UI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.flatTabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "UI";
            this.Text = "Form1";
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.UI_MouseDown);
            this.Move += new System.EventHandler(this.UI_Move);
            this.Resize += new System.EventHandler(this.UI_Resize);
            this.flatTabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.Button stopButton;
        private FlatTabControl.FlatTabControl flatTabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private OutputBox outputBox;
        private System.Windows.Forms.TextBox PortText;
    }
}

