using System;

namespace axc
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Hello World!");

			var dir = System.IO.Directory.GetCurrentDirectory() + "/../../../../../example/test001";
			System.IO.Directory.SetCurrentDirectory( dir );

			var c = new Compiler();
			c.compile();
		}
	}
}
