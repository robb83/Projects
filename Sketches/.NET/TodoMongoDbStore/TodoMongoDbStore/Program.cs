using System;
using MongoDB.Driver;
using MongoDB.Bson;
using System.Collections.Generic;
using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using System.IO;
using MongoDB.Bson.Serialization.Attributes;

namespace TodoMongoDbStore
{
	class Todo {
		[BsonId]
		public ObjectId ID { get;set; }
		[BsonElement("created")]
		public DateTime Created { get;set; }
		[BsonElement("status")]
		public bool Status { get;set; }
		[BsonElement("description")]
		public String Description { get;set; }
		[BsonElement("visited")]
		public long Visited { get;set; }
	}

	class MainClass
	{
		static MongoDatabase db;

		static void InsertTodos() {
			WriteConcernResult writeResult;
			IEnumerable<WriteConcernResult> writeResults;
			BsonDocument todo;
			ObjectId objectId;

			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			todo = new BsonDocument ();
			objectId = ObjectId.GenerateNewId ();
			todo.Add ("_id", objectId);
			todo.Add ("created", DateTime.UtcNow);
			todo.Add ("status", false);
			todo.Add ("description", "Make homework");
			todo.Add ("visited", 0);

			// not ordered, not transactional, not write safe
			writeResult = todos.Insert (todo);

			if (!writeResult.Ok) {
				Console.WriteLine (writeResult.ErrorMessage);
				return;
			}

			// http://en.wikipedia.org/wiki/Algorithm_examples
			String[] steps = new String[] {
				"Clear contents of A-register to Zero",
				"Increment A: Add one to the contents of A-register",
				"Increment A: Add one to the contents of A-register",
				"Increment A: Add one to the contents of A-register",
				"Copy contents of A to N",
				"Load register A with contents of register x whose address is in contained in N",
				"Increment A: Add one to the contents of A-register",
				"Store contents of A-register in register whose address is contained in N"
			};

			List<BsonDocument> newTodos = new List<BsonDocument> ();

			foreach (var s in steps) {
				// id generated by server
				todo = new BsonDocument ();
				todo.Add ("created", DateTime.UtcNow);
				todo.Add ("status", false);
				todo.Add ("description", s);
				todo.Add ("visited", 0);
				newTodos.Add (todo);
			}

			// not ordered, not transactional, not write safe
			writeResults = todos.InsertBatch (newTodos);
			foreach (var wr in writeResults) {
				if (!wr.Ok) {
					Console.WriteLine ("WriteFailed!");
				}
			}

			// error while insert
			try {
				todo = new BsonDocument ();
				todo.Add ("_id", objectId);
				todo.Add ("created", DateTime.UtcNow);
				todo.Add ("status", false);
				todo.Add ("description", "Make homework");
				todo.Add ("visited", 0);

				// not ordered, not transactional, not write safe
				writeResult = todos.Insert (todo);
			} catch ( MongoDuplicateKeyException) {
				// handle this
			}
		}

		static void SelectTodos() {
			MongoCursor cursor;
			MongoCursor<Todo> cursorOfTodos;
			IMongoQuery filter;
			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			// select all
			cursor = todos.FindAll ();

			Console.WriteLine ("Total count: {0}", cursor.Count ());
			foreach (BsonDocument doc in cursor) {
				Console.WriteLine ("ID: {0}", doc.GetValue ("_id"));
				Console.WriteLine ("Description: {0}", doc.GetValue ("description"));
				Console.WriteLine ("Created: {0}", doc.GetValue ("created"));
				Console.WriteLine ();
			}
			
			// select by criteria and set order
			filter = Query.EQ ("description", "Make homework");
			cursor = todos.Find (filter).SetSortOrder("created");

			Console.WriteLine ("Total count: {0}", cursor.Count ());
			foreach (BsonDocument doc in cursor) {
				Console.WriteLine ("ID: {0}", doc.GetValue ("_id"));
				Console.WriteLine ("Description: {0}", doc.GetValue ("description"));
				Console.WriteLine ("Created: {0}", doc.GetValue ("created"));
				Console.WriteLine ();
			}

			// typed select
			cursorOfTodos = todos.FindAllAs<Todo> ();

			foreach (Todo doc in cursorOfTodos) {
				Console.WriteLine ("ID: {0}", doc.ID);
				Console.WriteLine ("Description: {0}", doc.Description);
				Console.WriteLine ("Created: {0}", doc.Created);
				Console.WriteLine ();
			}
		}

		static void UpdateTodos() {
			IMongoQuery filter;
			MongoCursor cursor;
			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			// increment visited field
			filter = Query.EQ ("description", "Make homework");
			todos.Update (filter, Update.Inc ("visited", 1), UpdateFlags.Multi);

			cursor = todos.Find (filter);
			foreach (BsonDocument doc in cursor) {
				Console.WriteLine ("ID: {0}", doc.GetValue ("_id"));
				Console.WriteLine ("Visited: {0}", doc.GetValue ("visited"));
			}
		}

		static void Eval() {
			BsonValue result;

			result = db.Eval (new EvalArgs {
				Code = new BsonJavaScript("function() { return db.todos.find().count(); }")
			});

			Console.WriteLine ("Total count: {0}", result);
		}

		static void GridFS() {
			MongoCursor<MongoGridFSFileInfo> cursor;
			MongoGridFS fs = db.GetGridFS (new MongoGridFSSettings ());

			// create file
			using (MongoGridFSStream stream = fs.Create ("file1.txt")) {
				using (StreamWriter writer = new StreamWriter(stream)) {
					writer.WriteLine ("Hello World!");
				}
			}

			// read file
			using (StreamReader reader = fs.OpenText("file1.txt")) {
				Console.WriteLine("Content: {0}", reader.ReadLine());
			}

			// list files
			cursor = fs.FindAll ();
			foreach (var fi in cursor) {
				Console.WriteLine ("Filename: {0}, Id: {1}, MD5: {2}, Length: {3}", fi.Name, fi.Id, fi.MD5, fi.Length);
			}

			// upload file

			// delete file
			if (fs.Exists ("file1.txt")) {
				fs.Delete ("file1.txt");
			}
		}

		public static void Main (string[] args)
		{
			var client = new MongoClient ("mongodb://localhost:27017/todos");
			var server = client.GetServer ();
			db = server.GetDatabase ("todos");

			if (db == null) {
				Console.WriteLine ("Connection failed.");
				return;
			}

			if (!db.CollectionExists ("todos")) {
				var todoCollection = db.GetCollection ("todos");
				// quick order by date
				todoCollection.CreateIndex ("created");
				// enable fulltext search
				todoCollection.CreateIndex (IndexKeys.Text ("description"));
			}

			InsertTodos ();
			SelectTodos();
			UpdateTodos ();
			Eval ();
			GridFS ();
		}
	}
}