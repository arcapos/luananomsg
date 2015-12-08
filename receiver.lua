local nn = require 'nanomsg'

local s = nn.socket(nn.AF_SP, nn.SUB)

if s:setsockopt(nn.SUB, nn.SUB_SUBSCRIBE, 'foo') == false then
	print(nn.strerror())
end

local eid = s:connect('tcp://localhost:4711')

data, len = s:recv(1024)

print(len, data)


