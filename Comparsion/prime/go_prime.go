package main

import (
	"fmt"
	"math"
)

func isPrime(number int) bool {
	
	if number < 10 {
		
		if number == 0 || number == 1 || number == 9 {
			return false
		}
		
		if number == 2 || number == 3 || number == 5 || number == 7 {
			return true
		}
	}
	
	if number % 2 == 0 {
		return false
	}
	
	if number % 5 == 0 {
		return false
	}
	
	var limit int = int(math.Sqrt(float64(number)))
	for i := 3; i <= limit; i += 2 {
		if (number % i) == 0 {
			return false
		}
	}
	
	return true
}

func main() {
	
	var match int = 0
	
	for i:= 0; i < 5000011; i++ {
		if isPrime(i) {
			match++
		}
	}
	
	fmt.Println(match)
}
