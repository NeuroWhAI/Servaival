using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Core;

namespace RankManager
{
    class Ranking
    {
        public string Name
        { get; set; } = string.Empty;

        public int Score
        { get; set; } = 0;
    }


    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                string dbUri = string.Empty;
                string name = string.Empty;
                int score = 0;


                if (args.Length == 5)
                {
                    if (args[0] != "NeuroWhAI")
                        return;

                    Directory.SetCurrentDirectory(args[1]);

                    dbUri = args[2];

                    name = args[3].Trim();
                    score = int.Parse(args[4].Trim());
                }
                else
                {
                    return;
                }


                MongoClient client = null;
                IMongoDatabase db = null;

                client = new MongoClient(dbUri);
                int slashIdx = dbUri.LastIndexOf('/');
                db = client.GetDatabase(dbUri.Substring(slashIdx + 1));


                var col = db.GetCollection<BsonDocument>("ranking");
                var docs = col.Find(Builders<BsonDocument>.Filter.Empty).ToList();

                var rankList = new List<Ranking>();

                BsonDocument minDoc = docs.First();
                string minName = minDoc["name"].AsString;
                int minScore = minDoc["score"].AsInt32;

                foreach (var doc in docs)
                {
                    var ranking = new Ranking()
                    {
                        Name = doc["name"].AsString,
                        Score = doc["score"].AsInt32,
                    };

                    if (ranking.Score < minScore)
                    {
                        minDoc = doc;
                        minName = ranking.Name;
                        minScore = ranking.Score;
                    }

                    rankList.Add(ranking);
                }


                if (score > minScore)
                {
                    minDoc["name"] = name;
                    minDoc["score"] = score;

                    col.ReplaceOne(Builders<BsonDocument>.Filter.Eq(s => s["_id"], minDoc["_id"]),
                        minDoc);

                    var target = rankList.Find((ranking) => ranking.Name == minName
                        && ranking.Score == minScore);

                    target.Name = name;
                    target.Score = score;
                }


                rankList.Sort((r1, r2) => r2.Score.CompareTo(r1.Score));


                using (var sw = new StreamWriter(new FileStream("rank.txt", FileMode.Create)))
                {
                    sw.WriteLine(rankList.Count);

                    foreach (var rank in rankList)
                    {
                        sw.WriteLine(rank.Name);
                        sw.WriteLine(rank.Score);
                    }


                    sw.Close();
                }
            }
            catch
            {
                Console.WriteLine("FAIL!");


                return;
            }
        }
    }
}
