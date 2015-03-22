//
//  Params.h
//  ax_core
//
//  Created by Jason on 2014-11-29.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_Params_h
#define ax_core_Params_h

#include "../array/ExternalArray.h"
#include "Ptr.h"
#include "ax_FuncParams.h"

namespace ax {
namespace System {

template< typename T >
class Params : public ExternalArray< Ptr< const T > > {
	typedef	Ptr< const T >			ARG;
	typedef ExternalArray< ARG >	base;
public:
	Params() {}
	Params( ARG* p, ax_int n ) : base( p, n ) {}
};

} //namespace System
} //namespace ax



#endif
