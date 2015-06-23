#!/usr/bin/env python

import time
import sys
from RF24 import *

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

while not radio.available():
     print 'No data'
     time.sleep(1/10)

plen = radio.getDynamicPayloadSize()
receive_payload = radio.read(plen)
print 'Received ', receive_payload, ' len ',  plen

radio.stopListening()

data = 'off'

if len(sys.argv) > 1:
     data = sys.argv[1]

print 'Sending ', data
radio.write(data)

