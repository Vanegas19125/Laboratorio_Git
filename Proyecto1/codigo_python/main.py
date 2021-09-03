# This is a sample Python script.

# Press Mayús+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from serial import *
import time

def main():
    #iniciar comunicacion serial
    pic = Serial("COM4",19200)
    from Adafruit_IO import Client, RequestError, Feed
    ADAFRUIT_IO_KEY = "aio_wZHY00TIj6LBrcm95jxMQlvtnfVv"
    ADAFRUIT_IO_USERNAME = "JV19125"
    aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
    hall_feed = aio.feeds("hall")
    temp_feed = aio.feeds("temperatura")
    hum_feed = aio.feeds("humedad")

    while(True):

        valor = pic.readline()
        pic.flushInput()
        try:

            letra = valor.decode("ASCII")
            print(letra)
            #se separan los datos
            lista = letra.split(',')
            print(lista)
            if len(lista) == 3:

                aio.send_data(hall_feed.key, int(lista[0]))
                aio.send_data(temp_feed.key, int(lista[1]))
                aio.send_data(hum_feed.key, int(lista[2]))
                time.sleep(10)



        except:
            print("error de datos")





# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()
# See PyCharm help at https://www.jetbrains.com/help/pycharm/
