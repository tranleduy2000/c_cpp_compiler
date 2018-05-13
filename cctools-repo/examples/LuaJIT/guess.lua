secretword = "llama" -- the secret word

io.write ("What is your name? ")
name = io.read ("*l")

if name == "Randal" then
	print ("Hello, Randal! How good of you to be here!")
else
	print ("Hello " .. name .. "!") -- ordinary greeting
	io.write ("What is the secret word? ")
	guess = io.read ("*l")
	while guess ~= secretword do
		io.write ("Wrong, try again. What is the secret word? ")
		guess = io.read ("*l")
	end
end
