using System;
using MongoDB.Driver;
using MongoDB.Driver.Builders;
using MongoDB.Bson;
using System.Diagnostics;

namespace TodoMongoDbStore.Comparsions
{
	public class EvalFindOne
	{
		MongoDatabase db;

		public EvalFindOne (MongoDatabase db)
		{
			this.db = db;
		}

		public void Find(int iteration) {

			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			for (int i=0; i < iteration; i++) {
				todos.FindOne (Query.EQ ("_id", ObjectId.Parse ("53e33773e59f82131aef12ab")));
			}

			watch.Stop ();
			Console.WriteLine ("Find({1}): {0}", watch.Elapsed, iteration);
		}

		public void Eval(int iteration) {
			var code = new BsonJavaScript ("function(key) { return db.todos.findOne({ _id: key }); }");

			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			for (int i=0; i < iteration; i++) {
				db.Eval (new EvalArgs { 
					Code = code, 
					Args = new BsonValue[] { ObjectId.Parse ("53e33773e59f82131aef12ab") }
				});
			}

			watch.Stop ();
			Console.WriteLine ("Eval({1}): {0}", watch.Elapsed, iteration);
		}
	}
}

