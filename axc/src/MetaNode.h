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

class FuncNode;
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


	ax_string			name;
	LexerPos			pos;

	ax_NullableObj< MetaNode >	parent;
	
	TokenType			nodeType;
	bool		is_let		() { return nodeType == TokenType::t_let; }
	bool		is_var		() { return nodeType == TokenType::t_var; }
	
		ax_string	getFullname		( const ax_string & seperator ) const;
			void	appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const;


			ax_NullableObj< MetaNode >	getNode			( const ax_string & name );
	virtual	ax_NullableObj< MetaNode >	onGetNode		( const ax_string & name ) { return nullptr; }
	
			ax_NullableObj< MetaNode >	getMember		( const ax_string & name );
	virtual	ax_NullableObj< MetaNode >	onGetMember		( const ax_string & name );

	
	ax_NullableObj< FuncNode >	getFunc		( const ax_string & name );
	ax_Obj< FuncNode >			getOrAddFunc( const ax_string & name );
		
	ax_NullableObj< FuncNode >	getOperatorFunc				( TokenType op );
	ax_Obj< FuncNode >			getOrAddOperatorFunc		( TokenType op );

	ax_NullableObj< FuncNode >	getPrefixOperatorFunc		( TokenType op );
	ax_Obj< FuncNode >			getOrAddPrefixOperatorFunc	( TokenType op );
	
	typedef ax_Dict< ax_string, ax_Obj< MetaNode > >	ChildrenDict;
	
	ax_Obj<	ChildrenDict >	children;
	
	virtual void OnStringReq( ax_ToStringReq & req ) const;
};

class NamespaceNode : public MetaNode {
	ax_DefObject( NamespaceNode, MetaNode )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	NamespaceNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Obj< NamespaceNode > getOrAddNamespace	( const ax_string & name, LexerPos & pos );
};

class TypedNode : public MetaNode {
	ax_DefObject( TypedNode, MetaNode );
public:
	TypedNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	virtual	bool canAssignFrom( ax_Obj< TypedNode > rhs ) const {
		return true;
	}

	DeclarationModifier	modifier;
	
	bool	buildin;
};


class PrimitiveType : public TypedNode {
	ax_DefObject( PrimitiveType, TypedNode );
public:
	PrimitiveType( ax_NullableObj< MetaNode > parent, const ax_string & name )
	: base( parent, LexerPos(), name )
	{ buildin = true; }	
};


class TupleType : public TypedNode {
	ax_DefObject( TupleType, TypedNode )
public:

	TupleType( const LexerPos & pos, const ax_string & name, const ax_Array< ax_Obj< TypedNode > > & elementTypes_ );
	
	ax_Array_< ax_Obj<TypedNode>, 8 >		elementTypes;
	
};

class TupleTypeTable : System::NonCopyable {
public:

	ax_NullableObj< TupleType >	getTuple		( const ax_Array< ax_Obj<TypedNode> > & elementTypes );
			ax_Obj< TupleType >	getOrAddTuple	( const LexerPos & pos, const ax_Array< ax_Obj<TypedNode> > & elementTypes );

	ax_string		getTupleName( const ax_Array< ax_Obj<TypedNode> > & elementTypes );

private:
	ax_Dict< ax_string, ax_Obj<TupleType> >	tuples;
};


class StructureType : public TypedNode {
	ax_DefObject( StructureType, TypedNode )
public:
	StructureType( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Array_< LexerPos >	baseOrInterfacePos;

	LexerPos				bodyPos;
	
	ax_NullableObj< StructureType >		baseType;
	ax_Array_< ax_Obj< StructureType > >	interfaces;
	
	bool			isNestedType;
};

class InterfaceNode : public StructureType {
	ax_DefObject( InterfaceNode, StructureType );
public:
	InterfaceNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};

class StructNode : public StructureType {
	ax_DefObject( StructNode, StructureType );
public:
	StructNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};

class ClassNode : public StructureType {
	ax_DefObject( ClassNode, StructureType );
public:
	ClassNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) : base( parent, pos, name ) {}
};




class PropNode : public TypedNode {
	ax_DefObject( PropNode, TypedNode );
public:
	PropNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );
	
	LexerPos					initExprPos;
	ax_NullableObj< ExprAST >	initExpr;
	
	LexerPos	typePos;
	RType		type;
};

struct FuncParam {
	struct	ax_type_on_gc_trace : public std::true_type {};

	FuncParam() : opt(false) {}

	LexerPos	pos;
	ax_string	name;
	
	LexerPos	rtypePos;
	RType		rtype;
	bool		opt;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		req << name << ax_txt(":") << rtype;
	}
};

class FuncType : public TypedNode {
	ax_DefObject( FuncType, TypedNode );
public:
	FuncType( const ax_string & name, ax_Obj< FuncNode > func );
	ax_Obj< FuncNode >	func;
};

class FuncOverload : public TypedNode {
	ax_DefObject( FuncOverload, TypedNode )
public:
	FuncOverload( ax_Obj< FuncNode > parent, const LexerPos & pos );

	bool	isMatch		( const ax_Array<FuncParam> & callParams );

	ax_Array_< FuncParam, 8 >	params;
	
	ax_Obj< FuncNode >	func;

	LexerPos		returnTypePos;
	LexerPos		paramPos;
	LexerPos		bodyPos;
	
	RType			returnType;
};

class FuncNode : public TypedNode {
	ax_DefObject( FuncNode, TypedNode );
public:
	FuncNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );
	
						void			addOverload	( ax_Obj< FuncOverload > fo ) { overloads.add(fo); }
	ax_NullableObj< FuncOverload >		getOverload	( ax_Array< ax_Obj< FuncOverload > > & candidate, const ax_Array< FuncParam > & params );
	
	
	ax_Array_< ax_Obj< FuncOverload > >		overloads;
};

}} //namespace

#endif /* defined(__axc__MetaNode__) */
