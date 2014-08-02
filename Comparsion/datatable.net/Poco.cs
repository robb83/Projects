using System;
using System.Collections.Generic;

namespace ExampleOfPoco
{	
	public class Item {
		public String Kategoria;
		public String Termekcsoport;
		public String Cikkszam;
		public String GyartoCikkszam;
		public String Termek;
		public String Gyarto;
		public String NettoAr;
		public String Deviza;
		public String AkciosAr;
		public String AkcioNeve;
		public String AkcioDatumig;
		public String ME;
		public String Elerhetoseg;
		public String Megjegyzes;
		public String Garancia;
		public String Empty;
	}

	class MainClass
	{

		public static void Main (string[] args)
		{
			if (args.Length < 1) {
				throw new InvalidOperationException ("Missing source data path argument");
			}

			List<Item> table = SimpleTSVLoader (args [0]);
			GC.Collect ();
			GC.WaitForPendingFinalizers ();
			Console.ReadLine ();
			Console.WriteLine ("Rows: {0}", table.Count);
		}

		static List<Item> SimpleTSVLoader(String filepath) {
			if (String.IsNullOrWhiteSpace (filepath))
				throw new ArgumentNullException ("filepath");

			if (!System.IO.File.Exists (filepath))
				throw new System.IO.FileNotFoundException (filepath);
			
			List<Item> result = new List<Item> ();

			String[] lines = System.IO.File.ReadAllLines (filepath);

			if (lines.Length > 1) {
				for (int i=1; i<lines.Length; i++) {
					String[] data = lines [i].Split ('\t');
					result.Add (CreateItem(data));
				}
			}

			return result;
		}

		static Item CreateItem(String[] data) {
			Item instance = new Item ();

			int index = 0;

			instance.Kategoria = data [index++];
			instance.Termekcsoport = data [index++];
			instance.Cikkszam = data [index++];
			instance.GyartoCikkszam = data [index++];
			instance.Termek = data [index++];
			instance.Gyarto = data [index++];
			instance.NettoAr = data [index++];
			instance.Deviza = data [index++];
			instance.AkciosAr = data [index++];
			instance.AkcioNeve = data [index++];
			instance.AkcioDatumig = data [index++];
			instance.ME = data [index++];
			instance.Elerhetoseg = data [index++];
			instance.Megjegyzes = data [index++];
			instance.Garancia = data [index++];
			instance.Empty = data [index++];

			return instance;
		}
	}
}
