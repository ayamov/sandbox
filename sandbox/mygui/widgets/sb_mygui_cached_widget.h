//
//  sb_mygui_cached_widget.h
//  Sandbox
//
//  Created by Andrey Kunitsyn on 30/01/14.
//
//

#ifndef __Sandbox__sb_mygui_cached_widget__
#define __Sandbox__sb_mygui_cached_widget__

#include "MyGUI_Widget.h"
#include "meta/sb_meta.h"
#include "MyGUI_SharedLayerNode.h"

namespace Sandbox {
    
    namespace mygui {
        
        class CachedWidget : public MyGUI::Widget {
            MYGUI_RTTI_DERIVED( CachedWidget )
        public:
            CachedWidget();
            ~CachedWidget();
            
            virtual void onWidgetCreated(MyGUI::Widget* _widget);
            
            virtual void addChildItem(LayerItem* _item);
            virtual void removeChildItem(LayerItem* _item);
            virtual void addChildNode(LayerItem* _item);
            virtual void removeChildNode(LayerItem* _item);

            
        protected:
            void initialiseOverride();
            void shutdownOverride();
            virtual void setPropertyOverride(const std::string& _key, const std::string& _value);
            
            virtual void doRenderToTarget(MyGUI::IRenderTarget* rt);
        private:
            MyGUI::ITexture*        m_texture;
            
            MyGUI::SharedLayerNode*  m_replaced_layer;
            void frameEntered(float dt);
            std::string m_texture_name;
            
            bool    m_render_content;
        };
        
    }
    
}

#endif /* defined(__Sandbox__sb_mygui_cached_widget__) */
