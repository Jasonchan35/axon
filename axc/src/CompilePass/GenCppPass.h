//
//  GenCppPass.h
//  axc
//
//  Created by Jason on 2015-03-30.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#ifndef __axc__GenCppPass__
#define __axc__GenCppPass__


#include "CompilePass.h"

namespace ax {
namespace Compile {

class GenCppPass : public CompilePass, public IExprDispatcher {
public:
	GenCppPass() : ob( *this ) {}

	void	genCode		( const ax_string & outFolder );

private:
	
	void	saveFile			( ax_Obj< MetaNode > node, const ax_string & filename_suffix );
	
	bool	hasToGenSourceFile	( ax_Obj< MetaNode > node );
	
	void	genHdr				( ax_Obj< MetaNode > node );
	void	genCpp				( ax_Obj< MetaNode > node );

	void	genHdr_dispatch		( ax_Obj< MetaNode > node );
	void	genCpp_dispatch		( ax_Obj< MetaNode > node );
	
	void	genHdr_namespace	( ax_Obj< NamespaceNode > node );
	void	genCpp_namespace	( ax_Obj< NamespaceNode > node );

	void	genHdr_struct		( ax_Obj< StructureType > node );
	void	genCpp_struct		( ax_Obj< StructureType > node );

	void	genHdr_prop			( ax_Obj< PropNode >	node );
	void	genCpp_prop			( ax_Obj< PropNode >	node );

	void	genHdr_func			( ax_Obj< FuncNode >	node );
	void	genCpp_func			( ax_Obj< FuncNode >	node );

	void	genHdr_funcOverload	( ax_Obj< FuncOverload > node );
	void	genCpp_funcOverload	( ax_Obj< FuncOverload > node );
	
	

	virtual	void	onAST( IdentifierAST 	& p );

	virtual	void	onAST( NumberAST 		& p );
	virtual	void	onAST( StringLiteralAST & p );
//	virtual	void	onAST( ArrayConstAST	& p );
//	virtual	void	onAST( DictConstAST 	& p );
	
	virtual	void	onAST( PrefixAST 		& p );
	virtual	void	onAST( PostfixAST 		& p );
	virtual	void	onAST( BinaryAST 		& p );
	
//	virtual	void	onAST( TupleAST 		& p );
	virtual	void	onAST( FuncArgAST 		& p );
//	virtual	void	onAST( SubscriptArgAST 	& p );

//	virtual	void	onAST( StatementsAST	& p );
//	virtual	void	onAST( IfAST 			& p );
//	virtual	void	onAST( WhileAST 		& p );
//	virtual	void	onAST( DoWhileAST 		& p );
//	virtual	void	onAST( ForAST 			& p );
//	
//	virtual	void	onAST( ReturnAST 		& p );
//	virtual	void	onAST( BreakAST 		& p );
//	virtual	void	onAST( ContinueAST 		& p );
//
//	virtual	void	onAST( LocalVarAST 		& p );	
//	

	class OutBuf {
	public:
		OutBuf( GenCppPass & pass ) : indentLevel(0), cppPass(pass) {}
		
		OutBuf & operator<< ( const ax_string	 & rhs ) {
//			printf("%ls", rhs.c_str() );
			_buf << rhs; return *this;
		}
		OutBuf & operator<< ( const ax_MutString & rhs ) {
//			printf("%ls", rhs.c_str() );
			_buf << rhs; return *this;
		}
		OutBuf & operator<< ( const ax_char   	 & rhs ) {
//			printf("%c", rhs );
			_buf << rhs; return *this;
		}
				
		OutBuf & operator<< ( ax_Obj< MetaNode > node );
		OutBuf & operator<< ( ax_NullableObj< MetaNode > node );

		OutBuf & operator<< ( const TokenType 	& t  );
		OutBuf & operator<< ( const RType 		& t  );
		
		OutBuf & operator<< ( ax_Obj< ExprAST >  expr );
		OutBuf & operator<< ( ax_NullableObj< ExprAST > expr );

//		OutBuf & output		( const ax_Array< ax_Obj< TemplateParam > >	& tp, bool declare );
		
		OutBuf & nodeName	( ax_Obj< MetaNode > node, bool fullname=false );
		
		OutBuf & newline( ax_int indentOffset = 0);
		
		OutBuf & openBlock	();
		OutBuf & closeBlock	();

		void	beginNamespace		( ax_Obj< MetaNode > node );
		void	endNamespace		( ax_Obj< MetaNode > node );

		bool			inFor; //not include body
		
		ax_NullableObj< StructureType >		inStruct;
//		ax_ax_NullableObjObj< BlockNode >	inBlock;
		ax_NullableObj< NamespaceNode >		inNamespace;
	
		ax_int			indentLevel;
		ax_TempString	_buf;
		
		GenCppPass & cppPass;
	};
	
	OutBuf		ob;

	ax_string	outFolder;

};


}} //namespace

#endif /* defined(__axc__GenCppPass__) */
