//
//  DeclarationModifier.h
//  axc
//
//  Created by Jason Chan on 12/10/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__DeclarationModifier__
#define __axc__DeclarationModifier__

namespace ax {
namespace Compile {


class DeclarationModifier {
public:

	DeclarationModifier() {
		reset();
	}
	
	void	reset() {
		b_mutating		= false;
		b_nonmutating	= false;
		
		b_static 		= false;
		b_extern 		= false;
		
		b_virtual		= false;
		b_override		= false;
		
		b_unowned		= false;
		b_weak			= false;
	}
	
	bool	isEmpty() {
		if( b_mutating 		) return false;
		if( b_nonmutating 	) return false;
		if( b_static 		) return false;
		if( b_extern 		) return false;
		if( b_virtual 		) return false;
		if( b_override 		) return false;
		if( b_unowned 		) return false;
		if( b_weak 			) return false;
		return true;
	}
	
	bool	b_mutating		: 1;
	bool	b_nonmutating 	: 1;
	bool	b_static		: 1;
	bool	b_extern		: 1;
	bool	b_virtual		: 1;
	bool	b_override		: 1;
	bool	b_unowned		: 1;
	bool	b_weak			: 1;

};



}} //namespace

#endif /* defined(__axc__DeclarationModifier__) */
