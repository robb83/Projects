using System;

namespace Prime {
	class Program {
		
		static bool IsPrime(int number) {
			if (number < 10) {
				
				if (number == 0 || number == 1 || number == 9) 
					return false;
				if (number == 2 || number == 3 || number == 5 || number == 7)
					return true;
			}
			
			if (number % 2 == 0) return false;
			if (number % 5 == 0) return false;
			
			int limit = (int)Math.Sqrt(number);
			for( int i = 3; i <= limit; i += 2) {
				if ((number % i) == 0) {
					return false;
				}
			}
			
			return true;
		}
		
		static void Main(String[] args) {
			int match = 0;
			for( int i = 0;  i < 5000011; i++) {
				if (IsPrime(i)) ++match;
			}

			Console.WriteLine(match.ToString());
		}
	}
}

