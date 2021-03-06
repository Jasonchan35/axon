// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Mono Runtime Version: 4.0.30319.1
// 
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------
using System;
using System.Collections.Generic;

namespace axc {
public class Compiler {

	public	static Compiler	instance;

	public	string	projRoot;
	public	List< SourceFile >	sourceFiles = new List<SourceFile>();

	public	MetaData	metadata = new MetaData();

	public Compiler () {
		instance = this;
	}

	public	void compile() {
		Log.Info("=================================================");
		projRoot = System.IO.Directory.GetCurrentDirectory();
		Log.Info( "projRoot = " + projRoot );

		foreach( var f in System.IO.Directory.GetFiles( projRoot, "*.axc", System.IO.SearchOption.AllDirectories ) ) {
			var pass = new FirstPass();
			pass.parseFile( f );
		}
	}
}
}

