local clp=require "clp"

--clp.pool:add(clp.cpus())

local pool=clp.pool
local arg={...}
pool:add(arg[3] or clp.cpus()-1)
local n=tonumber(arg[1]) or 100
local it=tonumber(arg[2]) or 100000

local bench=function () return {
	start=function(self)
		self.t=clp.now()
		self.c=os.clock()
	end,
	stop=function(self)
		return clp.now()-self.t,os.clock()-self.c
	end,
	clock=function(self,str)
		local t,c=self:stop()
		print(string.format('%s\t%.3fs\t%.3fs',str,t,c))
	end
} end

local startup=bench()

bench=bench()
bench:start()
startup:start()

local finish=clp.channel()

local s2=clp.task(function()
	i=(i or 0) +1
	if i==n then
		print('finish')
		finish:push('end')
	end 
end)

local function f()
  local n=0
  for i=1,it do
    n=(n+i)
  end
  s2:push()
end

local s={}

local str='\t'..n..'\t'..it..'\t'


--local out=clp.channel()
local s1=clp.task(f,n)

for i=1,n do
	s1:push()
end
startup:clock('start_time'..str)

--bench:clock('start_time'..str)
--bench:start()

finish:get()
bench:clock('parallel'..str)
bench:start()
for i=0,n do
  f()
end
bench:clock('pure_lua'..str)
