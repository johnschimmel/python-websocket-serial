from Tkinter import *
import serial


class SerialPort():
  ports = []
  
  def __init__(self, serial_instance):
    self.serial = serial_instance
    self.ports.append(self)
  
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

class App:

    def __init__(self, master):

        frame = Frame(master)
        frame.pack()

        self.button = Button(
            frame, text="QUIT", fg="red", command=frame.quit
            )
        self.button.pack(side=LEFT)

        self.hi_there = Button(frame, text="Hello", command=self.say_hi)
        self.hi_there.pack(side=LEFT)

    def say_hi(self):
        print "hi there, everyone!"
        s.writer('y')
        

# serial port setup
ser = serial.Serial('/dev/tty.usbmodem1421')
ser.timeout = 0
s = SerialPort(ser)
s.start()


root = Tk()

app = App(root)

root.mainloop()
root.destroy() # optional; see description below
