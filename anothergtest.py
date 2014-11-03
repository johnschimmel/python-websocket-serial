
# coding=UTF-8
import time
import serial
from serial.tools.list_ports import comports
from gevent import monkey, spawn, sleep
monkey.patch_all()
import urllib2
import Tkinter as tk
import struct
import json

from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler


run_forever = True

serialConnected = False
serialContacted = False
ser = None
s = None

global_websocket = None

appConfig = {
  "title" : "Capacita by DIYAbility"
}

controllerMap = {
  "*": "*",
  "cross": "X",
  "circle": "O",
  "triangle": "T",
  "square": "S",
  "dpad_up": "U",
  "dpad_left": "F",
  "dpad_right": "G",
  "dpad_down": "D",
  "select": "Z",
  "start": "Y",
  "ps": "P",
  "left_joy_x": "L",
  "left_joy_y": "l"
}


class App(object):
    def __init__(self, root):
        
        self.frame = tk.Frame(root)
        root.title(appConfig.get('title'))
        # self.text = tk.Text(self.frame)
        # self.text.pack()
        # self.entry = tk.Entry(self.frame)
        # self.entry.insert(tk.END,   "http://ipv4.google.com")
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
        self.serial_connect_string.set('Connect!')
        self.serial_connect_button = tk.Button(self.frame, textvariable=self.serial_connect_string, command=connectSelectedSerial)
        self.serial_connect_button.pack()


        # def ledOn():
        #   s.writer('A1')
          
        # self.on_button = tk.Button(self.frame, text='on', command=ledOn)
        # self.on_button.pack()



        def quit():
            global serialConnected
            if serialConnected:
              ser.close()

            global run_forever
            run_forever = False

        self.quit_string = tk.StringVar()
        self.quit_string.set('Quit')
        self.quit_button = tk.Button(self.frame, textvariable=self.quit_string, command=quit)
        self.quit_button.pack()
        self.frame.pack()

        
        # def check_for_block():
        #     """ Simple visual indicator if mainloop is blocked """
        #     i = 0
        #     self.quit_string.set('Quit')
        #     # while True:
        #     #     self.quit_string.set("Quit " + "-\|/"[i % 1])
        #     #     i += 1
        #     #     sleep(0.1)

        # spawn(check_for_block)

        def read_incoming_serial():
          global serialContacted
          global serialConnected

          while True and serialConnected:
            buffer = ''
            buffer = ser.read(ser.inWaiting())

            # initial handshake
            # if not serialContacted:
            #   ser.write('A\n')
            #   ser.write('\n')
            #   serialContacted = True
  
            if buffer and buffer is not '':
              print buffer + " !!!!!!"
            
            sleep(0.01)



        from geventwebsocket import WebSocketServer, WebSocketApplication, Resource

        class RelayApplication(WebSocketApplication):
          
          isOn = False

          def on_open(self):
            global_websocket = self.ws
            print "Connection opened"
            global_websocket.send("websocket connected")

          def on_message(self, message):
            print message
            
            incomingData = json.loads(message)
            s.writer(incomingData)
            
          def on_close(self, reason):
            print reason
            print 'DISCONNECT!!!!'

        def start_websocket():
          WebSocketServer(
              ('localhost', 9001),
              Resource({'/controller': RelayApplication})
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
        print buffer + " !!!"

      #   for conn in WSHandler.connections:
      #     conn.write_message(buffer)
      # time.sleep(0.01)
      
  def writer(self, data):
            
    if (data['value'] != "*"): # and isinstance( valueToSend, ( int, long ) )):
      valueToSendPrepared = struct.pack('!{0}B'.format(1), int(data['value']))
    else:
      valueToSendPrepared = data['value'].encode('ascii','ignore')
    
    cmdReceived = data['cmd'].encode('ascii','ignore')
    if cmdReceived in controllerMap:
      cmdToSend = controllerMap[cmdReceived]
    else:
      cmdToSend = cmdReceived

    self.serial.write(cmdToSend)
    self.serial.write(valueToSendPrepared)
    # self.serial.flush()


def position_window(w=300, h=200):
    # get screen width and height
    ws = root.winfo_screenwidth()
    hs = root.winfo_screenheight()
    
    x = 100 #(ws/2) - (w/2) # to center
    y = 100 #(hs/2) - (h/2)
    root.geometry('%dx%d+%d+%d' % (w, h, x, y))

if __name__ == '__main__':
    root = tk.Tk()
    position_window(300, 300)
    app = App(root)
    
    for port, desc, hwid in sorted(comports()):
        print('%s' % (port))
    
    while run_forever:
        root.update()
        sleep(0.01)

