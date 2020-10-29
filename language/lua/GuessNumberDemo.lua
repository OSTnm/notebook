print("Guess a number")
math.randomseed(os.time())
number = math.random(100)

while true do
   answer = io.read("*n")
   if answer < number then
      print("Too low")
   elseif answer > number then
      print("Too high")
   else
      print("You got it")
      break
   end
end
