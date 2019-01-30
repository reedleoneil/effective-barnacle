require 'paho-mqtt'
require 'open3'
require 'rubyserial'

client = PahoMqtt::Client.new()
serialport = Serial.new 'COM5'

client.on_message do |p|
	puts "Topic: #{p.topic}\nPayload: #{p.payload}\nQoS: #{p.qos}"
	serialport.write(p.payload)
end

client.connect('iot.eclipse.org', 1883, client.keep_alive, true, true)
client.subscribe(["diana/controller", 2])

Thread.new {
	loop do
		payload = "{ \"powerStatus\": #{rand(0...2)}, \"waterLevel\": #{rand(0...600)} }"
		client.publish("diana/status", payload, false, 1)
		sleep 2
	end
}

loop do
  client.loop_write
  client.loop_read
end
