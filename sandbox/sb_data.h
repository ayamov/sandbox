//
//  sb_data.h
//  backgammon-osx
//
//  Created by Andrey Kunitsyn on 12/29/12.
//  Copyright (c) 2012 Andrey Kunitsyn. All rights reserved.
//

#ifndef backgammon_osx_sb_data_h
#define backgammon_osx_sb_data_h

#include "sb_vector.h"
#include "sb_ref_cntr.h"
#include <ghl_data.h>

namespace Sandbox {
    
    template <class T>
    class VectorData : public RefCounter<GHL::Data> {
    private:
        sb::vector<T>   m_data;
    public:
        VectorData(){}
        explicit VectorData( const sb::vector<T>& data ) : m_data(data) {
            
        }
        sb::vector<T>& vector() { return m_data; }
        const sb::vector<T>& vector() const { return m_data; }
        
        /// Data size
		virtual GHL::UInt32 GHL_CALL	GetSize() const {
            return GHL::UInt32(m_data.size() * sizeof(T));
        }
		/// Const data ptr
		virtual const GHL::Byte* GHL_CALL	GetData() const {
            return reinterpret_cast<const GHL::Byte*>(&m_data[0]);
        }
		/// set data
		virtual void GHL_CALL	SetData( GHL::UInt32 offset, const GHL::Byte* data, GHL::UInt32 size ) {
            sb_assert((offset%sizeof(T))==0);
            sb_assert((size%sizeof(T))==0);
            const T* src = reinterpret_cast<const T*>(data);
            size_t nsize = offset/sizeof(T)+size/sizeof(T);
            if (m_data.size()<nsize) {
                m_data.resize(nsize);
            }
            std::copy(src,src+size/sizeof(T),m_data.begin()+offset/sizeof(T));
        }
        /// clone data
        virtual GHL::Data* GHL_CALL  Clone() const {
            return new VectorData(m_data);
        }
    };
    
}

#endif
