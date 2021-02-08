import paho.mqtt.subscribe as subscribe

msg = subscribe.simple("led/message", hostname="mesures.ludiksciences.fr")
print("%s %s" % (msg.topic, msg.payload))
