function findMax(...)
   local arguments = {...}
   local max = ...              -- only have the very first one
   for i = 1, #arguments do
      if arguments[i] > max then
         max = arguments[i]
      end
   end
   return max
end

print(findMax(1,2,3,3,4,4,9))
