//
//  ExprAST.cpp
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "ExprAST.h"
#include "Log.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

ax_ImplObject( ExprAST );
ax_ImplObject( PrefixAST );
ax_ImplObject( PostfixAST );
ax_ImplObject( FuncArgAST );
ax_ImplObject( IdentifierAST );
ax_ImplObject( NumberAST );
ax_ImplObject( StringLiteralAST );
ax_ImplObject( BinaryAST );


NumberAST::NumberAST( const LexerPos &_pos, const ax_string & _srcStr )
: base(_pos)
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
				returnType = RType( g_compiler->metadata.type_uint64, false );
			}break;
			
			case t_float:{
				ax_c_str_to( tmp.c_str(), numberValue.v_float );
				numberType = t_float;
				returnType = RType( g_compiler->metadata.type_float, false );
			}break;
			
			case t_double:{
				ax_c_str_to( tmp.c_str(), numberValue.v_double );
				numberType = t_double;
				returnType = RType( g_compiler->metadata.type_double, false );
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
			returnType = RType( g_compiler->metadata.type_int64, false );
			
		}else if( ax_str_case_equals( suffix, ax_sz("UL") ) ) {
			if( numberType != t_uint64 ) Log::Error( pos, ax_txt("invalid suffix") );
			if( ! ax_safe_assign( numberValue.v_uint64, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
//			numberType = t_uint64;
			returnType = RType( g_compiler->metadata.type_uint64, false );
			
		}else if( ax_str_case_equals( suffix, ax_sz("U") ) ) {
			if( numberType != t_uint64 ) Log::Error( pos, ax_txt("invalid suffix") );
			if( ! ax_safe_assign( numberValue.v_uint, numberValue.v_uint64 ) ) {
				Log::Error( pos, ax_txt("overflow") );
			}
			numberType = t_uint;
			returnType = RType( g_compiler->metadata.type_uint, false );
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
			returnType = RType( g_compiler->metadata.type_int, false );
		}
	}
}

StringLiteralAST::StringLiteralAST( const LexerPos &pos_, const ax_string & value_ )
: base(pos_)
, value(value_) {
	returnType = RType( g_compiler->metadata.type_string, false );
}


}} //namespace
