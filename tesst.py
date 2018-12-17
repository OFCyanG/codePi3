#! /usr/bin/python
import sys
import paho.mqtt.publish as publish

import bluetooth
import time

# hc01.com
#bd_addr = "20:16:04:18:84:20"
# HC-05
bd_addr = "20:16:04:25:88:21"

port_b = 1
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bd_addr, port_b))

power = "12"
locatex = "0"
locatey = "0"
extra = "0"

print ("Connected to HC-05!")
check = 0
while 1:
    try:
        data = sock.recv(1)
        if data == '#':
            check = 1
            dataTotal = ""
        if check == 1 and data != '#' and data != '~':
            dataTotal += data
        if data == '~':
            check=0
            listItem = dataTotal.split();
            print (dataTotal)
            msg = "{\"mac\":\"" +bd_addr+ "\",\"pos\":{\"x\":" +locatex+ ",\"y\":" +locatey+ "},\"power\":" +power+ ",\"param\":{\"light\":" +listItem[0]+ ",\"temp\":" +listItem[1]+ ",\"hum\":" +listItem[2]+ ",\"eva\":" +listItem[3]+ ",\"act\":" +listItem[4]+ "},\"extra\":"+extra+"}"
            publish.single("/raspian/results", payload = msg , qos = 0 , retain= False ,hostname = "m14.cloudmqtt.com" , port = 16954 , client_id = "" , keepalive = 60 , will = None , auth = {'username':"llddsiwh",'password':"xSm_ikeuO3CZ"} , tls = None)
            print (msg)
    except Exception:
        time.sleep(10)
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        sock.connect((bd_addr, port_b))
        check = 0
        
sock.close()


