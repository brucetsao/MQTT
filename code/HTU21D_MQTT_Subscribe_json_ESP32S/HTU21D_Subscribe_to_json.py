import matplotlib.pyplot as plt
import sys	#作業系統套件，用於檔案、目錄資料使用
import requests	#建立雲端WinSocket連線的套件
import json	#了解json內容的json物件的套件
import time	#系統時間套件
import datetime	#時鐘物件
import time
import paho.mqtt.client as paho
import unicodedata	#Unicode
#define callback
def on_message(client, userdata, message):
    time.sleep(1)
    print(client)
    print(userdata)
    table=json.loads(message.payload.decode("utf-8")) 
    print(json.dumps(table, sort_keys=True, indent=4)) 


brokerurl="broker.emqx.io"
toipc1="/ncnu/DataCollector/%s"
mac = input("請您輸入查詢裝置網路卡編號(MAC Address):")
toipcstr= toipc1 % (mac)  #將輸入資料：MAC,變成MQTT Broker 網址，整合再一起，
print(brokerurl)
print(toipcstr)

client= paho.Client("bruce %s" % (mac)) #create client object client1.on_publish = on_publish #assign function to callback client1.connect(broker,port) #establish connection client1.publish("house/bulb1","on")
######Bind function to callback
client.on_message=on_message

#MAC=E89F6DE8F54C
#    30C6F704554C
print("connecting to broker ",brokerurl)
client.connect(brokerurl)#connect
client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe(toipcstr)#subscribe
while 1:
    time.sleep(1)

client.disconnect() #disconnect
client.loop_stop() #stop loop


