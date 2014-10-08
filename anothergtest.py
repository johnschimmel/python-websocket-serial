
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

serialConnected = False
serialContacted = False
ser = None
s = None

class App(object):
    def __init__(self, root):
        
        self.frame = tk.Frame(root)
        # self.text = tk.Text(self.frame)
        # self.text.pack()
        # self.entry = tk.Entry(self.frame)
        # self.entry.insert(tk.END, "http://ipv4.google.com")
        # self.entry.pack()
        self.serialPortOptionsValues = []
        for port, desc, hwid in sorted(comports()):
          self.serialPortOptionsValues.append(port)
        
        self.serialPortSelectionVar = tk.StringVar(root)
        self.serialPortSelectionVar.set(self.serialPortOptionsValues[0]) # default value
        self.serialPortOptionMenu = apply(tk.OptionMenu, (root, self.serialPortSelectionVar) + tuple(self.serialPortOptionsValues))
        self.serialPortOptionMenu.pack()

        def connectSelectedSerial():

          global serialConnected
          global ser
          global s

          # serial port setup
          # print self.serialPortSelectionVar.get()
          if ( not serialConnected):
            ser = serial.Serial(self.serialPortSelectionVar.get())
            serialConnected = True
            self.serial_connect_string.set('Disconnect')
            ser.setDTR(False)
            ser.baudrate = 115200
            ser.timeout = 0
            time.sleep(0.5)

            s = SerialPort(ser)
            s.start()
            

            # c = 0
            # while not ser.inWaiting():
            #   c = c +1
            #   print 'waiting %d' % c
              
            spawn(read_incoming_serial)
            

          else:
            if ser.isOpen():
              ser.close()

            serialConnected = False
            serialContacted = False
            self.serial_connect_string.set('Connect')

          

        self.serial_connect_string = tk.StringVar()
        self.serial_connect_string.set('Connect')
        self.serial_connect_button = tk.Button(self.frame, textvariable=self.serial_connect_string, command=connectSelectedSerial)
        self.serial_connect_button.pack()

        self.isOn = False

        # def fetch_url():
        #     url = self.entry.get()
        #     self.text.delete(1.0, tk.END)
        #     self.text.insert(tk.END, "Fetching...")
        #     data = urllib2.urlopen(url).read()
        #     self.text.delete(1.0, tk.END)
        #     self.text.insert(tk.END, data)

        # self.fetch_button = tk.Button(self.frame, text="Fetch URL", command=lambda : spawn(fetch_url))
        # self.fetch_button.pack()

        def ledOn():
          s.writer('A1')
          # ser.write('A')
          # ser.write('1')
          # ser.write('\n')
          
        self.on_button = tk.Button(self.frame, text='on', command=ledOn)
        self.on_button.pack()

        def ledOff():
          s.writer('A0')
          # ser.write('1')
          # ser.write('\n')
          # ser.write('\n')
        self.off_button = tk.Button(self.frame, text='OFF', command=ledOff)
        self.off_button.pack()


        def quit():
            global serialConnected
            if serialConnected:
              ser.close()

            global run_forever
            run_forever = False

        self.quit_string = tk.StringVar()
        self.quit_string.set('Connect')
        self.quit_button = tk.Button(self.frame, textvariable=self.quit_string, command=quit)
        self.quit_button.pack()
        self.frame.pack()

        def read_incoming_serial():
          global serialContacted
          global serialConnected

          while True and serialConnected:
            buffer = ''
            buffer = ser.read(ser.inWaiting())

            # initial handshake
            if not serialContacted:
              ser.write('A\n')
              ser.write('\n')
              serialContacted = True
  
            if buffer and buffer is not '':
              print buffer
            
            sleep(0.01)

        def check_for_block():
            """ Simple visual indicator if mainloop is blocked """
            i = 0
            while True:
                self.quit_string.set("Quit " + "-\|/"[i % 1])
                i += 1
                sleep(0.1)

        spawn(check_for_block)


        # def websocket_app(environ, start_response):
        #     print envrion
        #     print '--------'
        #     print start_response

        #     if environ["PATH_INFO"] == '/echo':
        #         ws = environ["wsgi.websocket"]
        #         message = ws.receive()
        #         print message
        #         self.isOn = not self.isOn
        #         sendVal = "A1" if self.isOn else "B1"
        #         s.writer(sendVal + '\n')
        #         s.writer('\n')
                
                
        #         ws.send(message + " wowowowowowowo")

        # def start_websocket():
        #     server = pywsgi.WSGIServer(("", 9001), websocket_app, handler_class=WebSocketHandler)
        #     print "Starting websocket server"
        #     server.serve_forever()
            
        # spawn(start_websocket)


        from geventwebsocket import WebSocketServer, WebSocketApplication, Resource

        class EchoApplication(WebSocketApplication):
          
          isOn = False

          def on_open(self):
            print "Connection opened"

          def on_message(self, message):
            print message
            print "**************"
            self.ws.send('FUCK')
            self.isOn = not self.isOn
            sendVal = "A1" if self.isOn else "B1"
            s.writer(sendVal)


          def on_close(self, reason):
            print reason
            print 'DISCONNECT!!!!'

        def start_websocket():
          WebSocketServer(
              ('', 9001),
              Resource({'/echo': EchoApplication})
          ).serve_forever()

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
    for d in data:
      self.serial.write(d)
      self.serial.flush()
    # print('pyserial : ', data[0])
    # self.serial.write(data[1:])
    # print('pyserial : ', data[1:])
    # self.serial.write('\n')
    # self.serial.flush()
    
    # print "sending", data


if __name__ == '__main__':
    root = tk.Tk()
    app = App(root)
    
    for port, desc, hwid in sorted(comports()):
        print('%s' % (port))
    
    while run_forever:
        root.update()
        sleep(0.01)

