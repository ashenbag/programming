import sys
import telnetlib
import re

def DutSendCmd (instance, cmd, max_wait=3, pattern='# '):
	instance.write(cmd + '\n')
	iteration = 1
	text_output = ""
	while (iteration <= max_wait):
		print "iteration %d" % iteration
		index,match_object,text_return = instance.expect([r'[^\\n\\r]*# '],5)
		text_output = text_output + text_return
		if index != -1:
			print "expect match"
			break
		iteration += 1
	if index == -1:
		return (-1,text_output)
	else:
		return (1,text_output)


def connect_to_dut(telnet_ip,telnet_port):
	telnet_instance = telnetlib.Telnet(telnet_ip,telnet_port,10)
	telnet_instance.write("\n")
	index,match_object,text_return = telnet_instance.expect([r'[^\\n\\r]*# '])
	if index != -1:
		print "expect match"
	elif index == -1:
		print "unable to connect"

	return (index,telnet_instance)

telnet_ip = "10.31.233.22"
telnet_port = 3014

return_value, telnet_instance = connect_to_dut(telnet_ip,telnet_port)

print DutSendCmd(telnet_instance,"show version")
#print DutSendCmd(telnet_instance,"show run")
interface_name = "ethernet 3/1"
command_string = "show interface" + " " + interface_name
return_value, return_text = DutSendCmd(telnet_instance,command_string)
print "return_text"
print return_text
m = re.search('Receive Statistics:[\n\r]\s+(\d+)\s+packets.*Transmit Statistics:[\n\r]\s+(\d+)\s+packets',return_text,re.DOTALL)
if m:
	rx_packet_count = m.group(1)
	print "rx_packet_count"
	print rx_packet_count
	tx_packet_count = m.group(2)
	print "tx_packet_count"
	print tx_packet_count
else:
	print "no match"
