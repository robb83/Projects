using System;
using System.Collections.Generic;

namespace Messaging.Msmq.Messages
{
    [Serializable]
    public class RegistrationMessage
    {
        public String DisplayName { get; set; }
        public String Username { get; set; }
        public String Password { get; set; }
        public String Email { get; set; }
        public DateTime Created { get; set; }
    }
}
