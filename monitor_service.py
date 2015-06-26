#from mosquitto import Mosquitto as mq
#import paho.mqtt.client as mosquitto
import mosquitto
import time
from RF24 import *

def on_connect(obj, rc):
          print("rc: " + str(rc))

def on_message(obj, msg):
	print(msg.topic + " " + str(msg.payload))
	#mqttc.publish("home/sala/luz", "ON")
	radio.stopListening()
	data = str(msg.payload).lower()
        print 'Sending ', data
        radio.write(data)
        radio.startListening()


def on_publish(obj, mid):
          print("mid: " + str(mid))

def on_subscribe(obj, mid, granted_qos):
     print("Suscribed: " + str(mid) + " " + str(granted_qos))

def on_log(obj, level, string):
     print(string)

def start_mosquitto():
     mqttc = mosquitto.Mosquitto("mqtt")

     mqttc.on_message = on_message
     mqttc.on_connect = on_connect
     mqttc.on_publish = on_publish
     mqttc.on_subscribe = on_subscribe

     mqttc.on_log = on_log

     mqttc.connect("localhost", 1883, 60)

     mqttc.subscribe("home/sala/luz", 0)

     return mqttc

def start_rf24():
     radio = RF24(22, 0)

     pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

     radio.begin()
     radio.enableDynamicPayloads()
     radio.setRetries(5,15)

     radio.openWritingPipe(pipes[0])
     radio.openReadingPipe(1,pipes[1])

     radio.printDetails()

     #radio.stopListening()
     radio.startListening()

     return radio

def loop_monitor():
     rc = 0
     while rc == 0:
          mqttc.loop()
          if radio.available():
               plen = radio.getDynamicPayloadSize()
               receive_payload = radio.read(plen)
               print 'Received ', receive_payload, ' len ',  plen
          else:     
               time.sleep(1/100)

     print("rc: " + str(rc))


radio = start_rf24()
mqttc = start_mosquitto()
loop_monitor()

