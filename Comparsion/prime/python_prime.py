import math

def isPrime(number):
	if (number < 10):
		if (number == 0 or number == 1 or number == 9):
			return 0
		if (number == 2 or number == 3 or number == 5 or number == 7):
			return 1
	if (number % 2 == 0):
		return 0
	if (number % 5 == 0):
		return 0
	
	limit = math.sqrt(number)
	i = 3
	while ( i <= limit ):
		if ( number % i == 0):
			return 0
		i = i + 2
	
	return 1
	
match = 0
i = 0
while ( i < 5000011 ):
	if (isPrime(i)):
		match = match + 1
	i = i + 1
print(match)
