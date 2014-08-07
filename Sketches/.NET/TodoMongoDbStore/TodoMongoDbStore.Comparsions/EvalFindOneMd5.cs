using System;
using MongoDB.Driver;
using MongoDB.Bson;
using System.Diagnostics;
using MongoDB.Driver.Builders;

namespace TodoMongoDbStore.Comparsions
{
	public class EvalFindOneMd5
	{
		MongoDatabase db;

		public EvalFindOneMd5 (MongoDatabase db)
		{
			this.db = db;
		}

		public void Find(int iteration, String id, String content) {

			MongoCollection<BsonDocument> todos = db.GetCollection ("todos");

			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			for (int i=0; i < iteration; i++) {
				ObjectId objectID = ObjectId.Parse (id);

				BsonDocument todo = todos.FindOne (Query.EQ ("_id", objectID));
				if (todo != null) {

					var hash = MainClass.CreateMD5String(content + ":" + todo.GetValue ("salt"));
					todos.FindOne (Query.And (
						Query.EQ ("_id", objectID),
						Query.EQ ("hash", hash)));
				}
			}

			watch.Stop ();
			Console.WriteLine ("Find({1}): {0}", watch.Elapsed, iteration);
		}

		public void Eval(int iteration, String id, String content) {
			var code = new BsonJavaScript (@"
function(key, content) { 
	var todo = db.todos.findOne({ _id: key });
	if (todo) {
		var hash = hex_md5(content + ':' + todo.salt);
		return db.todos.findOne({ _id: key, hash: hash });
	}

	return null;
}");
			Stopwatch watch = new Stopwatch ();
			watch.Start ();

			for (int i=0; i < iteration; i++) {
				
				ObjectId objectID = ObjectId.Parse (id);
				var args = new BsonValue[] { objectID, content };

				db.Eval (new EvalArgs { 
					Code = code, 
					Args = args
				});
			}

			watch.Stop ();
			Console.WriteLine ("Eval({1}): {0}", watch.Elapsed, iteration);
		}
	}
}

