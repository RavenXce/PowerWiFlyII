using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PowerWiFlyServer
{
    class EventHandlers
    {
        public static event EventHandler UpdateOutputBoxHandler;
        public static void UpdateOutputBox(string text) { UpdateOutputBoxHandler(text, EventArgs.Empty); }
        public static event EventHandler ButtonStateHandler;
        public static void UpdateButtonState(int state) { ButtonStateHandler(state, EventArgs.Empty); }
    }
}
