local Health = require "CalorieCounterDemo"

local HealthMonitor = Health.HealthMonitor

hm = HealthMonitor:new({goal = 1500, stepGoal = 5000})
hm:add(900)
hm:step(5001)
print(hm:didReachGoal())
hm:add(600)
print(hm:didReachGoal())
