//
//  GenCppPass.h
//  axc
//
//  Created by Jason on 2015-03-30.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#ifndef __axc__GenCppPass__
#define __axc__GenCppPass__


#include "../Parser.h"
#include "../MetaNode.h"

namespace ax {
namespace Compile {

class GenCppPass : public Parser, public IExprDispatcher {
public:
	GenCppPass() : ob( *this ) {}

	void	genCode		( const ax_string & outFolder );

private:
	
	void	saveFile			( ax_Obj< MetaNode > node, const ax_string & filename_suffix );
	
	bool	hasToGenSourceFile	( ax_Obj< MetaNode > node );
	
	void	genMainCpp			();
	
	void	genMakefile			();
	void	genMakefile			( ax_Obj< MetaNode > node );
	
	void	genHdr				( ax_Obj< MetaNode > node );
	void	genCpp				( ax_Obj< MetaNode > node );

	void	genHdr_dispatch		( ax_Obj< MetaNode > node );
	void	genCpp_dispatch		( ax_Obj< MetaNode > node );
	
	void	genHdr_namespace	( ax_Obj< Namespace > node );
	void	genCpp_namespace	( ax_Obj< Namespace > node );

	void	genHdr_struct		( ax_Obj< StructType > node );
	void	genCpp_struct		( ax_Obj< StructType > node );

	void	genHdr_prop			( ax_Obj< Prop >	node );
	void	genCpp_prop			( ax_Obj< Prop >	node );

	void	genHdr_func			( ax_Obj< Func >	node );
	void	genCpp_func			( ax_Obj< Func >	node );

	void	genHdr_funcOverload	( ax_Obj< FuncOverload > node );
	void	genCpp_funcOverload	( ax_Obj< FuncOverload > node );
	
	

	virtual	void	onAST( TypeAST 			& p );
	virtual	void	onAST( PropAST 			& p );

	virtual	void	onAST( NumberLiteralAST 		& p );
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
		OutBuf( GenCppPass & pass ) : cppPass(pass) { reset(); }

		ax_MutString &	buf() { return _buf; }
				void	reset();
		
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
				
		template< typename T >
		OutBuf & operator<< ( ax_Obj< T > 				node ) { nodeName( node, true ); return *this;; }


		OutBuf & operator<< ( const TokenType 	& t  );
		OutBuf & operator<< ( const RType 		& t  );
		
		OutBuf & operator<< ( ax_Obj< ExprAST >  expr );
		OutBuf & operator<< ( ax_NullableObj< ExprAST > expr );

//		OutBuf & output		( const ax_Array< ax_Obj< TemplateParam > >	& tp, bool declare );
		
		OutBuf & nodeName	( ax_Obj< MetaNode > node, bool fullname );
		
		OutBuf & newline	( ax_int indentOffset = 0);
		
		OutBuf & openBlock	();
		OutBuf & closeBlock	();

		void	beginNamespace		( ax_NullableObj< Namespace > node );
		void	endNamespace		( ax_NullableObj< Namespace > node );

		bool			inFor; //not include body
		
		ax_NullableObj< MetaNode >		inCppNode;
	
		ax_int			indentLevel;
		GenCppPass & 	cppPass;
		
	private:
		ax_TempString	_buf;
	};
	
	OutBuf		ob;

	ax_string	outFolder;

};


}} //namespace

#endif /* defined(__axc__GenCppPass__) */
