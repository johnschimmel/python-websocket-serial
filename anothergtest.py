
# coding=UTF-8
import time
import serial
from serial.tools.list_ports import comports
from gevent import monkey, spawn, sleep
monkey.patch_all()
import urllib2
import Tkinter as tk

from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler


run_forever = True


class App(object):
    def __init__(self, root):
        
        self.frame = tk.Frame(root)
        self.text = tk.Text(self.frame)
        self.text.pack()
        self.entry = tk.Entry(self.frame)
        self.entry.insert(tk.END, "http://ipv4.google.com")
        self.entry.pack()
        self.isOn = False

        def fetch_url():
            url = self.entry.get()
            self.text.delete(1.0, tk.END)
            self.text.insert(tk.END, "Fetching...")
            data = urllib2.urlopen(url).read()
            self.text.delete(1.0, tk.END)
            self.text.insert(tk.END, data)

        self.fetch_button = tk.Button(self.frame, text="Fetch URL", command=lambda : spawn(fetch_url))
        self.fetch_button.pack()

        
        def quit():
            global run_forever
            run_forever = False

        self.quit_string = tk.StringVar()
        self.quit_button = tk.Button(self.frame, textvariable=self.quit_string, command=quit)
        self.quit_button.pack()

        self.frame.pack()


        def check_for_block():
            """ Simple visual indicator if mainloop is blocked """
            i = 0
            while True:
                self.quit_string.set("Quit " + "-\|/"[i % 4])
                i += 1
                sleep(0.1)

        spawn(check_for_block)

        def websocket_app(environ, start_response):
            if environ["PATH_INFO"] == '/echo':
                ws = environ["wsgi.websocket"]
                message = ws.receive()
                print message
                self.isOn = not self.isOn
                sendVal = "y" if self.isOn else "n"
                s.writer(sendVal)
                
                ws.send(message + " wowowowowowowo")

        def start_websocket():
            server = pywsgi.WSGIServer(("", 9001), websocket_app, handler_class=WebSocketHandler)
            print "Starting websocket server"
            server.serve_forever()
            
        spawn(start_websocket)

class SerialPort():
  ports = []
  
  def __init__(self, serial_instance):
    self.serial = serial_instance
    self.ports.append(self)
  
  def close(self):
    self.serial.close()

  def start(self):
    print 'starting'
    # self.thread_read = threading.Thread(target=self.reader)
    # self.thread_read.start()


  def reader(self):
    while True:
      buffer = ''
      buffer = ser.read(ser.inWaiting())
      
      if buffer and buffer is not empty:
        print buffer
      #   for conn in WSHandler.connections:
      #     conn.write_message(buffer)
      # time.sleep(0.01)
      
  def writer(self, data):
    print "sending", data
    self.serial.write(str(data))

if __name__ == '__main__':
    root = tk.Tk()
    app = App(root)

    # serial port setup
    ser = serial.Serial('/dev/tty.usbmodem1421')
    ser.timeout = 0
    s = SerialPort(ser)
    s.start()
    
    for port, desc, hwid in sorted(comports()):
        print('--- %-20s %s\n' % (port, desc))
    
    while run_forever:
        root.update()
        sleep(0.01)