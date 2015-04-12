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
#include "AST.h"

namespace ax {
namespace Compile {

#define DefMetaNode(T,B)	\
	ax_DefObject(T,B);	\
public:	\
	T( ax_NullableObj< MetaNode > parent, const ax_string & name, const Location & pos ) \
	: base( parent, name, pos ) { \
		onInit(); \
	} \
	void onInit(); \
	virtual	ax_Obj<MetaNode> clone ( ax_Obj< MetaNode > new_parent ) { \
		auto p = ax_new_obj( T, new_parent, name(), pos ); \
		p->onDeepCopy( ax_ThisObj ); \
		return p; \
	} \
	virtual	ax_Obj<MetaNode> clone ( const ax_string & new_name ) { \
		auto p = ax_new_obj( T, parent, new_name, pos ); \
		p->onDeepCopy( ax_ThisObj ); \
		return p; \
	} \
	virtual void onDeepCopy( ax_Obj<MetaNode> p ) { \
		B::onDeepCopy( p ); \
		T::onCopy( p->ax_cast<T>() ); \
	} \
	void onCopy( ax_Obj<T> p ); \
//----------

extern ax_string	k_ctor_name;

class Func;
class AST;

class TemplateParam;
class TemplateReplaceReq;

class Location;
class MetaNode : public System::Object {
	ax_DefObject( MetaNode, System::Object )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	MetaNode( ax_NullableObj< MetaNode > parent, const ax_string & name, const Location & pos );

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
	
	Location			pos;
	bool				buildin;
	bool				isTemplateInstance;
		
		ax_string	fullname		() const { return getFullname( ax_txt(".") ); }
		ax_string	getFullname		( const ax_string & seperator ) const;
		void		appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const;


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
	
	ax_string 		getTemplateInstanceName		( const ax_Array< Type > & templateParams );
	
	template< typename REQ >
	void	onDeepVisit( REQ & req ) {
		onVisit( req );
		ax_foreach( &c, children ) {
			c->onDeepVisit( req );
		}
	}
	
	virtual	void onVisit( TemplateReplaceReq & req ) {}
	
	virtual	ax_Obj<MetaNode> clone ( ax_Obj< MetaNode > new_parent ) = 0;
	virtual	ax_Obj<MetaNode> clone ( const ax_string & new_name ) = 0;
				
			void	onDeepInit() { onInit(); }
			void	onInit();
	
	virtual	void 	onDeepCopy	( ax_Obj<MetaNode> p ) { onCopy(p); }
			void 	onCopy		( ax_Obj<MetaNode> p );
};

class Namespace : public MetaNode {
	DefMetaNode( Namespace, MetaNode )

	ax_Obj< Namespace > getOrAddNamespace	( const ax_string & name, Location & pos );

};

class TypeSpec : public MetaNode {
	DefMetaNode( TypeSpec, MetaNode );
	
	DeclarationModifier						modifier;
	
	 ax_Obj< TemplateParam >				 addTemplateParam( const ax_string & name, const Location & pos );
	
	ax_Array_< ax_Obj< TemplateParam > >	_templateParams;
	
	ax_Dict< ax_string, ax_Obj<TypeSpec> >	templateInstance;
	
	virtual	bool canAssignFrom( ax_Obj< TypeSpec > rhs ) const {
		return true;
	}
		
	ax_Obj< TypeSpec >		getOrAddTemplateInstance( const ax_Array< Type > & params, const Location & pos );
	
	virtual	void onVisit( TemplateReplaceReq & req );
	
	void	OnStringReq( ax_ToStringReq & req ) const;
};

class TemplateParam : public TypeSpec {
	DefMetaNode( TemplateParam, TypeSpec )
	
	virtual void OnStringReq( ax_ToStringReq & req ) const;
	
	Type	type;
};

class TemplateReplaceReq : public System::NonCopyable {
public:
	ax_Dict< ax_Obj<MetaNode>, Type > dict;
	
	TemplateReplaceReq &	operator << ( Type & rhs );
	TemplateReplaceReq &	operator << ( ax_Obj< TemplateParam > rhs );
};


class PrimitiveTypeSpec : public TypeSpec {
	DefMetaNode( PrimitiveTypeSpec, TypeSpec );	
};

class TypenameSpec : public TypeSpec {
	DefMetaNode( TypenameSpec, TypeSpec )
	
	void onInit_Typename();
};

class CompositeTypeSpec : public TypeSpec {
	DefMetaNode( CompositeTypeSpec, TypeSpec )
	
	ax_Array_< Location >				baseOrInterfacePos;

	Location							bodyPos;
	
	ax_NullableObj< CompositeTypeSpec >		baseType;
	ax_Array_< ax_Obj< CompositeTypeSpec > >	interfaces;
			
	bool			isNestedType;
};

class Interface : public CompositeTypeSpec {
	DefMetaNode( Interface, CompositeTypeSpec );	
};

class Struct : public CompositeTypeSpec {
	DefMetaNode( Struct, CompositeTypeSpec );
};

class Class : public CompositeTypeSpec {
	DefMetaNode( Class, CompositeTypeSpec );
};

class Prop : public MetaNode {
	DefMetaNode( Prop, MetaNode );
	
	Location					initExprPos;
	ax_NullableObj< AST >	initExpr;
	
	bool		is_let;
	Location	typePos;
	Type		type;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		base::OnStringReq( req );
		req << ax_txt(" type=") << type;		
	}
};

struct FuncParam {
	struct	ax_type_on_gc_trace : public std::true_type {};

	FuncParam() : opt(false) {}

	Location	namePos;
	ax_string	name;
	
	Location	typePos;
	Type		type;
	
	ax_NullableObj< AST >	defaultValueExpr;
		
	bool		opt;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		req << name << ax_txt(":") << type;
	}
};

class FuncType : public TypeSpec {
	DefMetaNode( FuncType, TypeSpec );
	
	ax_Obj< Func >	func;
};

class FuncOverload : public TypeSpec {
	DefMetaNode( FuncOverload, TypeSpec )

	virtual void OnStringReq( ax_ToStringReq & req ) const;

	bool	isMatch		( const ax_Array<FuncParam> & callParams );

	FuncParam &	addParam( const ax_string & name, const Location & namePos, const Type & type, const Location & typePos );

	ax_Array_< FuncParam, 8 >	params;
	
	Location		returnTypePos;
	Location		paramPos;
	Location		bodyPos;
	
	Type			returnType;
	
	ax_Obj<Func>	func;

	void		onVisit( TemplateReplaceReq & req );
};

class Func : public TypeSpec {
	DefMetaNode( Func, TypeSpec );

	ax_NullableObj< FuncOverload >		getOverload	( ax_Array< ax_Obj< FuncOverload > > & candidate, const ax_Array< FuncParam > & params );
	
	ax_Obj< FuncOverload >	addOverload( const Location & pos );
	
	ax_Array_< ax_Obj< FuncOverload > >		overloads;	
};

}} //namespace

#endif /* defined(__axc__MetaNode__) */
