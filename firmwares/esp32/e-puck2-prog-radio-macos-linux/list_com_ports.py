import serial.tools.list_ports
import re

def serial_ports():
	# produce a list of all serial ports. The list contains a tuple with the port number, 
	# description and hardware address
	#
	ports = list(serial.tools.list_ports.comports())  
	
	# return the port if 'USB' is in the description 
	#for port_no, description, address in ports:
	#	print("num = " + port_no)
	#	print("desc = " + description)
	#	print("addr = " + address)
	#	print("\r\n")	
		#if "GDB" in description:
		#	print("num = " + port_no)
		#	print("desc = " + description)
		#	print("addr = " + address)
		#	print("\r\n")
		#if  in description:
		#	#return port_no
		#	print(port_no)
	
	epuck_ports = list()
			
	for port in ports:
		#print("device = " + str(port.device))
		#print("name = " + str(port.name))
		#print("desc = " + str(port.description))
		#print("hwid = " + str(port.hwid))
		#print("vid = " + str(port.vid))
		#print("pid = " + str(port.pid))
		#print("serial_number = " + str(port.serial_number))
		#print("location = " + str(port.location))
		#print("manufacturer = " + str(port.manufacturer))
		#print("product = " + str(port.product))
		#print("interface = " + str(port.interface))
		#print("\r\n")
		if port.vid==7504 and port.pid==24600: # Put in a list only the ports related to the programmer
			epuck_ports.append(port)

	if len(epuck_ports) == 2:
		num1 = (re.findall(r'\d+', str(epuck_ports[0].device))) # Extract numbers from string, we are interested to the last digits
		#print("extracted num1 = " + str(num1))
		num2 = (re.findall(r'\d+', str(epuck_ports[1].device)))
		#print("extracted num2 = " + str(num2))
		#print("num1 = " + str(num1[-1]))
		#print("num2 = " + str(num2[-1]))
		if int(num1[-1]) > int(num2[-1]):
			print(str(epuck_ports[0].device))
		else:
			print(str(epuck_ports[1].device))


serial_ports()