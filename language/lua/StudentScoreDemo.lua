local grades = {Mary = "100" }
while true do
   print "Enter stu name (q to quit)"
   local name = io.read()
   if name == "q" then
      break
   end
   print "Enter stu score (q to quit)"
   local score = io.read("*n", "*l")
   grades[name] = score
end

for name, val in pairs(grades) do
   print(name .. "  " .. val)
end

-- local arr = {1,2,3,4}

-- for i = 0, #arr do
--    print(arr[i])
-- end

print(#grades)
for i = 0, #grades do
   print(grades[i])
end

