CalorieCounter = {count = 0, goal = 2000}

function CalorieCounter:add(amount)
   self.count = self.count + amount
end

function CalorieCounter:del(amount)
   self.count = self.count - amount
end

function CalorieCounter:didReachGoal()
   return self.count == self.goal
end

function CalorieCounter:new(t)
   t = t or {}
   setmetatable(t, self)
   self.__index = self
   -- self.__newindex = self
   return t
end

-- c = CalorieCounter:new({goal = 1500})

-- print(c:didReachGoal())
-- c:add(1500)
-- print(c:didReachGoal())

-- d = CalorieCounter:new({goal = 1500})
-- print(d.count)

HealthMonitor = CalorieCounter:new({steps = 0, stepGoal = 10000})

function HealthMonitor:step(amount)
   self.steps = self.steps + amount
end

function HealthMonitor:didRead()
   local goalsReached = 0
   if self.count >= self.goal then
      print("Calorie goal of " .. self.count .. "reached")
      goalsReached = goalsReached + 1
   else
      print("Calorie goal of not reached yet")
   end

   if self.steps >= self.stepGoal then
      print("Step goal of " .. self.stepGoal .. "reached")
   else
      print("Step goal of not reached yet")
      goalsReached = goalsReached + 1
   end

   return goalsReached >= 2
end

-- hm = HealthMonitor:new({goal = 1500, stepGoal = 5000})
-- hm:add(900)
-- hm:step(5001)
-- print(hm:didReachGoal())
-- hm:add(600)
-- print(hm:didReachGoal())

return {
   CalorieCounter = CalorieCounter,
   HealthMonitor = HealthMonitor
}
