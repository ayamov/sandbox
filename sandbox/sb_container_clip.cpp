#include "sb_container_clip.h"

SB_META_DECLARE_OBJECT(Sandbox::ContainerClip, Sandbox::Container)

namespace Sandbox {
    
    ContainerClip::ContainerClip() {
        
    }
    
    void ContainerClip::Draw(Graphics &g) const {
        const Transform2d& tr = g.GetTransform();
        Recti clip = g.GetClipRect();
        Recti nclip = m_rect;
        Vector2f lt = tr.transform(nclip.GetTopLeft());
        Vector2f rb = tr.transform(nclip.GetBottomRight());
        nclip.x = std::max(0.0f,std::min(lt.x, rb.x));
        nclip.y = std::max(0.0f,std::min(lt.y, rb.y));
        nclip.w = std::max(lt.x, rb.x) - nclip.x;
        nclip.h = std::max(lt.y, rb.y) - nclip.y;
        if (nclip.w > 0 && nclip.h > 0) {
            nclip = nclip.GetIntersect(clip);
            if (nclip.w > 0 && nclip.h > 0) {
                g.SetClipRect(nclip);
                Container::Draw(g);
                g.SetClipRect(clip);
            }
        }
    }
    
}
