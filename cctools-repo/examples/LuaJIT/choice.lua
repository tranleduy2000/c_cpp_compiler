io.write ("What is your name? ")
name = io.read("*l")

if name == "Randal" then
	print ("Hello, Randal! How good of you to be here!")
else
	print ("Hello " .. name .. "!") -- ordinary greeting
end
