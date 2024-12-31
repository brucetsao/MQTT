import matplotlib.pyplot as plt
import sys	#作業系統套件，用於檔案、目錄資料使用
import requests	#建立雲端WinSocket連線的套件
import json	#了解json內容的json物件的套件
#import time	#系統時間套件
from datetime import datetime	#時鐘物件
import time
import paho.mqtt.client as paho
import unicodedata	#Unicode
plt.rcParams["font.family"] = ["Microsoft JhengHei"]
tt = "%s:%s:%s"
#define callback
def on_message(client, userdata, message):
    currentDateAndTime = datetime.now()
    table=json.loads(message.payload.decode("utf-8")) 
    print(json.dumps(table, sort_keys=True, indent=4)) 
    xdata=[]
    y1data=[]
    y2data=[]
    cnt=1
    #---------------------
    s01 = table['Device']  
    s02 = table['Temperature'] 
    s03 = table['Humidity'] 
    for x in s02:
        y1data.append(float(x))
        xdata.append(cnt)
        cnt=cnt+1
    for x in s03:
        y2data.append(float(x))
    #print(xdata)
    #print(y1data)
    #print(y2data)
    tt1 = tt % (currentDateAndTime.hour,currentDateAndTime.minute,currentDateAndTime.second)
    plt.title("溫濕度感測器視覺化圖表(%s)" % (tt1), fontsize=24)
    plt.plot(xdata, y1data, color='c',label='Temperature')          # 設定青色cyan            
    plt.plot(xdata, y2data, color='r',label='Humidity')          # 設定紅色red
    plt.legend()
    plt.show()

    #---------------------


brokerurl="broker.emqx.io"
toipc1="/ncnu/DataCollector/%s"
xdata=[]
y1data=[]
y2data=[]
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


