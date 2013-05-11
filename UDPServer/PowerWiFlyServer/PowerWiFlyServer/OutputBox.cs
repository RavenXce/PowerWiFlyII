//@raaj A0081202Y
using System.Drawing;
using System.Windows.Forms;

namespace PowerWiFlyServer
{
    class OutputBox : RichTextBox
    {
        private int textSize;

        /// <summary>
        /// Currently sets the Text Size of the OutputBox
        /// </summary>
        public void InitializeWithSettings(int size)
        {
            textSize = size;
            this.SetOutputSize(textSize);
        }

        #region TextSizeControl

        /// <summary>
        /// Sets the Text Size of the OutputBox directly
        /// </summary>
        /// <param name="size">Size of Text in int</param>
        public void SetOutputSize(int size)
        {
            this.SelectAll();
            var family = this.SelectedText.ToString();
            this.SelectionFont = new Font(family, size);
            this.DeselectAll();
        }

        /// <summary>
        /// Decrease the Text Size by 1 unit, while modifying the settings
        /// </summary>
        public void DecreaseSizeOfOutput()
        {
            textSize -= 1;
            this.SetOutputSize(textSize);
        }

        /// <summary>
        /// Increase the Text Size by 1 unit, while modifying the settings
        /// </summary>
        public void IncreaseSizeOfOutput()
        {
            textSize += 1;
            this.SetOutputSize(textSize);
        }

        #endregion

        #region FormattingControl

        /// <summary>
        /// Set Formatting of Text to be set into OutputBox
        /// </summary>
        public void SetFormat(Color color, string text, int size)
        {
            RichTextBox box = this;
            int start = box.TextLength;
            box.AppendText(text);
            int end = box.TextLength;

            box.Select(start, end - start + 1);
            box.SelectionColor = color;
            box.SelectionFont = new Font("Courier", size);
            box.SelectionLength = 0;
        }

        #endregion

        /// <summary>
        /// This method displays both ToDo++ output and input with correct formatting
        /// </summary>
        /// <param name="userInput">What the User typed in the input box</param>
        /// <param name="systemOutput">What ToDo++ returns as an output</param>
        public void DisplayCommand(string userInput, string systemOutput)
        {
            int currentSize = textSize;
            SetFormat(Color.Blue, "Input: ", currentSize);
            SetFormat(Color.Black, userInput, currentSize);
            SetFormat(Color.Red, "\n", currentSize);
            SetFormat(Color.Red, "Output: ", currentSize);
            SetFormat(Color.Black, systemOutput, currentSize);
            SetFormat(Color.Red, "\n", currentSize);
            this.ScrollToCaret();
        }

    }
}
