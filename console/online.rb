require 'paho-mqtt'
require 'open3'
require 'rubyserial'

client = PahoMqtt::Client.new()
serialport = Serial.new '/dev/ttyACM0'

client.on_message do |p|
	puts "Topic: #{p.topic}\nPayload: #{p.payload}\nQoS: #{p.qos}"
	serialport.write(p.payload)
end

client.connect('iot.eclipse.org', 1883, client.keep_alive, true, true)
#client.connect('localhost', 1883, client.keep_alive, true, true)
client.subscribe(["diana/controller", 2])

Thread.new {
	loop do
		serialport.write 'q'
		x = ''
		payload = "{"
		while x != '{' do
			x = serialport.read 1
		end
		while x != '}' do
			x = serialport.read 1
			if x != '' then payload << x end
		end
		puts payload
		client.publish("diana/status", payload, false, 1)
		sleep 1
	end
}

loop do
  client.loop_write
  client.loop_read
end

