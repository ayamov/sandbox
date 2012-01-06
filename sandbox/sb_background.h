//
//  sb_background.h
//  pairs
//
//  Created by Андрей Куницын on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef pairs_sb_background_h
#define pairs_sb_background_h

#include "sb_object.h"
#include "sb_image.h"

namespace Sandbox {
    
    class Resources;
    
    class Background : public Object {
    public:
        Background();
        bool Load( const char* file, Resources* res );
        void Draw( Graphics& g ) const;
    private:
        std::vector<Image>  m_images;
    };
    
}

#endif