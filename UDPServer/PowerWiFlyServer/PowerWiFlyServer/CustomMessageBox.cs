//@raaj A0081202Y
using System.Drawing;
using System.Windows.Forms;

namespace PowerWiFlyServer
{
    public static class TinyAlertView
    {
        private static TinyAlert tinyAlert = new TinyAlert();
        private static Form form;
        public enum StateTinyAlert { SUCCESS, FAILURE, WARNING };

        /// <summary>
        /// Passes an instance of UI into TinyAlertView to capture UI Movements
        /// </summary>
        /// <param name="formPass">Instance of UI passed in</param>
        internal static void SetUI(Form formPass)
        {
            form = formPass;
        }

        /// <summary>
        /// Displays TinyAlert for the specified period of time
        /// </summary>
        /// <param name="state">Set state of TinyAlert</param>
        /// <param name="response">Set the response to be shown</param>
        /// <returns></returns>
        internal static void Show(StateTinyAlert state, string response)
        {
            switch (state)
            {
                case StateTinyAlert.SUCCESS:
                    tinyAlert.SetColorText(Color.Green, Color.White, response);
                    break;

                case StateTinyAlert.FAILURE:
                    tinyAlert.SetColorText(Color.Maroon, Color.White, response);
                    break;

                case StateTinyAlert.WARNING:
                    tinyAlert.SetColorText(Color.DarkOrange, Color.Black, response);
                    break;

            }
            tinyAlert.ShowInTaskbar = false;
            tinyAlert.ShowDisplay();
            form.BringToFront();
            SetLocation();
        }

        /// <summary>
        /// Ensures TinyAlert moves with UI
        /// </summary>
        internal static void SetLocation()
        {
            tinyAlert.Location = new Point(form.Left, form.Bottom +5);
        }

        /// <summary>
        /// Set how long you want TinyAlert to display before fading out
        /// </summary>
        /// <param name="time"></param>
        internal static void SetTiming(int time)
        {
            tinyAlert.SetTiming(time);
        }

        /// <summary>
        /// Dismiss even if TinyAlert is still showing
        /// </summary>
        internal static void DismissEarly()
        {
            tinyAlert.Dismiss();
        }

        
    }

}
