local nn = require 'nanomsg'
local unix = require 'unix'

local s = nn.socket(nn.AF_SP, nn.NN_REP)
local eid = s:bind('tcp://127.0.0.1:4712')

while true do
	data, len = s:recv(256)

	print('got ' .. data)

	s:send('ack')
end

