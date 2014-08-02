using System;
using System.Data;

namespace ExampleOfDataTable
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			if (args.Length < 1) {
				throw new InvalidOperationException ("Missing source data path argument");
			}

			DataTable table = SimpleTSVLoader (args [0]);
			GC.Collect ();
			GC.WaitForPendingFinalizers ();
			Console.ReadLine ();
			Console.WriteLine ("Rows: {0}", table.Rows.Count);
		}

		static DataTable SimpleTSVLoader(String filepath) {
			if (String.IsNullOrWhiteSpace (filepath))
				throw new ArgumentNullException ("filepath");

			if (!System.IO.File.Exists (filepath))
				throw new System.IO.FileNotFoundException (filepath);
			
			DataTable result = new DataTable ();

			String[] lines = System.IO.File.ReadAllLines (filepath);

			if (lines.Length > 0) {
				CreateColumns (result, lines [0].Split ('\t'));

				if (lines.Length > 1) {
					for (int i=1; i<lines.Length; i++) {
						String[] data = lines [i].Split ('\t');
						result.Rows.Add (data);
					}
				}
			}

			return result;
		}

		static void CreateColumns(DataTable table, String[] columnNames) {
			foreach (var columnName in columnNames) {
				table.Columns.Add (columnName);
			}
		}
	}
}
