//
//  MetaNode.h
//  axc
//
//  Created by Jason on 2014-12-07.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__MetaNode__
#define __axc__MetaNode__

#include "Log.h"
#include "DeclarationModifier.h"
#include "ExprAST.h"

namespace ax {
namespace Compile {

extern ax_string	k_ctor_name;

class Func;
class ExprAST;

class LexerPos;
class MetaNode : public System::Object {
	ax_DefObject( MetaNode, System::Object )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	MetaNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	template< typename T >
	ax_NullableObj<T>	getUpperByType	() {
//		ax_dump( name );
		ax_assert( this );
		
		ax_if_let( p, this->ax_as<T>() ) {
			return p;
		}
		
		ax_if_let( o, parent ) {
			return o->getUpperByType<T>();
		}else{
			return nullptr;
		}
	}

	const ax_string &	name	() const	{ return _name; }
	const ax_string &	cppName	() const	{ return _cppName.size() ? _cppName : _name; }

	void	setCppName( const ax_string & s, bool macro_cppName_ ) { _cppName = s; macro_cppName = macro_cppName_; }

	ax_NullableObj< MetaNode >	parent;
	ax_string			_name;
	ax_string			_cppName;
	bool				macro_cppName;
	
	LexerPos			pos;
	bool				buildin;
	bool				isTemplateInstance;
		
		ax_string	getFullname		( const ax_string & seperator ) const;
			void	appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const;


			ax_NullableObj< MetaNode >	getNode			( const ax_string & name );
	virtual	ax_NullableObj< MetaNode >	onGetNode		( const ax_string & name ) { return nullptr; }
	
			ax_NullableObj< MetaNode >	getMember		( const ax_string & name );
	virtual	ax_NullableObj< MetaNode >	onGetMember		( const ax_string & name );

	
	ax_NullableObj< Func >	getFunc		( const ax_string & name );
	ax_Obj< Func >			getOrAddFunc( const ax_string & name );
	ax_Obj< Func >			addFunc		( const ax_string & name );
		
	ax_NullableObj< Func >	getOperatorFunc				( TokenType op );
	ax_Obj< Func >			getOrAddOperatorFunc		( TokenType op );

	ax_NullableObj< Func >	getPrefixOperatorFunc		( TokenType op );
	ax_Obj< Func >			getOrAddPrefixOperatorFunc	( TokenType op );
	
	ax_Dict< ax_string, ax_Obj< MetaNode > >	children;

	virtual void 	OnStringReq		( ax_ToStringReq & req ) const;
	
	ax_string 		getTemplateInstanceName			( const ax_Array< RType > & templateParams );
	
	virtual	void	onParentCreateTemplateInstance	( ax_Obj< MetaNode > new_inst_parent, const ax_Array< RType > & templateParams ) {}
};

class Namespace : public MetaNode {
	ax_DefObject( Namespace, MetaNode )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Namespace( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Obj< Namespace > getOrAddNamespace	( const ax_string & name, LexerPos & pos );
};

class TypeNode : public MetaNode {
	ax_DefObject( TypeNode, MetaNode );
public:
	TypeNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	virtual	bool canAssignFrom( ax_Obj< TypeNode > rhs ) const {
		return true;
	}
	
	DeclarationModifier		modifier;
	
	ax_Obj< TypeNode >		getOrAddTemplateInstance( const ax_Array< RType > & templateParams );
	
	void	OnStringReq( ax_ToStringReq & req ) const;
	
	ax_Array_< RType >						templateParams;
	ax_Dict< ax_string, ax_Obj<TypeNode> >	templateInstance;
	
};


class PrimitiveType : public TypeNode {
	ax_DefObject( PrimitiveType, TypeNode );
public:
	PrimitiveType( ax_NullableObj< MetaNode > parent, const ax_string & name )
	: base( parent, LexerPos(), name )
	{ buildin = true; }	
};


class TupleType : public TypeNode {
	ax_DefObject( TupleType, TypeNode )
public:

	TupleType( const LexerPos & pos, const ax_string & name, const ax_Array< RType > & elementTypes_ );
	
	ax_Array_< RType, 8 >		elementTypes;
	
};

class TupleTypeTable : System::NonCopyable {
public:

	ax_NullableObj< TupleType >	getTuple		( const ax_Array< ax_Obj<TypeNode> > & elementTypes );
			ax_Obj< TupleType >	getOrAddTuple	( const LexerPos & pos, const ax_Array< RType > & elementTypes );

	ax_string		getTupleName( const ax_Array< RType > & elementTypes );

private:
	ax_Dict< ax_string, ax_Obj<TupleType> >	tuples;
};

class Typename : public TypeNode {
	ax_DefObject( Typename, TypeNode )
public:
	Typename( const ax_NullableObj<MetaNode> parent, const LexerPos & pos, const ax_string & name );
};

class StructType : public TypeNode {
	ax_DefObject( StructType, TypeNode )
public:
	StructType( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Array_< LexerPos >	baseOrInterfacePos;

	LexerPos				bodyPos;
	
	ax_NullableObj< StructType >		baseType;
	ax_Array_< ax_Obj< StructType > >	interfaces;
			
	bool			isNestedType;
};

class Interface : public StructType {
	ax_DefObject( Interface, StructType );
public:
	Interface( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};

class Struct : public StructType {
	ax_DefObject( Struct, StructType );
public:
	Struct( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};

class Class : public StructType {
	ax_DefObject( Class, StructType );
public:
	Class( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};

class Prop : public MetaNode {
	ax_DefObject( Prop, MetaNode );
public:
	Prop( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name, bool is_let );
	
	LexerPos					initExprPos;
	ax_NullableObj< ExprAST >	initExpr;
	
	bool		is_let;
	LexerPos	typePos;
	RType		type;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		base::OnStringReq( req );
		req << ax_txt(" type=") << type;		
	}
};

struct FuncParam {
	struct	ax_type_on_gc_trace : public std::true_type {};

	FuncParam() : opt(false) {}

	LexerPos	namePos;
	ax_string	name;
	
	LexerPos	typePos;
	RType		type;
	
	ax_NullableObj< ExprAST >	defaultValueExpr;
		
	bool		opt;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		req << name << ax_txt(":") << type;
	}
};

class FuncType : public TypeNode {
	ax_DefObject( FuncType, TypeNode );
public:
	FuncType( const ax_string & name, ax_Obj< Func > func );
	ax_Obj< Func >	func;
		
};

class FuncOverload : public TypeNode {
	ax_DefObject( FuncOverload, TypeNode )
public:
	FuncOverload( ax_Obj< Func > parent, const LexerPos & pos );

	virtual void OnStringReq( ax_ToStringReq & req ) const;

	bool	isMatch		( const ax_Array<FuncParam> & callParams );

	ax_Obj< FuncOverload >	cloneTemplateInstance( ax_Obj< Func > new_func );

	FuncParam &	addParam( const ax_string & name, const LexerPos & namePos, const RType & type, const LexerPos & typePos );

	ax_Obj< Func >	func;

	ax_Array_< FuncParam, 8 >	params;
	
	LexerPos		returnTypePos;
	LexerPos		paramPos;
	LexerPos		bodyPos;
	
	RType			returnType;
};

class Func : public TypeNode {
	ax_DefObject( Func, TypeNode );
public:
	Func( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	virtual void OnStringReq( ax_ToStringReq & req ) const;
	
	ax_NullableObj< FuncOverload >		getOverload	( ax_Array< ax_Obj< FuncOverload > > & candidate, const ax_Array< FuncParam > & params );
	
	
	ax_Array_< ax_Obj< FuncOverload > >		overloads;
};

}} //namespace

#endif /* defined(__axc__MetaNode__) */
