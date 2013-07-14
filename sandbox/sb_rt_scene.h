//
//  sb_rt_scene.h
//  caleydoscope
//
//  Created by Andrey Kunitsyn on 7/6/13.
//  Copyright (c) 2013 andryblack. All rights reserved.
//

#ifndef __caleydoscope__sb_rt_scene__
#define __caleydoscope__sb_rt_scene__

#include "sb_scene.h"
#include "sb_rendertatget.h"

namespace GHL {
    struct Render;
}

namespace Sandbox {
    
    class RTScene : public Scene {
        SB_META_OBJECT
    public:
        explicit RTScene(const RenderTargetPtr& rt);
        void Draw(GHL::Render* r,Graphics& g);
        const RenderTargetPtr& GetTarget() const { return m_rt;}
    private:
        RenderTargetPtr m_rt;
    };
    typedef sb::shared_ptr<RTScene> RTScenePtr;
}

#endif /* defined(__caleydoscope__sb_rt_scene__) */
