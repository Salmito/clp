local event=require'clp.event'

--marshal test
local a=event.encode("Test")
assert(event.decode(a)=="Test")
local clp=require'clp'

local sleep=clp.task(function(...)
	print("sleeping")
	for i=1,10 do
		event.sleep(1)
		print((10-i)..'s remaining')
	end
	print("done",...)
end)
sleep:push("event1")

local function handler(str,thread)
	local io=require'io'
	while true do
		if event.waitfd(1,event.READ,1) then
		   local a=io.stdin:read('*l')
		   print("Typed",a)
		else
		   print("Timed out :(");
      end
	end	
end

local stage=clp.task(handler,1,1)
stage:push('test',thread)
print('Type something in the next 10s')

event.sleep(10)
