local meta = {}
local SS = {}

function SS.new(s)
   local superString =  {}
   superString.s = s
   setmetatable(superString, meta)
   return superString
end

function SS.add(s1, s2)
   s1.s = s1.s .. " " .. s2.s
   return s1
end

function SS.index(s, key)
   print(key)
   if key == "reverse" then
      return string.reverse(s.s)
   end
end

meta.__add = SS.add
meta.__index = SS.index

firstName = SS.new("Bill")
firstName = firstName + SS.new("Gilr")
print(firstName.s)
print(firstName.reverse)
