local nn = require 'nanomsg'
local unix = require 'unix'

local s = nn.socket(nn.AF_SP, nn.PUB)
local eid = s:bind('tcp://127.0.0.1:4711')

print(eid)

while true do
nsent = s:send('foo|Hello')

if nsent == nil then
	print(nn.strerror())
else
	print('sent ' .. nsent .. ' bytes')
end

	unix.sleep(1)
end

