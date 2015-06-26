#from mosquitto import Mosquitto as mq
#import paho.mqtt.client as mosquitto
import mosquitto

def on_connect(obj, rc):
          print("rc: " + str(rc))

def on_message(obj, msg):
	print(msg.topic + " " + str(msg.payload))
	mqttc.publish("home/sala/luz", "ON")

def on_publish(obj, mid):
          print("mid: " + str(mid))

def on_subscribe(obj, mid, granted_qos):
     print("Suscribed: " + str(mid) + " " + str(granted_qos))

def on_log(obj, level, string):
     print(string)

mqttc = mosquitto.Mosquitto("mqtt")

mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

mqttc.on_log = on_log

mqttc.connect("localhost", 1883, 60)

mqttc.subscribe("home/sala/luz", 0)

rc = 0
while rc == 0:
     mqttc.loop()

print("rc: " + str(rc))
