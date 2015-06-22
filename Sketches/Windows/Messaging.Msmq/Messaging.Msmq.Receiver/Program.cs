using Messaging.Msmq.Messages;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Messaging;

namespace Messaging.Msmq.Receiver
{
    class Program
    {
        static void Main(string[] args)
        {
            int limit = 10000;

            String path = ".\\private$\\test001";
            MessageQueue queue;

            if (!MessageQueue.Exists(path))
            {
                queue = MessageQueue.Create(path);
            }
            else
            {
                queue = new MessageQueue(path);
            }

            queue.Formatter = new BinaryMessageFormatter();

            Dictionary<String, RegistrationMessage> storage = new Dictionary<string, RegistrationMessage>();

            Stopwatch watch = new Stopwatch();
            watch.Start();

            while (limit > 0)
            {
                Message message = queue.Receive();

                RegistrationMessage registrationMessage = message.Body as RegistrationMessage;
                if (registrationMessage != null)
                {
                    storage[registrationMessage.Username] = registrationMessage;
                }

                --limit;
            }

            watch.Stop();
            Console.Write("Elapsed = {0} (Count = {1})", watch.Elapsed, storage.Count);
            Console.ReadLine();
        }
    }
}
