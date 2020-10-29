local clock = os.clock
function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end

Skill = {pri = 1, name = '', coolDown = 0, cd = 0}

function Skill:new(t)
   t = t or {}
   setmetatable(t, self)
   self.__index = self
   return t
end

function Skill:coolingdown(s)
   self.cd = self.cd - s
   if self.cd  < 0 then
      self.cd = 0
   end
end

function Skill:iscollingdown()
   if self.cd ~= 0 then
      return true
   end
   return false
end

function Skill:getpri()
   return self.pri
end

function Skill:getname()
   return self.name
end

function Skill:cast()
   self.cd = self.coolDown
   -- return (" - pri:" .. self.pri .. " cast:" .. self.name)
   return (" cast:" .. self.name)
end

SkillEx = Skill:new()

function SkillEx:iscollingdown()
   if self.cd ~= 0 and self.number <= 0 then
      return true
   end
   return false
end

function SkillEx:cast()
   if self.number <= 0 then
      print(self.name .. " wrong")
   end

   self.number = self.number - 1
   self.cd = self.coolDown
   -- str = " - pri:" .. self.pri .. " cast:" .. self.name
   str = " cast:" .. self.name
   self.pri = 4
   return str
end

function SkillEx:coolingdown(s)
   self.cd = self.cd - s
   if self.cd  < 0 then
      self.cd = 0
      self.number = self.number + 1
      if self.number > 2 then
         self.number = 2
      end
   end
   if self.cd <= 5 then
      self.pri = 2
   end
end

SkillLowPri = Skill:new()
function SkillLowPri:cast()
   -- str = " - pri:" .. self.pri .. " cast:" .. self.name
   str = " cast:" .. self.name
   if self.pri == 4 then
      self.pri = 5
   else
      self.pri = 4
   end
   return str
end

Skills1 = {}
Skills2 = {}
Skills3 = {}
Skills4 = {}
Skills5 = {}

Pris = {}
Pris[1] = Skills1
Pris[2] = Skills2
Pris[3] = Skills3
Pris[4] = Skills4
Pris[5] = Skills5

Skills1['野性守护'] = Skill:new({name = '野性守护', coolDown = 120})
Skills1['夺命黑鸦'] = Skill:new({name = '夺命黑鸦', coolDown = 60})
Skills1['狂野怒火'] = Skill:new({name = '狂野怒火', coolDown = 90})
Skills2['倒刺射击'] = SkillEx:new({name = '倒刺射击', coolDown = 11, number = 2, pri = 2})
Skills3['杀戮命令'] = Skill:new({name = '杀戮命令', coolDown = 6.7, pri = 3})
Skills4['眼镜蛇射击'] = SkillLowPri:new({name = '眼镜蛇射击', coolDown = 0, pri = 4})

local GCD = 1.6

function adjust()
   local tmp = {}
   for idx, Pri in ipairs(Pris) do
      tmp[idx] = {}
      for name, val in pairs(Pri) do
         tmp[idx][name] = 0
      end
   end

   for idx, Pri in ipairs(Pris) do
      for name, val in pairs(Pri) do
         local p = val:getpri()
         if p ~= idx then
            tmp[idx][name] = p
         end
      end
   end

   for idx, t in ipairs(tmp) do
      for name, val in pairs(t) do
         if val ~= 0 then
            local skill = Pris[idx][name]
            Pris[idx][name] = nil
            Pris[val][name] = skill
         end
      end
   end
end

function cast(time)
   for idx, Pri in ipairs(Pris) do
      for name, val in pairs(Pri) do
         if val:iscollingdown() ~= true then
            -- print(time .. ":" .. val:cast())
            print(val:cast())
            return
         end
      end
   end
end

function coolingdown(time)
   for idx, Pri in ipairs(Pris) do
      for name, val in pairs(Pri) do
         val:coolingdown(time)
      end
   end
end

local globaltime = 0

while true do
   cast(globaltime)
   adjust()
   coolingdown(GCD)
   adjust()
   sleep(GCD)
   globaltime = globaltime + GCD
end
