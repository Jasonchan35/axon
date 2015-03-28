//
//  Dict.h
//  ax_core
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Dict_h
#define ax_core_Dict_h

#include "Intrusive_LinkedList.h"
#include "UPtr.h"
#include "../array/Array.h"
#include "Nullable.h"
#include "Obj.h"

namespace ax {
namespace System {

template< typename KEY, typename VALUE >
class Dict : public Object {
public:
	struct	ax_type_on_gc_trace : public std::false_type {};

	class	Pair : public Intrusive_LinkedListNode< Pair > {
	public:
		struct	ax_type_on_gc_trace : public std::false_type {};
	
		Pair( ax_int hash, const KEY & key, const VALUE &  value ) : _hash(hash), _key(key), value(value) {}
		Pair( ax_int hash, const KEY & key,       VALUE && value ) : _hash(hash), _key(key), value( ax_move(value) ) {}

		const	KEY &	key		() const { return _key;  }
				ax_int	hash	() const { return _hash; }
					
		void	OnStringReq( ToStringReq & req ) const {
			req << _key << ax_txt(":") << value;
		}
		
		struct	HashNode : public Intrusive_LinkedListNode< HashNode > {
			struct	ax_type_on_gc_trace : public std::false_type {};
			
			Pair&	pair			() 	{ return ax_member_owner( &Pair::_hashNode, this ); }
			bool	onIsOwnedByList	()	{ return false; }
		};
		
	friend class Dict;
	protected:
		HashNode	_hashNode;
		ax_int		_hash;
		KEY			_key;
	public:
		VALUE		value;
	};
	
	typedef	Intrusive_LinkedList< Pair >					PairList;
	typedef	Intrusive_LinkedList< typename Pair::HashNode >	HashList;
	
	Dict() : _size(0) {}
	virtual ~Dict() { release(); }
	
	ax_int		size		() const 	{ return _size; }
	ax_int		tableSize	() const	{ return _table.size(); }
	
	void	release	()	{
		clear();
		_table.release();
	}
	
	void	clear		() {
		_table.clear();
		_pairs.clear();
		_size = 0;
	}
	
	void	set		( const KEY & key, const VALUE & value ) {
		auto h = ax_hash_of(key);
		auto & list = _listForAdd( h );
		
		auto p = _getPairFromList( list, h, key );
		if( !p ) { throw Err_Dict_NoSuchKey(); }
		
		p->value = value;
	}
	
	void	setOrAdd( const KEY & key, const VALUE & value ) {
		auto h = ax_hash_of(key);
		auto & list = _listForAdd( h );
		
		auto p = _getPairFromList( list, h, key );
		if( p ) {
			p->value = value;
			return;
		}
		
		auto u = ax_uptr_new( Pair, h, key, value );
		list.add( & u->_hashNode );
		_pairs.add( u.ptr() );
		_size++;
		u.giveup();
	}
		
	void	add( const KEY & key, const VALUE &  value ) {
		auto h = ax_hash_of(key);
		auto & list = _listForAdd( h );
		
		if( _getPairFromList( list, h, key ) ) { throw Err_Dict_DuplicateKey(); }
		
		auto u = ax_uptr_new( Pair, h, key, value );
		list.add( & u->_hashNode );
		_pairs.add( u.ptr() );
		_size++;
		u.giveup();
	}
	
	bool	remove( const KEY key ) {
		auto p = tryGetPair( key );
		if( !p ) return false;
		
		_remove( p );
		ax_delete_uncollect( p );
		return true;
	}
	
	ax_ALWAYS_INLINE(	 VALUE&	operator[] 	( const KEY & key ) 	) {
		return getValue( key );
	}
	
	ax_ALWAYS_INLINE(	 VALUE &	getValue	( const KEY & key ) 	){
		auto p = tryGetPair( key );
		if( !p ) throw Err_Dict_NoSuchKey();
		return p->value;
	}

	ax_ALWAYS_INLINE(	 bool	hasKey( const KEY & key ) 	) {
		return tryGetPair( key ) != nullptr;
	}
	
	ax_ALWAYS_INLINE(	 Nullable<VALUE>	tryGetValue( const KEY & key ) 	) {
		Nullable<VALUE>	o;
		auto p = tryGetPair( key );
		if( p ) o = p->value;
		return o;
	}
	
	ax_ALWAYS_INLINE(	 Pair*	tryGetPair( const KEY & key ) 	) {
		auto h  = ax_hash_of( key );
		auto ts = _table.size();
		if( ts == 0 ) return nullptr;
		
		auto & list = GetHashList( h );
		return _getPairFromList( list, h, key );
	}
		
	void	setTableSize( ax_int n ) {
		if( n == tableSize() ) return;
		_setTableSize( n );
	}
	
	void	OnStringReq( ToStringReq & req ) const {
		req << ax_txt("[");
		ax_int i=0;
		ax_foreach( & p, pairs() ) {
			if( i>0 ) req << ax_txt(", ");
			req << p.key() << ax_txt(":") << p.value;
			i++;
		}
		req << ax_txt("]");
	}

		
//--------------------
	DebugEnumeratingCounter		_enumerating;

	template< typename D, typename P >
	class PairEnumerator {
		D*	_dict;
	public:
		PairEnumerator( D* dict ) : _dict(dict) { ++_dict->_enumerating; }
		~PairEnumerator() { --_dict->_enumerating; }

		class	iterator {
		public:
			iterator( P* p=nullptr ) : p_(p) {}
			operator P*			()  { return  p_; }
			P&		operator*	()	{ return *p_; }
			P*		operator->	()	{ return  p_; }
			void	operator++	()	{ if(p_) { p_ = p_->_list_next; } }
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
			
			P*		_ptr() { return p_; }
		private:
			P*	p_;
		};

		iterator	begin	()			{ return iterator( _dict->_pairs.head() ); }
		iterator	end		()			{ return iterator( nullptr ); }
	};
	
	template< typename D, typename P >
	class Pairs {
		D*	_dict;
	public:
		Pairs ( D* dict ) : _dict(dict) {
		#if ax_DEBUG_Enumerating
			++_dict->_enumerating;
		#endif
		}
		~Pairs() {
		#if ax_DEBUG_Enumerating
			--_dict->_enumerating;
		#endif
		}
	
		PairEnumerator<       D,       P >	getEnumerator()			{ return PairEnumerator<       D,       P >( _dict ); }
		PairEnumerator< const D, const P >	getEnumerator()	const 	{ return PairEnumerator< const D, const P >( _dict ); }
	};
	
    Pairs< Dict,       Pair >	pairs()	      { return Pairs< Dict,       Pair >( this ); }
	Pairs< Dict, const Pair >	pairs()	const { return Pairs< Dict, const Pair >( ax_const_cast(this) ); }
	
	
//--------------------

	template< typename D, typename P, typename K >
	class KeyEnumerator {
		D*	_dict;
	public:
		KeyEnumerator( D* dict ) : _dict(dict)	{ ++_dict->_enumerating; }
		~KeyEnumerator()						{ --_dict->_enumerating; }

		class	iterator {
		public:
			iterator( P* p=nullptr ) : p_(p) {}
			operator K*			()  { return &p_->key(); }
			K&		operator*	()	{ return  p_->key(); }
			K*		operator->	()	{ return &p_->key(); }
			void	operator++	()	{ if(p_) { p_ = p_->_list_next; } }
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
			
			K*		_ptr() { return p_; }
		private:
			P*	p_;
		};

		iterator	begin	()			{ return iterator( _dict->_pairs.head() ); }
		iterator	end		()			{ return iterator( nullptr ); }		
	};
	
	template< typename D, typename P, typename K >
	class Keys {
		D*	_dict;
	public:
		Keys ( D* dict ) : _dict(dict)	{ ++_dict->_enumerating; }
		~Keys() 						{ --_dict->_enumerating; }
	
		KeyEnumerator< D,P,K >	getEnumerator()		{ return KeyEnumerator< D,P,K >( _dict ); }
	};
	
	Keys< Dict,       Pair,       KEY >	keys()	      { return Keys< Dict,       Pair,       KEY >(this); }
	Keys< Dict, const Pair, const KEY >	keys()	const { return Keys< Dict, const Pair, const KEY >( ax_const_cast(this) ); }

//--------------------
	template< typename D, typename P, typename V >
	class ValueEnumerator {
		D*	_dict;
	public:
		ValueEnumerator( D* dict ) : _dict(dict) { ++_dict->_enumerating; }
		~ValueEnumerator()						 { --_dict->_enumerating; }

		class	iterator {
		public:
			iterator( P* p=nullptr ) : p_(p) {}
			operator V*			()  { return &p_->value; }
			V&		operator*	()	{ return  p_->value; }
			V*		operator->	()	{ return &p_->value; }
			void	operator++	()	{ if(p_) { p_ = p_->_list_next; } }
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
			
			V*		_ptr() { return p_; }
		private:
			P*	p_;
		};

		iterator				begin	()			{ return iterator( _dict->_pairs.head() ); }
		iterator				end		()			{ return iterator( nullptr ); }
	};
	
	template< typename D, typename P, typename V >
	class Values {
		D*	_dict;
	public:
		Values ( D* dict ) : _dict(dict)	{ ++_dict->_enumerating; }
		~Values() 							{ --_dict->_enumerating; }
	
		ValueEnumerator< D,P,V >	getEnumerator()		{ return ValueEnumerator< D,P,V >( _dict ); }
	};

	Values< Dict,       Pair,       VALUE >	values()			{ return Values< Dict,       Pair,       VALUE >( this ); }
	Values< Dict, const Pair, const VALUE >	values()	const 	{ return Values< Dict, const Pair, const VALUE >( ax_const_cast(this) ); }
	
	
	ValueEnumerator< Dict,       Pair,       VALUE >	getEnumerator()			{ return ValueEnumerator< Dict,       Pair,       VALUE >( this ); }
	ValueEnumerator< Dict, const Pair, const VALUE >	getEnumerator()	const 	{ return ValueEnumerator< Dict, const Pair, const VALUE >( ax_const_cast(this) ); }
	
		
private:
	typedef	Array_< HashList, 0 >		Table;
	
	ax_int		_size;

	PairList	_pairs; // order base on add
	Table		_table; // please use GetHashList instead of _table[]
	
	HashList &	GetHashList( ax_int hash_code ) {
	  	return _table[ ax_type_to_unsigned(hash_code) % ax_type_to_unsigned( _table.size() ) ];
	}
	
	void	_remove( Pair* p ) {
		p->_hashNode->_list->remove( & p->_hashNode );
		_pairs.remove( p );
		_size--;
	}
	
	HashList &	_listForAdd( ax_int hash ) {
		auto ts = tableSize();
		if( ts == 0 ) {
			ts = 1;
		}else if( ts < _size / 8 + 1 ) {
			ts = _size / 4;
		}
		setTableSize( ts );
		return GetHashList( hash );
	}

	Pair*	_getPairFromList( HashList & list, ax_int hash, const KEY & key ) {
		ax_foreach( & node, list ) {
			auto & p = node.pair();
			if( p._hash != hash ) continue;
			if( p._key  == key  ) return &p;
		}
		return nullptr;
	}
		
	void	_setTableSize( ax_int n ) {
		if( n <= 0 && _size > 0 ) {
			throw Err_Undefined();
		}

		ax_foreach( & t, _table ) {
			t.removeAll();
		}
		
		_table.resize( n );
		
		ax_foreach( & p, _pairs ) {
			auto & list = GetHashList( p._hash );
			list.add( &p._hashNode );
		}		
	}
		
};

template< typename KEY, typename VALUE > using DictObj = Obj< Dict< KEY, VALUE > >;


}} //namespace

#endif
