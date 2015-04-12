//
//  Compiler.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Compiler.h"
#include "CompilePass/DeclarePass.h"
#include "CompilePass/GenCppPass.h"

namespace ax {
namespace Compile {

Compiler*	g_compiler;

Compiler::Compiler() {
	g_compiler = this;
}

void Compiler::loadAllSourceFiles() {
	auto list = System::IO::Directory::GetFiles( project_root, true, false, ax_txt(".axc") );

	ax_foreach( & filename, *list ) {
		auto ext		= System::IO::Path::GetExtension( filename );
		
		auto dir		= System::IO::Path::GetDirectoryName( filename );
		auto basename	= System::IO::Path::GetFileName( filename, false );
		ax_dump3( dir, basename, ext );
		
		auto src = ax_new_obj( SourceFile );
		
		sourceFiles.add( src );
		src->loadFile( filename );
		
	}
}

void Compiler::declarePass() {
	ax_foreach( & f, sourceFiles ) {
		DeclarePass	pass;
		pass.parseFile( f );
	}
	dumpMetadata();

	{
		DeclarePass pass;
		pass.findBuildinTypePass();
		
		pass.run2ndPass();
	}
	dumpMetadata();
}

void Compiler::dumpMetadata() {
	auto s = ax_format( ax_txt("{?}"), *g_metadata );
	System::IO::File::WriteText( project_root + ax_txt("/_auto_gen_/metadata.txt")  , s );
}

void Compiler::genCppPass() {
	GenCppPass	pass;
	pass.genCode( project_root + ax_txt("/_auto_gen_") );
	
	dumpMetadata();
}

void Compiler::compile( const ax_string & project_root ) {
	System::Time::StopWatch	watch;

	this->project_root = project_root;
	ax_log( ax_txt("compile {?}"), project_root );
	
	loadAllSourceFiles();
	declarePass();
				
	genCppPass();

	ax_log( ax_txt("=== compile done ! ===") );
	ax_log( ax_txt("Time: {?}s"), watch.get() );
}


}} //namespace