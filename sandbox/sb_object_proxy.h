/*
 *  sb_object_proxy.h
 *  SR
 *
 *  Created by Андрей Куницын on 23.02.11.
 *  Copyright 2011 andryblack. All rights reserved.
 *
 */

#ifndef SB_OBJECT_PROXY_H
#define SB_OBJECT_PROXY_H

#include "sb_object.h"
#include "sb_function.h"
#include "sb_vector2.h"

namespace Sandbox {
	
	class ObjectDrawProxy : public Object {
	public:
  
		explicit ObjectDrawProxy(const function<void(Graphics&)>& func);
		void Draw(Graphics& g) const;
        
  	private:
		function<void(Graphics&)> m_func;
	};
	typedef shared_ptr<ObjectDrawProxy> ObjectDrawProxyPtr;
    
    
    class ObjectTouchProxy : public Object {
	public:
        typedef function<bool(TouchInfo)> TouchFunc;
        void Draw(Graphics& g) const {}
		explicit ObjectTouchProxy(const TouchFunc& func);
		
        /// self mouse handling implementation
        bool HandleTouch( const TouchInfo& touch );
	private:
        TouchFunc   m_touch_func;
  	};
	typedef shared_ptr<ObjectTouchProxy> ObjectTouchProxyPtr;
}

#endif /*SB_OBJECT_PROXY_H*/
