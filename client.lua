local nn = require 'nanomsg'
local unix = require 'unix'

local s = nn.socket(nn.AF_SP, nn.NN_REQ)
if s == nil then
	print(nn.strerror())
end

local eid = s:connect('tcp://127.0.0.1:4712')

print(eid, unix.getpid())

while true do
	nsent = s:send('this is process ' .. unix.getpid())

	if nsent == nil then
		print(nn.strerror())
	else
		reply = s:recv(256)

		print(reply)
	end

	unix.sleep(1)
end

