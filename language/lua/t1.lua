guesses = {1, 2}

guesses[#guesses + 2] = 3

for i = 1, #guesses do
   print(guesses[i])
end

for name, val in pairs(guesses) do
   print(name, val)
end
