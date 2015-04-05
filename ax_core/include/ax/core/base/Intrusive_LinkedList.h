//
//  Intrusive_LinkedList.h
//  ax_core
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Intrusive_LinkedList_h
#define ax_core_Intrusive_LinkedList_h

#include "Exception.h"
#include "Memory.h"

namespace ax {
namespace System {

template< typename T > class Intrusive_LinkedList;

//! Intrusive doubly linked list
template< typename T >
class Intrusive_LinkedListNode : public NonCopyable {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Intrusive_LinkedListNode()
	: _list_next( nullptr )
	, _list_prev( nullptr )
	, _list( nullptr )
	{}
	
	void	onWillRemoveFromList() 	{}
	bool	onIsOwnedByList		()	{ return true; }

	typedef	Intrusive_LinkedList<T>		List;

	T*		_list_next;
	T*		_list_prev;
	List*	_list;
};

template< typename T >
class Intrusive_LinkedList : public NonCopyable {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	typedef	Intrusive_LinkedListNode<T>	Node;

	Intrusive_LinkedList() { _ctor(); }

	Intrusive_LinkedList( Intrusive_LinkedList && rhs ) { _ctor(); addRange_move( rhs ); }

	~Intrusive_LinkedList() { release(); }
	
	void operator=( Intrusive_LinkedList && rhs ) 		{ clear(); addRange_move( rhs ); }
	
		  T*	head	() 			{ return _head; }
	const T*	head	() const 	{ return _head; }

		  T*	tail	() 			{ return _tail; }
	const T*	tail	() const	{ return _tail; }
	
	
	ax_int		size	() const { return _size; }

	ax_ALWAYS_INLINE(	void	insert			( T* p )				);
	ax_ALWAYS_INLINE(	void	insert			( T* p, T* before )		);
	
	ax_ALWAYS_INLINE(	void	add				( T* p )				);
	ax_ALWAYS_INLINE(	void	add				( T* p, T* after )		);
	ax_ALWAYS_INLINE(	void	addRange_move	( Intrusive_LinkedList & rhs ) );

	ax_ALWAYS_INLINE(	T*		removeHead		()						);

	ax_ALWAYS_INLINE(	void	remove			( T* p )				);
	ax_ALWAYS_INLINE(	void	removeAll		()						);
	
	ax_ALWAYS_INLINE(	void	clear			()						);
	ax_ALWAYS_INLINE(	void	release			()						) { clear(); }

	template< typename A >
	class Enumerator {
	public:
		Enumerator( Intrusive_LinkedList* list ) : _list(list) {}
	
		class	iterator {
		public:
			iterator( A* p=nullptr ) : p_(p) {}
			operator A*			()  { return  p_; }
			A&		operator*	()	{ return *p_; }
			A*		operator->	()	{ return  p_; }
			void	operator++	()	{ if(p_) { p_ = p_->Node::_list_next; } }
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
			
			A*		_ptr() { return p_; }
		private:
			A*	p_;
		};

			iterator	begin	()			{ return iterator( _list->_head ); }
			iterator	end		()			{ return iterator( nullptr ); }
	private:
		Intrusive_LinkedList*	_list;
	};
	
	Enumerator<       T >	getEnumerator() 		{ return Enumerator<       T >( this ); }
	Enumerator< const T >	getEnumerator() const	{ return Enumerator< const T >( ax_const_cast(this) ); }
	

private:
	void	_ctor() {
		_size = 0;
		_head = nullptr;
		_tail = nullptr;
	}

	ax_int	_size;
	T*		_head;
	T*		_tail;
};

template< typename T > inline
T*	Intrusive_LinkedList<T>::removeHead	() {
	auto p = _head;
	if( p ) remove(p);
	return p;
}

template< typename T > inline
void	Intrusive_LinkedList<T>::addRange_move	( Intrusive_LinkedList & rhs ) {
	for(;;) {
		auto p = rhs.removeHead();
		if( !p ) break;
		add(p);
	}
}

template< typename T > inline
void	Intrusive_LinkedList<T>::clear	() {
	while( _head ) {
		auto p = _head;
		remove( p );
		if( p->onIsOwnedByList() ) {
			ax_delete( p );
		}
	}
}

template< typename T > inline
void	Intrusive_LinkedList<T>::removeAll	() {
	while( _head ) {
		remove( _head );
	}
}

template< typename T > inline
void	Intrusive_LinkedList<T>::insert	( T* p ) {
	if( ! p ) throw Err_Undefined();
	if( p->_list || p->_list_prev || p->_list_next ) throw Err_Undefined();
	
	p->_list_next = _head;
	p->_list_prev = nullptr;
	if( _head ) {
		_head->_list_next = p;
		_head = p;
	}else{
		_head = _tail = p;
	}

	p->_list = this;
	_size++;
}

template< typename T > inline
void	Intrusive_LinkedList<T>::add	( T* p ) {
	if( ! p ) throw Err_Undefined();
	if( p->_list || p->_list_prev || p->_list_next ) throw Err_Undefined();
	
	p->_list_next = nullptr;
	p->_list_prev = _tail;
	if( _tail ) {
		_tail->_list_next = p;
		_tail = p;
	}else{
		_head = _tail = p;
	}

	p->_list = this;
	_size++;
}

template< typename T > inline
void	Intrusive_LinkedList<T>::insert	( T* p, T* before ) {
	if( ! before ) return insert(p);
	
	if( ! p ) throw Err_Undefined();
	if( p->_list || p->_list_prev || p->_list_next ) throw Err_Undefined();

	if( before->_list != this ) throw Err_Undefined();
	
	p->_list_next = before;
	p->_list_prev = before->_prev_;
	
	if( before->_list_prev ) {
		before->_list_prev->_list_next = p;
	}else{
		_head = p;
	}

	p->_list = this;
	_size++;
}


	
template< typename T > inline
void	Intrusive_LinkedList<T>::add	( T* p, T* after ) {
	if( ! after ) return add( p );
	
	if( ! p ) throw Err_Undefined();
	if( p->_list || p->_list_prev || p->_list_next ) throw Err_Undefined();
	
	if( after->_list != this ) throw Err_Undefined();
	
	p->onWillRemoveFromList();

	p->_list_next = after->_list_next;
	p->_list_prev = after;
	
	if( after->_list_next ) {
		after->_list_next->_list_prev = p;
	}else{
		_tail = p;
	}

	p->_list = this;
	_size++;
}

template< typename T > inline
void	Intrusive_LinkedList<T>::remove	( T* p ) {
	if( ! p ) throw Err_Undefined();
	if( p->_list != this ) throw Err_Undefined();
	
	if( p->_list_prev ) {
		p->_list_prev->_list_next = p->_list_next;
	}else{
		_head = p->_list_next;
	}
	
	if( p->_list_next ) {
		p->_list_next->_list_prev = p->_list_prev;
	}else{
		_tail = p->_list_prev;
	}
	
	p->_list_next = nullptr;
	p->_list_prev = nullptr;
	p->_list 	  = nullptr;
	
	assert( _size > 0 );
	_size--;
}

}} //namespace

#endif
