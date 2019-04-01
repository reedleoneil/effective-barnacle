require 'sinatra'
require 'rubyserial'

`rm red`
`touch red`
serialport = Serial.new '/dev/ttyACM0'

get '/local' do
	headers 'Access-Control-Allow-Origin' => '*'
	if @server != "" then
		Thread.new { `nohup ruby ~/local.rb &` }
		`echo local >> red`
	end
	"local"
end

get '/online' do
	headers 'Access-Control-Allow-Origin' => '*'
	if @server != "" then
		Thread.new { `nohup ruby ~/online.rb &` }
		`echo online >> red`
	end
	"online"
end

get '/' do
	headers 'Access-Control-Allow-Origin' => '*'
	`cat red`
end

