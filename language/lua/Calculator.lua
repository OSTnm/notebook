function adder(x,y)
   return x + y
end

function suber(x,y)
   return x - y
end

function calculator(x,y, f)
   return f(x, y)
end

print(calculator(1,2,adder))
print(calculator(1,2,suber))
