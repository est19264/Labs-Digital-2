# Código Laboratorio 5 Adafruit IO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Diego Estrada - 19264

# Adafruit IO
# https://io.adafruit.com/

import serial
from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_PaRS857e955ajmvlSlSMUTpe3uCo"
ADAFRUIT_IO_USERNAME = "diego_estrada"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

#sensor1 = [15, 25, 13, 10, 11]
#sensor2 = [1, 3, 4, 6, 8, 12]

#for i in range(0, len(sensor1)):
    #send1 = sensor1[i]
#Sensor 01 Feed
digital_feed = aio.feeds('sensor1') # Se selecciona el feed al que se le manda datos 
aio.send_data(digital_feed.key, 200)  # Se manda el dato deseado en el segundo lugar
digital_data = aio.receive(digital_feed.key)    # esta parte es para recibir datos
print(f'digital signal: {digital_data.value}')

#for a in range(0, len(sensor2)):
 #   send2 = sensor2[a]
#Sensor 02 Feed
analog_feed = aio.feeds('sensor2')
aio.send_data(analog_feed.key, 85)
analog_data = aio.receive(analog_feed.key)
print(f'analog signal: {analog_data.value}')

# Read data Feed
send_feed = aio.feeds('send')
#aio.send_data(send_feed.key, 25)
send_data = aio.receive(send_feed.key)
print(f'send signal: {send_data.value}')

serialpic = serial.Serial("COM3",9600)
