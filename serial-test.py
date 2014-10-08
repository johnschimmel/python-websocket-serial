import serial
from serial.tools.list_ports import comports

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



ser = serial.Serial('/dev/cu.RNBT-2BDF-RNI-SPP')
ser.timeout = 0
ser.baudrate = 115200
ser.setDTR(False)

s = SerialPort(ser)
c = 0
while not ser.isOpen():
	c = c +1
	print 'waiting %d' % c


# if ser.inWaiting():
# 	buffer = None
# 	buffer = ser.read(ser.inWaiting())

# 	if buffer and buffer is not None:
# 		print buffer

s.writer('A1\n')

s.close()