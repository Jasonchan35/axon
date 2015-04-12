//
//  AST.cpp
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "AST.h"
#include "Log.h"
#include "Compiler.h"
#include "MetaNode.h"

namespace ax {
namespace Compile {

ax_ImplObject( AST );
ax_ImplObject( PrefixAST );
ax_ImplObject( PostfixAST );
ax_ImplObject( FuncParamAST );
ax_ImplObject( PropAST );
ax_ImplObject( TypeAST );
ax_ImplObject( NumberLiteralAST );
ax_ImplObject( StringLiteralAST );
ax_ImplObject( BinaryAST );


AST::AST	( const Location & pos_, const Type & returnType_ )
: pos(pos_)
, returnType( returnType_ ) {
}


NumberLiteralAST::NumberLiteralAST( const Location &_pos, const ax_string & _srcStr )
: base(_pos, Type() )
, srcStr(_srcStr)
, numberType(t_none)
, numberPrefix(0)
{
	const ax_char* c = srcStr.c_str();
	const ax_char* suffix = nullptr;
	
	if( *c == '0' ) {
		switch( c[1] ) {
			case 'b':{
				c++;
				numberPrefix = *c;		c++;

				uint64_t v = 0;
				int bits = 0;
				for( ; *c; c++ ) {
					if( *c == '_' ) continue;
					if( *c >= '0' && *c <= '1' ) {
						bits++;
						if( bits > sizeof(v) * 8 ) Log::Error( pos, ax_txt("number overflow") );
						
						v <<= 1;
						v += *c - '0';
					}else{
						break;
					}
				}
				
				suffix = c;
				numberValue.v_uint64 = v;
				numberType = t_uint64;
			}break;
			
			case 'o':{
				c++;
				numberPrefix = *c;		c++;

				uint64_t v = 0;
				int bits = 0;
				for( ; *c; c++ ) {
					if( *c == '_' ) continue;
					if( *c >= '0' && * c <= '7' ) {
						bits+=3;
						if( bits > sizeof(v) * 8 ) Log::Error( pos, ax_txt("number overflow") );
						
						v <<= 3;
						v += *c - '0';
					}else{
						break;
					}
				}
				
				suffix = c;
				numberValue.v_uint64 = v;
				numberType = t_uint64;
			}break;
			
			case 'x':{
				c++;
				numberPrefix = *c;		c++;

				uint64_t v = 0;
				int bits = 0;
				for( ; *c; c++ ) {
					if( *c == '_' ) continue;
					if( ax_ishex(*c) ) {
						bits+=4;
						if( bits > sizeof(v) * 8 ) Log::Error( pos, ax_txt("number overflow") );
						
						v <<= 4;
						v += ax_hex_to_int(*c);
					}else{
						break;
					}
				}
				
				suffix = c;
				numberValue.v_uint64 = v;
				numberType = t_uint64;
			}
		}
	}
	
	if( ! numberPrefix ) { // dex
		ax_TempString tmp;
		c = srcStr.c_str();
		bool dot = false;
		
		numberType = t_uint64;		
		
		for( ;*c; c++ ){
			if( *c == '_' ) {
				//skip
			}else if( *c == '.' ) {
				dot = true;
				tmp.append(*c);
			}else if( ax_isdigit(*c) ) {
				tmp.append(*c);
			}else{
				break;
			}
		}
		
		switch( *c ) {
			case 'E':
			case 'e':{
				numberType = t_double;
				tmp.append(*c); c++;

				if( *c == '+' || *c == '-' ) {
					tmp.append(*c); c++;
				}
				
				for( ; *c; c++ ) {
					if( *c == '_' ) {
						//skip
					}else if( ax_isdigit(*c) ) {
						tmp.append(*c);
					}else{
						break;
					}
				}
				
			}break;
		}
		
		suffix = c;
		
		if( dot ) {
			numberType = t_double;
		}
		
		if( suffix ) {
			if( ax_str_case_equals( suffix,ax_sz("f") ) ) {
				numberType = t_float;
			}			
		}
	
		switch( numberType ) {
			case t_uint64:{
				ax_c_str_to( tmp.c_str(), numberValue.v_uint64 );
				numberType = t_uint64;
			}break;
			
			case t_float:{
				ax_c_str_to( tmp.c_str(), numberValue.v_float );
				numberType = t_float;
			}break;
			
			case t_double:{
				ax_c_str_to( tmp.c_str(), numberValue.v_double );
				numberType = t_double;
			}break;
			
			default: {
				Log::Error( pos, ax_txt("error") );
			}break;
		}
		
	} // numberPrefix

	bool hasSuffix = false;

	if( suffix ) {
		if( ax_strlen( suffix ) == 0 ) suffix = nullptr;
	}
	
	if( suffix ) {
		if( ax_str_case_equals( suffix, ax_sz("L") ) ) {
			if( numberType != t_uint64 ) Log::Error( pos, ax_txt("invalid suffix") );
			if( ! ax_safe_assign( numberValue.v_int64, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
			numberType = t_int64;
			
		}else if( ax_str_case_equals( suffix, ax_sz("UL") ) ) {
			if( numberType != t_uint64 ) Log::Error( pos, ax_txt("invalid suffix") );
			if( ! ax_safe_assign( numberValue.v_uint64, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
//			numberType = t_uint64;
			
		}else if( ax_str_case_equals( suffix, ax_sz("U") ) ) {
			if( numberType != t_uint64 ) Log::Error( pos, ax_txt("invalid suffix") );
			if( ! ax_safe_assign( numberValue.v_uint, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
			numberType = t_uint;
		}else if( ax_str_case_equals( suffix, ax_sz("f") ) ) {
			//float
			
		}else{
			Log::Error( pos, ax_txt("unknown suffix {?}"), ax_string::Clone_c_str( suffix ) );
		}
				
	}

	if( ! hasSuffix ) {
		if( numberType == t_uint64 ) {
			if( ! ax_safe_assign( numberValue.v_int, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
			numberType = t_int;
		}
	}
	
	switch( numberType ) {
		case t_int:		base::returnType = Type::MakeLiteral( g_metadata->type_int 	); break;
		case t_uint:	base::returnType = Type::MakeLiteral( g_metadata->type_uint	); break;

		case t_int32:	base::returnType = Type::MakeLiteral( g_metadata->type_int32	); break;
		case t_uint32:	base::returnType = Type::MakeLiteral( g_metadata->type_uint32	); break;

		case t_int64:	base::returnType = Type::MakeLiteral( g_metadata->type_int64	); break;
		case t_uint64:	base::returnType = Type::MakeLiteral( g_metadata->type_uint64	); break;

		case t_float:	base::returnType = Type::MakeLiteral( g_metadata->type_float	); break;
		case t_double:	base::returnType = Type::MakeLiteral( g_metadata->type_double	); break;
		default: Log::Error(_pos, ax_txt("unknown number type") );
	}
}

StringLiteralAST::StringLiteralAST( const Location &pos_, const ax_string & value_ )
: base(pos_, Type::MakeLiteral( g_metadata->type_string ) )
, value(value_) {
}

PrefixAST::PrefixAST( const Location &pos_,  ax_Obj< FuncOverload > fo_, ax_Obj< AST > expr_  )
: base(pos_, fo_->returnType )
, funcOverload(fo_)
, expr(expr_) {
}

PostfixAST::PostfixAST( const Location &pos_, ax_Obj< FuncOverload > fo_, ax_Obj< AST > expr_ )
: base(pos_, fo_->returnType )
, funcOverload(fo_)
, expr(expr_) {
}

BinaryAST::BinaryAST( const Location & pos_, ax_Obj< FuncOverload > fo_, ax_Obj<AST> lhs_, ax_Obj<AST> rhs_, bool parenthesis_ )
: base(pos_, fo_->returnType )
, funcOverload(fo_)
, lhs( lhs_ )
, rhs( rhs_ )
, parenthesis( parenthesis_ )
{
}

TypeAST::TypeAST( Location &pos_, ax_Obj< TypeSpec > node_ )
: base( pos_, Type::MakeTypename( node_ )  )
, node( node_ ) {
}

PropAST::PropAST( Location &pos_, ax_Obj< Prop > node_ )
: base( pos_, node_->type )
, node( node_ ) {
}

FuncParamAST::FuncParamAST( const Location &_pos )
: base(_pos, Type() ) {
}


}} //namespace
