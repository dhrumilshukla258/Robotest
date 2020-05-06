x = 0

function sentinelVelocity( deltaTime2 )
  x = x + 0.0005
  return x
end

--[[
deltaTime = 0.2
localtime = 0
maxtime = 2.0
velocity = 0.05

function sentinelVelocity( deltaTime2 )
  localtime = localtime + deltaTime2
  if localtime > maxtime then  
    localtime = 0 
    velocity = -velocity
   --print("Direction Change")
    
  end
 -- print (localtime)
  return velocity
end


--for i=0, 20, 1 do
 -- sentinelVelocity(0.1)
--end
]]--