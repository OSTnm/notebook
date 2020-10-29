function getRandomNumber(maxvalue)
   math.randomseed(os.time())
   return math.random(maxvalue)
end

function handleGuesses(...)
   lm, hm, number = ...
   while true do
      answer = io.read("*n")
      if answer < number then
         print(lm)
      elseif answer > number then
         print(hm)
      else
         print("You got it")
         break
      end
   end
end

handleGuesses("Too low", "Too high", getRandomNumber(100))
