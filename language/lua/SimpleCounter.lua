function simpleCounter(max)
   local counter = 0
   return function()
      counter = counter + 1
      if counter > max then
         return nil
      end
      return counter
   end
end

for v in simpleCounter(50) do
   print(v)
end
