/**
 * Copyright (c) 2018 Inversebit
 * 
 * This code is licensed under the MIT License.
 * Full license text: https://opensource.org/licenses/MIT
 * 
 */

using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Timers;

namespace ArduinoPCMonitor
{
    class Program
    {
        /// <summary>
        /// Total PC RAM in MB
        /// In this case 8GB = 8 * 1024MB
        /// </summary>
        private const long PC_MEM = 8 * 1024;

        /// <summary>
        /// Counters to get system stats
        /// </summary>
        private static PerformanceCounter _cpuCounter;
        private static PerformanceCounter _ramCounter;

        /// <summary>
        /// Interface to the arduino
        /// </summary>
        private static SerialPort _serialPort;

        static void Main(string[] args)
        {
            _cpuCounter = new PerformanceCounter();
            _cpuCounter.CategoryName = "Processor";
            _cpuCounter.CounterName = "% Processor Time";
            _cpuCounter.InstanceName = "_Total";

            _ramCounter = new PerformanceCounter("Memory", "Available MBytes");
            
            try
            {
                _serialPort = new SerialPort();
                //Change to match arduino port
                _serialPort.PortName = "COM3";
                //Must match arduino baud rate
                _serialPort.BaudRate = 9600;
                _serialPort.Open();

                //Get data every 1000ms = 1 second
                Timer t = new Timer(1000);
                t.Elapsed += new ElapsedEventHandler(UsageReporter);
                t.Start();
            }
            catch (Exception e)
            {
                Console.WriteLine($"cought exception {e}");
            }
            Console.ReadLine();
        }

        public static void UsageReporter(object source, ElapsedEventArgs e)
        {
            //Get values
            var cpu = _cpuCounter.NextValue();
            var ram = _ramCounter.NextValue();
            
            //Adapt values to a percentage between 0 and 100
            var cpui = ((int)cpu).ToString();
            var rami = (100 - (int)((ram / PC_MEM) * 100)).ToString();

            //Send the values to the arduino
            _serialPort.WriteLine($"{cpui} {rami}");
        }
    }
}
