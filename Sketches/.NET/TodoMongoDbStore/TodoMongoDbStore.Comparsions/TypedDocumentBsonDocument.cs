using System;
using MongoDB.Driver;
using System.Diagnostics;
using MongoDB.Bson;
using MongoDB.Driver.Builders;

namespace TodoMongoDbStore.Comparsions
{
	public class TypedDocumentBsonDocument
	{
		MongoDatabase db;

		public TypedDocumentBsonDocument (MongoDatabase db)
		{
			this.db = db;
		}

		public void TypedDocumentTest(int iteration) {

			MongoCollection<Todo> todos = db.GetCollection <Todo>("todos");

			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			int counter = 0;
			for (int i=0; i < iteration; i++) {
				var cursor = todos.FindAll ();

				foreach (var c in cursor) {
					++counter;
				}
			}

			watch.Stop ();
			Console.WriteLine ("TypedDocumentTest({1}): {0}", watch.Elapsed, iteration, counter);
		}

		public void BsonDocumentTest(int iteration) {
			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			int counter = 0;
			for (int i=0; i < iteration; i++) {
				var cursor = todos.FindAll ();

				foreach (var c in cursor) {
					++counter;
				}
			}

			watch.Stop ();
			Console.WriteLine ("BsonDocumentTest({1}): {0}", watch.Elapsed, iteration, counter);
		}
	}
}

