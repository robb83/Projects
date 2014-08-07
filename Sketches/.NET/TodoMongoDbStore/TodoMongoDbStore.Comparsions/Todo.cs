using System;
using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace TodoMongoDbStore.Comparsions
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
		[BsonElement("hash")]
		public String Hash { get;set; }
		[BsonElement("salt")]
		public String Salt { get;set; }
	}
}

