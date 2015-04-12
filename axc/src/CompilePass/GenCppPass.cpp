//
//  GenCppPass.cpp
//  axc
//
//  Created by Jason on 2015-03-30.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "GenCppPass.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

auto	symbol_prefix 		= ax_txt("_ax_");
auto	symbol_seperator 	= ax_txt("___");

bool GenCppPass::hasToGenSourceFile( ax_Obj< MetaNode > node ) {
	if( node->ax_is< Namespace >() ) return true;

	ax_if_let( type, node->ax_as< CompositeType >() ) {
		if( ! type->buildin && ! type->isNestedType ) {
			return true;
		}
	}
	return false;
}

void GenCppPass::genCode( const ax_string & outFolder ) {
	ax_log( ax_txt("genCode {?}"), outFolder );
		
	this->outFolder = outFolder;

//---- find class App
	ax_if_let( appNode, g_metadata->root->getNode( ax_txt("App") ) ) {
		ax_if_not_let( app, appNode->ax_as<Class>() ) {
			Log::Error( app->pos, ax_txt("App should be class type") );
		}
		g_metadata->type_app = app;
	}
		
		
//------
	System::IO::Directory::RemoveIfExists( outFolder, true );
	System::IO::Directory::Create( outFolder );

	genMakefile();

	genHdr( g_metadata->namespace_ax );
	genCpp( g_metadata->namespace_ax );
	
	genMainCpp();
}

void GenCppPass::genMainCpp() {
	ax_if_not_let( app, g_metadata->type_app ) {
		return;
	}
	ob << ax_txt("#include \"ax/ax_build/App.h\"\n\n");
	
	ob << ax_txt("int main() {\n");
	ob << ax_txt("	ax::ax_build::App app;\n");
	ob << ax_txt("	app.main();\n");
	ob << ax_txt("}\n");

	auto filename = ax_format( ax_txt("{?}/main.cpp"), outFolder );

//	ax_log("save file {?}", filename );

	auto dir = System::IO::Path::GetDirectoryName( filename );
	System::IO::Directory::Create( dir );

	ax_TempStringA	tmp;
	tmp.assignUtf( ob.buf() );

	System::IO::File::WriteUtf8( filename, tmp );
	ob.reset();
}

void GenCppPass::genMakefile() {
	ob << ax_txt(	"SOURCES=\\\n");

	ax_if_let( app, g_metadata->type_app ) {
		ob << ax_txt(	"	main.cpp \\\n");
	}

	genMakefile( g_metadata->namespace_ax );
	ob << ax_txt(	"#------\n\n");

	ob << ax_txt(	"OBJECTS=$(SOURCES:.cpp=.cpp.o)	\n"
					"CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++	\n"
					"CFLAGS=-c -Wall -I../../../ax_core/include	-std=gnu++11 -stdlib=libc++ -fno-rtti\n"
					"LDFLAGS=-L../../../ax_core/lib/MacOSX/Debug -L../../../ax_core/lib/MacOSX -ObjC -lax_core -lgc -stdlib=libc++ -framework Foundation \n"
					"EXECUTABLE=hello_axc\n"
					"\n"
					"all: $(EXECUTABLE)	\n"
					"\n"
					"$(EXECUTABLE): $(OBJECTS)	\n"
					"	$(CC) $(OBJECTS) $(LDFLAGS) -o $@	\n"
					"\n"
					"%.cpp.o: %.cpp\n"
					"	$(CC) $(CFLAGS) $< -o $@	\n");


	auto filename = ax_format( ax_txt("{?}/GNUmakefile"), outFolder );

//	ax_log("save file {?}", filename );

	auto dir = System::IO::Path::GetDirectoryName( filename );
	System::IO::Directory::Create( dir );

	ax_TempStringA	tmp;
	tmp.assignUtf( ob.buf() );

	System::IO::File::WriteUtf8( filename, tmp );
	ob.reset();
}

void GenCppPass::genMakefile( ax_Obj< MetaNode > node ) {

	bool writeFile = hasToGenSourceFile(node);
	
	if( writeFile ) {
		auto filename = node->getFullname( ax_txt("/") );
		ob << ax_txt("\t") << filename << ax_txt(".cpp \\\n");
	}

	ax_foreach( &c, node->children ) {
		genMakefile( c );
	}
}

void GenCppPass::genHdr( ax_Obj< MetaNode > node ) {

	ax_TempString 	define_name;
	
	bool writeFile = hasToGenSourceFile(node);
	
	if( writeFile ) {
		define_name << node->getFullname( symbol_seperator );
		define_name << ax_txt("_h_file");
	
		ob << ax_txt("#ifndef ") << define_name << ax_txt("\n");
		ob << ax_txt("#define ") << define_name << ax_txt("\n");
		
		ob.newline();
		ob << ax_txt("#include <ax/core.h>") << ax_txt("\n");
		
		ax_foreach( & c, node->children ) {
			if( ! hasToGenSourceFile( c ) ) continue;
			ob << ax_txt("#include \"") << node->cppName() << ax_txt("/") << c->cppName() << ax_txt( ".h\"\n" );
		}

		ax_if_let( inNamespace, node->getUpperByType<Namespace>() ) {
			ob.inCppNode = inNamespace;
		}

		ob.beginNamespace( inNamespace );
			ob.newline();		
			genHdr_dispatch( node );
			ob.newline();
		ob.endNamespace( inNamespace );

		ob << ax_txt("\n\n#endif // ") << define_name << ax_txt("\n");
		
		saveFile( node, ax_txt(".h") );
	}
	
	ax_foreach( & c, node->children ) {
		genHdr( c );
	}
}

void GenCppPass::genCpp		( ax_Obj< MetaNode > node ) {

	bool writeFile = hasToGenSourceFile( node );
	if( writeFile ) {
	
		ax_if_let( ns, node->ax_as< Namespace >() ) {
		/*
			ax_foreach( & c, *ns->children ) {
				if( ! hasToGenSourceFile( c ) ) continue;
				ob << ax_txt("#include \"") << node->cppName() << ax_txt("/") << c->cppName() << ax_txt(".cpp\"\n");
			}
		*/	
		}else{
			ob << ax_txt("#include \"") << node->cppName() << ax_txt( ".h\"\n" );
		}

		ax_if_let( inNamespace, node->getUpperByType<Namespace>() ) {
			ob.inCppNode = inNamespace;
		}
				
		ob.beginNamespace( inNamespace );
			ob.newline();
			genCpp_dispatch( node );
			ob.newline();
		ob.endNamespace( inNamespace );
		
		saveFile( node, ax_txt(".cpp") );
	}
	
	ax_foreach( & c, node->children ) {
		genCpp( c );
	}
}

void GenCppPass::genHdr_dispatch( ax_Obj< MetaNode > node ) {
	ax_if_let( ns, 		node->ax_as<Namespace>() )	{ return genHdr_namespace( ns ); }
	ax_if_let( fn, 		node->ax_as<Func>() )		{ return genHdr_func( fn ); }
	ax_if_let( prop,	node->ax_as<Prop>() )		{ return genHdr_prop( prop ); }
	ax_if_let( cp,		node->ax_as<CompositeType>() )	{ return genHdr_struct( cp ); }
}

void GenCppPass::genCpp_dispatch( ax_Obj< MetaNode > node ) {
	ax_if_let(  ns,		node->ax_as<Namespace>() )	{ return genCpp_namespace( ns ); }
	ax_if_let(  fn,		node->ax_as<Func>() )		{ return genCpp_func( fn ); }
	ax_if_let(  prop,	node->ax_as<Prop>() )		{ return genCpp_prop( prop ); }
	ax_if_let(  cp,		node->ax_as<CompositeType>() )	{ return genCpp_struct( cp ); }
}

void GenCppPass::genHdr_namespace( ax_Obj< Namespace > node ) {
}

void GenCppPass::genCpp_namespace( ax_Obj< Namespace > node ) {
}

void GenCppPass::genHdr_func( ax_Obj< Func > node ) {
	ax_foreach( &fo, node->overloads ) {
		genHdr_funcOverload( fo );
	}
}

void GenCppPass::genCpp_func( ax_Obj< Func > node ) {
	ax_foreach( &fo, node->overloads ) {
		genCpp_funcOverload( fo );
	}
}

void GenCppPass::genHdr_funcOverload( ax_Obj< FuncOverload > fo ) {
	ob.newline();
	
	ob << fo->returnType;
	ob << ax_txt(" ") << fo->cppName() << ax_txt("(");
	
	if( fo->params.size() > 0 ) {
		ob << ax_txt(" ");
		
		ax_int c = 0;
		ax_foreach( &p, fo->params ) {
			if( c > 0 ) ob << ax_txt(", ");
			ob << p.type << ax_txt(" ") << p.name;
			c++;
		}
		
		ob << ax_txt(" ");
	}
	
	ob << ax_txt(");");
}

void GenCppPass::genCpp_funcOverload( ax_Obj< FuncOverload > fo ) {
	ob.newline();
	
	ob << fo->returnType;
	ob << ax_txt(" ") << fo->func->name() << ax_txt("(");
	
	if( fo->params.size() > 0 ) {
		ob << ax_txt(" ");
		
		ax_int c = 0;
		ax_foreach( &p, fo->params ) {
			if( c > 0 ) ob << ax_txt(", ");
			ob << p.type << ax_txt(" ") << p.name;
			c++;
		}
		
		ob << ax_txt(" ");
	}
	
	ob << ax_txt(") {\n");
	ob << ax_txt("}\n");
}

void GenCppPass::genHdr_prop( ax_Obj< Prop > node ) {
	ob.newline();
	ob << node->type;
	ob << ax_txt("\t") << node->cppName();
/*
	ax_if_let( expr, node->initExpr ) {
		ob << ax_txt(" = " ) << expr;
	}
*/	
	ob << ax_txt(";");
}

void GenCppPass::genCpp_prop( ax_Obj< Prop > node ) {
}

void GenCppPass::genHdr_struct( ax_Obj< CompositeType > node ) {
	ob.newline();
	ob.newline();

//	if( node->templateParams.size() ) {
//		outputTemplateParams( node, true );
//	}

	if( node->ax_is<Struct>() ) {
		ob << ax_txt("struct ");
		
	}else if( node->ax_is<Class>() ) {
		ob << ax_txt("class ");
		
	}else if( node->ax_is<Interface>() ) {
		ob << ax_txt("//interface");
		ob.newline() << ax_txt("class ");
		
	}else{
		Log::Error( node->pos , ax_txt("unknown type") );
	}
	ob << node->cppName();

	ax_int c = 0;

	ax_if_let( bt, node->baseType ) {
		ob << ax_txt(" : public ") << bt;
		c++;
	}
		
	ax_foreach( & p, node->interfaces ) {
		ob << ( c == 0 ? ax_txt(" : ") : ax_txt(", ") ) << ax_txt("public ") << p;
	}
	
	auto scope_struct = ax_scope_value( ob.inCppNode, node );
	
	ob << ax_txt(" ");
	ob.openBlock();
	
	if( node->ax_is< Class >() || node->ax_is< Interface >() ) {
//		ob.newline() << ax_txt("ax_DefObject( ") << node->name << ax_txt(", ") << baseClass << ax_txt(" );");
		ob.newline(-1) << ax_txt("public:");
	}
	
	ax_foreach( & c, node->children ) {
		ax_if_let( prop, 		c->ax_as<Prop>() 		) { genHdr_prop(prop); continue; }
		ax_if_let( fn,		 	c->ax_as<Func>() 		) { genHdr_func(fn); 	 continue; }
		ax_if_let( nestedType, 	c->ax_as<CompositeType>() 	) { genHdr_struct(nestedType); continue; }
	}
	
	ob.newline(-1) << ax_txt("private:");
	ob.newline();
	
	ob << ax_txt("void ") << symbol_prefix << ax_txt("init_props();");
	
	
	ob.closeBlock();
	ob << ax_txt("; //") << node->cppName();
	ob.newline();
}

void GenCppPass::genCpp_struct( ax_Obj< CompositeType > node ) {
	ax_foreach( & c, node->children ) {
		ax_if_let( prop, c->ax_as<Prop>() ) { genCpp_prop(prop); continue; }
		ax_if_let( fn,	 c->ax_as<Func>() ) { genCpp_func(fn); 	 continue; }
		ax_if_let( nestedType, c->ax_as<CompositeType>() ) { genCpp_struct(nestedType); continue; }
	}
}

void GenCppPass::saveFile( ax_Obj< MetaNode > node, const ax_string & filename_suffix ) {

	auto path = node->getFullname( ax_txt("/") );

	auto filename = ax_format( ax_txt("{?}/{?}{?}"), outFolder, path, filename_suffix );

//	ax_log("save file {?}", filename );

	auto dir = System::IO::Path::GetDirectoryName( filename );
	System::IO::Directory::Create( dir );

	ax_TempStringA	tmp;
	tmp.assignUtf( ob.buf() );

	System::IO::File::WriteUtf8( filename, tmp );
	ob.reset();
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( const TokenType 	& t  ) {
	*this << ax_to_string(t);
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( const RType 		& t  ) {
	ax_if_let( type, t.type ) {
		*this << type;
	}else{
		*this << ax_txt("NULL");
	}
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_Obj< AST >  expr ) {
	expr->dispatch( cppPass );
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_NullableObj< AST > expr ) {
	ax_if_let( expr_, expr ) {
		expr_->dispatch( cppPass );
	}
	return *this;
}


GenCppPass::OutBuf& GenCppPass::OutBuf::nodeName ( ax_Obj< MetaNode > node, bool fullname ) {

//	if( ! node ) Log::Error( node->pos, "node=null" );

//	auto typealias = node->ax_as< Typealias >();
//	if( typealias ) return nodeName( typealias->alias, fullname );

	if( fullname && ! node->macro_cppName && inCppNode != node ) {
		ax_if_let( p, node->parent ) {
			if( inCppNode != p ) {
				nodeName( p, true );
				*this << ax_txt("::");
			}
		}
	}

	*this << node->cppName();
	return *this;
}

GenCppPass::OutBuf& GenCppPass::OutBuf::openBlock() {
	*this << ax_txt("{");
	indentLevel++;
//	newline();
	return *this;
}

GenCppPass::OutBuf& GenCppPass::OutBuf::closeBlock() {
	indentLevel--;
	newline() << ax_txt("}");
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::newline( ax_int indentOffset ) 	{
	_buf.append( '\n' );
	_buf.appendRepeat('\t', indentLevel + indentOffset );
	return *this;
}

void GenCppPass::OutBuf::beginNamespace		( ax_NullableObj< Namespace > node ) {
	ax_if_not_let( ns, node ) {
		return;
	}

	ax_if_let( p, ns->parent ) {
		beginNamespace( p->ax_as<Namespace>() );
	}
	newline() << ax_txt("namespace ") << ns->cppName() << ax_txt(" {");
}

void GenCppPass::OutBuf::endNamespace		( ax_NullableObj< Namespace > node ) {
	ax_if_not_let( ns, node ) {
		return;
	}

	newline() << ax_txt("} //namespace ") << ns->cppName();
	
	ax_if_let( p, ns->parent ) {
		endNamespace( p->ax_as<Namespace>() );
	}
}

void GenCppPass::OutBuf::reset() {
	indentLevel = 0;

	inCppNode	= nullptr;
	inFor		= false;
	
	_buf.clear();
}


//--------------


void	GenCppPass::onAST( PropAST & p ) {
	ax_if_not_let( node, p.node ) {
		Log::Error( p.pos, ax_txt("invalid prop identifier") );
	}
	ob << node;
}

void	GenCppPass::onAST( TypeAST & p ) {
	ax_if_not_let( node, p.node ) {
		Log::Error( p.pos, ax_txt("invalid type identifier") );
	}
	ob << node;
}

void	GenCppPass::onAST( NumberLiteralAST 			& p ) {
	ob << p.srcStr;
	/*
	switch( p.numberType ) {
		case NumberLiteralAST::t_int32:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberLiteralAST::t_uint32:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberLiteralAST::t_int64:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberLiteralAST::t_uint64:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberLiteralAST::t_float:	{ ob << ax_to_string( p.numberValue.v_float ) << ax_txt("f"); }break;
		case NumberLiteralAST::t_double:	{ ob << ax_to_string( p.numberValue.v_double );	}break;

		default:{ ob << p.srcStr; }break;
	}
	*/
}

void	GenCppPass::onAST( StringLiteralAST  	& p ) {
	ob << ax_txt("ax_txt(\"");
	const ax_char* c = p.value.c_str();
	for( ; *c; c++ ) {
		switch(*c) {
			case '\t': ob << ax_txt("\\t"); 	break;
			case '\r': ob << ax_txt("\\r"); 	break;
			case '\n': ob << ax_txt("\\n"); 	break;
			default:   ob << *c;		break;
		}
	}
	ob << ax_txt("\")");
}
/*
void	GenCppPass::onAST( ArrayConstAST	& p ) {
	ob << ax_txt( "ArrayConstAST");
}

void	GenCppPass::onAST( DictConstAST 	& p ) {
	ob << ax_txt( "DictConstAST");
}

void	GenCppPass::onAST( ReturnAST 		& p ) {
	ob << ax_txt( "return" );
	ax_if_let( ret, p.returnValue ) {
		ob << ax_txt(" ") << ret;
	}	
}

void	GenCppPass::onAST( BreakAST 		& p ) {
	ob << ax_txt( "break" );
}

void	GenCppPass::onAST( ContinueAST 		& p ) {
	ob << ax_txt( "continue" );
}

void	GenCppPass::onAST( StatementsAST	& p ) {
	ax_foreach( auto & s, p.statements ) {
		ob.newline() << s << ax_txt(";");
	}
}

void	GenCppPass::onAST( IfAST 		& p ) {
//	newLine();
	ob << ax_txt( "if" ) << p.condExpr;
	ob.openBlock();
	
	ob << p.trueExpr;
	
	ob.closeBlock();
	ax_if_let( falseExpr, p.falseExpr ) {
		ob << ax_txt("else");
		ob.openBlock();
			ob << falseExpr;
		ob.closeBlock();
	}
}

void	GenCppPass::onAST( WhileAST 		& p ) {
	ob.newline();
	ob << ax_txt( "while" );
	ob << p.condExpr;
	
	ob.openBlock();
		ob << p.bodyExpr;
	ob.closeBlock();
}

void	GenCppPass::onAST( DoWhileAST 		& p ) {
	ob.newline();
	ob << ax_txt( "do" );
	
	ob.openBlock();
		ob << p.bodyExpr;
	ob.closeBlock();
	
	ob << ax_txt( "while " );
	ob << p.condExpr;
}

void	GenCppPass::onAST( ForAST 		& p ) {
	ob.newline();
	ob << ax_txt( "for( " );
	ob << p.initExpr << ax_txt("; ");
	ob << p.condExpr << ax_txt("; ");
	ob << p.stepExpr << ax_txt(" ) ");
	
	ob.openBlock();
		ob << p.bodyExpr;
	ob.closeBlock();
}

void	GenCppPass::onAST( TupleAST 			& p ) {
	ob << ax_txt("<TupleAST>");
}

void	GenCppPass::onAST( SubscriptArgAST 	& p ) {
	ob << ax_txt("<SubscriptArgAST>");
}

void	GenCppPass::onAST( LocalVarAST 		& p ) {

	ax_foreach( auto & prop, p.props ) {
		if( prop->dataType == nullptr ) {
			Log::Error( &prop->pos, "Unknown type for prop '{?}'", prop->name );
		}
		ob << prop->dataType << ax_txt(" ") << prop->name << ax_txt(" = ");

		ax_if_let( initExpr, prop->initExpr ) {
			ob << initExpr;
		}else{
			ob << ax_txt("type_default");
		}
	}
}

*/

void	GenCppPass::onAST( PrefixAST 	& p ) {
	ob << p.funcOverload->cppName();
	ob << p.expr;
}

void	GenCppPass::onAST( PostfixAST 	& p ) {
	ob << p.funcOverload->cppName();
	ob << p.expr;
}

void	GenCppPass::onAST( BinaryAST 	& p ) {
	ob << p.lhs << ax_txt(" ") << p.funcOverload->cppName() << ax_txt(" ") << p.rhs;
}

void	GenCppPass::onAST( FuncParamAST 			& p ) {
	ob << ax_txt("(");
	
	int c = 0;
	ax_foreach( & param, p.args ) {
		if( c > 0 ) ob << ax_txt(", ");
		ob << param;
		c++;
	}
	
	ob << ax_txt(")");
}



}} //namespace

