
import paho.mqtt.client as mqtt
# 使用 paho 套件，將 paho.mqtt.client 套件下連線的物件 import 進來並更名為 mqtt
import json #將json套件仔入
from commlib import *

#------------------------------
import pymysql as DB
# 將 pymysql 套件匯入並更名為 DB，以便後續使用
#-----------------------

# 資料庫連線設定
# 使用 DB.connect() 方法建立與資料庫的連線
# 參數包括主機名、連接埠、使用者名稱、密碼、資料庫名稱及字符集
db = DB.connect(host='localhost', port=3306, user='big', passwd='12345678', db='big', charset='utf8')
# 建立操作游標，使用 db.cursor() 方法獲取一個操作游標，用於執行 SQL 語句
cursor = db.cursor()
# 定義 SQL 語法（插入資料）， 將資料插入到 dhtdata 資料表中
sql1 = "INSERT INTO dhtdata (id, MAC, crtdatetime, temperature, humidity, systime) VALUES (NULL, 'a01110001', current_timestamp(), '25.3', '88.9', '20220624220101');"

#----------------------------------------------------------
# 設定 MQTT Broker 伺服器詳細資訊
broker_address = "broker.emqx.io"  # 設定 MQTT Broker 伺服器網址
port = 1883  # 設定 MQTT Broker 伺服器通訊埠
username = ""  # 設定 MQTT Broker 伺服器登錄使用者名稱
password = ""  # 設定 MQTT Broker 伺服器登錄使用者密碼
topic = "/nuk/sensor/"  # 訂閱 MQTT Broker 伺服器主題

# MQTT get published data as following
# {
#   "Device": "E89F6DE8F3BC",
#   "Temperature": 24,
#   "Humidity": 77
# }

sqlstr0 = "INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES ('%s', '%s', %f, %f, '%s');"

# 定義連接回調函數
def on_connect(client, userdata, flags, rc):
    # MQTT 連接處理程序
    print("Connected with result code " + str(rc))
    # 訂閱主題
    client.subscribe(topic)  # 訂閱指定的主題

# 定義訂閱回調函數
def on_message(client, userdata, msg):
    # 訂閱處理程序
    print("Data coming from "+msg.topic)  # 打印接收到的主題和消息內容
    # jsonStr = json.dumps(msg.payload, ensure_ascii=False, indent=4)
    print("Payload is :\n"+str(msg.payload))  # 打印接收到的主題和消息內容
    payload = msg.payload.decode("utf-8")
    jsondata = json.loads(payload)
    jsonStr = json.dumps(jsondata, ensure_ascii=False, indent=4)
    print(jsonStr)  # 印出jsonStr
    print("Devce:",jsondata["Device"])
    print("Temperature:",jsondata["Temperature"])
    print("Humidity:",jsondata["Humidity"])
    sqlstr = sqlstr0 % (jsondata["Device"],get_local_ip(),float(jsondata["Temperature"]),float(jsondata["Humidity"]),getsystime())
    print(sqlstr)
    # 執行 SQL 語法
    # 使用 cursor.execute() 方法執行定義的 SQL 語句
    cursor.execute(sqlstr)

# 建立 MQTT 客戶端物件
client = mqtt.Client()

# 設定帳戶名稱和密碼
client.username_pw_set(username, password)

# 設定伺服器連線的回調函數和訂閱的回調函數
client.on_connect = on_connect  # 設定伺服器連線的回調函數
client.on_message = on_message  # 設定訂閱的回調函數

# 連接到 MQTT Broker 伺服器
client.connect(broker_address, port, 60)
# client.connect(broker_address, port, keepalive=60) 各個參數的意義如下：
# broker_address：需要連接的 MQTT broker 伺服器的地址，可以是 IP 地址或主機名稱。
# port：需要連接的 MQTT broker 伺服器的通訊埠，預設值為 1883，可以是其他通訊埠。
# keepalive：保持活動狀態的時間間隔（秒），預設為 60 秒。在這個時間內，如果客戶端沒有向 MQTT broker 伺服器發送任何消息，
# broker 伺服器會向客戶端發送一個 PING 消息來維持連接狀態。如果 keepalive 設置為 0，表示不啟用伺服器保持活動狀態功能。
# 簡單來說，這行程式碼的作用是使用指定的地址和端口，建立與 MQTT broker 的連接，並設置 60 秒的 keepalive 時間間隔。
# 這個連接可以用於發布和訂閱 MQTT 消息，並在完成後使用 client.disconnect() 方法關閉連接。

# 進入循環模式，等待 MQTT Broker 伺服器針對主題傳送之訊息後，接收訊息
client.loop_forever()