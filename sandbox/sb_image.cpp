#include "sb_image.h"
#include "sb_bitmask.h"

namespace Sandbox {
    
    bool Image::CheckBit(int x,int y,Resources* resources) {
        x+=m_hotspot.x*m_width/m_src_w;
        if (x<0)
            return false;
        y+=m_hotspot.y*m_height/m_src_h;
        if (y<0)
            return false;
        if (x>m_width)
            return false;
        if (y>m_height)
            return false;
        x = m_src_x + x * m_src_w / m_width;
        y = m_src_y + y * m_src_h / m_height;
        if (!m_texture) return false;
        BitmaskPtr bm = m_texture->GetBitmask(resources);
        if (!bm)
            return false;
        return bm->Get(x, y);
    }
    
    sb::intrusive_ptr<Image> Image::Clone() const {
        return sb::intrusive_ptr<Image>(new Image(*this));
    }
}