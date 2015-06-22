using Messaging.Msmq.Messages;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Messaging;
using System.Text;

namespace Messaging.Msmq.Sender
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
            
            Stopwatch watch = new Stopwatch();
            watch.Start();

            while (limit > 0)
            {
                queue.Send(new RegistrationMessage
                {
                    Username = "username" + limit,
                    DisplayName = "Display Name " + limit,
                    Created = DateTime.Now,
                    Email = "username" + limit + "@email.com",
                    Password = "password"
                });

                --limit;
            }

            watch.Stop();
            Console.Write("Elapsed = {0}", watch.Elapsed);
            Console.ReadLine();
        }
    }
}
