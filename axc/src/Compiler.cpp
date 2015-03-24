//
//  Compiler.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Compiler.h"
#include "CompilePass/DeclarePass.h"

namespace ax {
namespace Compile {

Compiler*	g_compiler;

Compiler::Compiler() {
	g_compiler = this;
}

void Compiler::loadAllSourceFiles() {
	auto list = System::IO::Directory::GetFilesWithExtension( project_root, ax_txt(".ax"), true );

	ax_foreach( & filename, *list ) {
		auto ext		= System::IO::Path::GetExtension( filename );
		
		auto dir		= System::IO::Path::GetDirectoryName( filename );
		auto basename	= System::IO::Path::GetFileName( filename, false );
		ax_dump( dir, basename, ext );
		
		auto src = ax_new_obj( SourceFile );
		
		sourceFiles.append( src );
		src->loadFile( filename );
		
	}
}

void Compiler::declarePass() {
	ax_foreach( & f, sourceFiles ) {
//		ax_dump( f->filename );
		DeclarePass	pass;
		pass.process( f );
	}
}

void Compiler::compile( const ax_string & project_root ) {
	System::Time::StopWatch	watch;

	this->project_root = project_root;
	ax_log( "compile {?}", project_root );
	
	loadAllSourceFiles();
	declarePass();
	

	ax_dump( metadata );

	ax_log("=== compile done ! ===" );
	ax_log("Time: {?}s", watch.get() );
}


}} //namespace