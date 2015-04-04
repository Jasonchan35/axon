//
//  GenCppPass.cpp
//  axc
//
//  Created by Jason on 2015-03-30.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "GenCppPass.h"

namespace ax {
namespace Compile {

bool GenCppPass::hasToGenSourceFile( ax_Obj< MetaNode > node ) {
	if( node->ax_is< NamespaceNode >() ) return true;

	ax_if_let( type, node->ax_as< StructureType >() ) {
		if( ! type->buildin && ! type->isNestedType ) {
			return true;
		}
	}
	return false;
}

void GenCppPass::genCode( const ax_string & outFolder ) {
	ax_log( ax_txt("genCode {?}"), outFolder );
		
	this->outFolder = outFolder;
	
	System::IO::Directory::RemoveIfExists( outFolder, true );
	System::IO::Directory::Create( outFolder );

	genHdr( g_compiler->metadata.root );
	genCpp( g_compiler->metadata.root );
}

void GenCppPass::genHdr( ax_Obj< MetaNode > node ) {

	ax_TempString 	define_name;
	
	auto define_seperator = ax_txt("__");
	
	bool writeFile = hasToGenSourceFile(node);
	
	if( writeFile ) {
		define_name << node->getFullname( define_seperator );
		define_name << ax_txt("_h_file");
	
		ob << ax_txt("#ifndef ") << define_name << ax_txt("\n");
		ob << ax_txt("#define ") << define_name << ax_txt("\n");
		
		ax_foreach( & c, *node->children ) {
			if( ! hasToGenSourceFile( c ) ) continue;
			ob << ax_txt("#include \"") << node->name << ax_txt("/") << c->name << ax_txt( ".h\"\n" );
		}

		ob.beginNamespace( node );
			genHdr_dispatch( node );
			ob.newline();
		ob.endNamespace( node );

		ob << ax_txt("\n\n#endif // ") << define_name << ax_txt("\n");
		
		saveFile( node, ax_txt(".h") );
	}
	
	ax_foreach( & c, *node->children ) {
		genHdr( c );
	}
}


void GenCppPass::genCpp		( ax_Obj< MetaNode > node ) {

	bool writeFile = hasToGenSourceFile( node );
	if( writeFile ) {
		ax_if_let( ns, node->ax_as< NamespaceNode >() ) {
			ax_foreach( & c, *ns->children ) {
				if( ! hasToGenSourceFile( c ) ) continue;
				ob << ax_txt("#include \"") << node->name << ax_txt("/") << c->name << ax_txt(".cpp\"\n");
			}
		}else{
			ob << ax_txt("#include \"") << node->name << ax_txt( ".h\"\n" );
		}
				
		ob.beginNamespace( node );
			genCpp_dispatch( node );
			ob.newline();
		ob.endNamespace( node );
		
		saveFile( node, ax_txt(".cpp") );
	}
	
	ax_foreach( & c, *node->children ) {
		genCpp( c );
	}
}



void GenCppPass::genHdr_dispatch( ax_Obj< MetaNode > node ) {
	ax_if_let( ns, 		node->ax_as<NamespaceNode>() )	{ return genHdr_namespace( ns ); }
	ax_if_let( fn, 		node->ax_as<FuncNode>() )		{ return genHdr_func( fn ); }
	ax_if_let( prop,	node->ax_as<PropNode>() )		{ return genHdr_prop( prop ); }
	ax_if_let( cp,		node->ax_as<StructureType>() )		{ return genHdr_struct( cp ); }
}

void GenCppPass::genCpp_dispatch( ax_Obj< MetaNode > node ) {
	ax_if_let(  ns,		node->ax_as<NamespaceNode>() )	{ return genCpp_namespace( ns ); }
	ax_if_let(  fn,		node->ax_as<FuncNode>() )		{ return genCpp_func( fn ); }
	ax_if_let(  prop,	node->ax_as<PropNode>() )		{ return genCpp_prop( prop ); }
	ax_if_let(  cp,		node->ax_as<StructureType>() )		{ return genCpp_struct( cp ); }
}

void GenCppPass::genHdr_namespace( ax_Obj< NamespaceNode > node ) {
}

void GenCppPass::genCpp_namespace( ax_Obj< NamespaceNode > node ) {
}

void GenCppPass::genHdr_func( ax_Obj< FuncNode > node ) {
	ob.newline();
//	ob << ax_Obj< MetaNode >( node->type );
	ob << ax_txt("func ");
	ob << ax_txt("\t") << node->name << ax_txt("();");
}

void GenCppPass::genCpp_func( ax_Obj< FuncNode > node ) {
}

void GenCppPass::genHdr_prop( ax_Obj< PropNode > node ) {
	ob.newline();
//	ob << ax_Obj< MetaNode >( node->type );
	ob << ax_txt("var");
	ob << ax_txt("\t") << node->name;
	
	ax_if_let( expr, node->initExpr ) {
		ob << ax_txt(" = " ) << expr;
	}
	
	ob << ax_txt(";");
}

void GenCppPass::genCpp_prop( ax_Obj< PropNode > node ) {
}

void GenCppPass::genHdr_struct( ax_Obj< StructureType > node ) {
	ob.newline();
	ob.newline();

	auto scope_struct = ax_scope_value( ob.inStruct, node );
	
//	if( node->templateParams.size() ) {
//		outputTemplateParams( node, true );
//	}

	if( node->ax_is<StructNode>() ) {
		ob << ax_txt("struct ");
		
	}else if( node->ax_is<ClassNode>() ) {
		ob << ax_txt("class ");
		
	}else if( node->ax_is<InterfaceNode>() ) {
		ob << ax_txt("//interface");
		ob.newline() << ax_txt("class ");
		
	}else{
		Log::Error( node->pos , ax_txt("unknown type") );
	}
	
	ob << node->name;

	ax_int c = 0;

	ax_if_let( bt, node->baseType ) {
		ob << ax_txt(" : public ") << bt->as_MetaNode();
		c++;
	}
		
	ax_foreach( & p, node->interfaces ) {
		ob << ( c == 0 ? ax_txt(" : ") : ax_txt(", ") ) << ax_txt("public ") << p->as_MetaNode();
	}
	
	ob << ax_txt(" ");
	ob.openBlock();
	
	if( node->ax_is< ClassNode >() || node->ax_is< InterfaceNode >() ) {
//		ob.newline() << ax_txt("ax_DefObject( ") << node->name << ax_txt(", ") << baseClass << ax_txt(" );");
		ob.newline(-1) << ax_txt("public:");
	}
	
	ax_foreach( & c, *node->children ) {
		if( this != & ob.cppPass ) {
			throw System::Err_Undefined();
		}
	
		ax_if_let( prop, c->ax_as<PropNode>() ) { genHdr_prop(prop); continue; }
		ax_if_let( fn,	 c->ax_as<FuncNode>() ) { genHdr_func(fn); 	 continue; }
		ax_if_let( nestedType, c->ax_as<StructureType>() ) { genHdr_struct(nestedType); continue; }
	}
	
	ob.closeBlock();
	ob << ax_txt("; //") << node->name;
	ob.newline();
}

void GenCppPass::genCpp_struct( ax_Obj< StructureType > node ) {
}

void GenCppPass::saveFile( ax_Obj< MetaNode > node, const ax_string & filename_suffix ) {

	auto path = node->getFullname( ax_txt("/") );

	auto filename = ax_format( ax_txt("{?}/{?}{?}"), outFolder, path, filename_suffix );

//	ax_log("save file {?}", filename );

	auto dir = System::IO::Path::GetDirectoryName( filename );
	System::IO::Directory::Create( dir );

	ax_TempStringA	tmp;
	
	tmp.assignUtf( ob._buf );

	System::IO::File::WriteUtf8( filename, tmp );
	
	ob._buf.clear();
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_Obj< MetaNode > node ) {
	return nodeName(node);
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_NullableObj< MetaNode > node ) {
	ax_if_let( node_, node ) {
		return nodeName( node_ );
	}else{
		return *this;
	}
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( const TokenType 	& t  ) {
	*this << ax_to_string(t);
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( const RType 		& t  ) {
	ax_if_let( type, t.type ) {
		*this << ax_Obj< MetaNode >( type );
	}else{
		Log::Error( nullptr, nullptr, ax_txt("error") );
	}
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_Obj< ExprAST >  expr ) {
	expr->dispatch( cppPass );
	return *this;
}

GenCppPass::OutBuf & GenCppPass::OutBuf::operator<< ( ax_NullableObj< ExprAST > expr ) {
	ax_if_let( expr_, expr ) {
		expr_->dispatch( cppPass );
	}
	return *this;
}


GenCppPass::OutBuf& GenCppPass::OutBuf::nodeName ( ax_Obj< MetaNode > node, bool fullname ) {
/*
	if( ! node ) Log::Error( node->pos, "node=null" );

	auto typealias = node->ax_as< Typealias >();
	if( typealias ) return nodeName( typealias->alias, fullname );

	if( node != inClass ) {
		auto p = node->parent;
		if( p ) {
			bool outParent = true;
			
			if( ! fullname ) {
				if( p == inClass || p == inBlock ) {
					outParent = false;
				}else{
					auto ns = inNamespace;
					while( ns ) {
						if( p == ns ) {
							outParent = false;
							break;
						}
						if( ns->parent ) {
							ns = ns->parent->ax_as<Namespace>();
						}else{
							ns = nullptr;
						}
					}
				}
			}

			if( outParent ) {
				nodeName( p, fullname );
				_buf << ax_str("::");
			}
		}
	}
*/
	*this << node->name;
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

void GenCppPass::OutBuf::beginNamespace		( ax_Obj< MetaNode > node ) {
	ax_if_let( p, node->parent ) {
		beginNamespace( p );
	}
	ax_if_let( ns, node->ax_as< NamespaceNode >() ) {
		newline() << ax_txt("namespace ") << node->name << ax_txt(" {");
		inNamespace = ns;
	}
}

void GenCppPass::OutBuf::endNamespace		( ax_Obj< MetaNode > node ) {
	ax_if_let( ns, node->ax_as< NamespaceNode >() ) {
		newline() << ax_txt("} //namespace ") << node->name;
	}
	ax_if_let( p, node->parent ) {
		endNamespace( p );
	}
	inNamespace = nullptr;
}

//--------------


void	GenCppPass::onAST( IdentifierAST & p ) {
	ax_if_not_let( node, p.node ) {
		Log::Error( p.pos, ax_txt("invalid identifier") );
	}
	ob << node;
}

void	GenCppPass::onAST( NumberAST 			& p ) {
	ob << p.srcStr;
	/*
	switch( p.numberType ) {
		case NumberAST::t_int32:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberAST::t_uint32:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberAST::t_int64:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberAST::t_uint64:	{ ob << ax_to_string( p.numberValue.v_int32 );	}break;
		case NumberAST::t_float:	{ ob << ax_to_string( p.numberValue.v_float ) << ax_txt("f"); }break;
		case NumberAST::t_double:	{ ob << ax_to_string( p.numberValue.v_double );	}break;

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
	ob << ax_txt("(") << p.op << p.expr << ax_txt(")");
}

void	GenCppPass::onAST( PostfixAST 	& p ) {
	ob << ax_txt("(") << p.expr << p.op << ax_txt(")");
}

void	GenCppPass::onAST( BinaryAST 	& p ) {
	bool parenthesis = true;

	switch( p.op ) {
		default: break;
		case TokenType::t_op_subscript:
		case TokenType::t_op_call:
		case TokenType::t_assign:
			parenthesis = false;
			break;
	}

	bool ctor = false;
	auto lt = p.lhs->returnType;
	if( lt.isTypeName && p.op == TokenType::t_op_call ) {
		ctor = true;
	}
	
	if( parenthesis ) ob << ax_txt("(");
		
	ob << p.lhs;

	if( ctor ) {
//		output("::init");
	}
	
	if( p.op != TokenType::t_op_call && p.op != TokenType::t_op_subscript ) {
		ob << ax_txt( " " ) << p.op << ax_txt( " " );
	}
	ob << p.rhs;
	
	if( parenthesis ) ob << ax_txt(")");
}

void	GenCppPass::onAST( FuncArgAST 			& p ) {
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

