import paho.mqtt.client as mqtt #import the client1
broker_address="broker.emqx.io" 
#broker_address="iot.eclipse.org"
print("creating new instance")
client = mqtt.Client("P112345678") #create new instance
print("connecting to broker")
client.connect(broker_address) #connect to broker
print("Subscribing to topic","/ncnu/DataCollector/#")
client.subscribe("/ncnu/DataCollector/#")
print("Publishing message to topic","/ncnu/DataCollector/E89F6DE8F3BC")
client.publish("/ncnu/DataCollector/E89F6DE8F3BC","OFF")