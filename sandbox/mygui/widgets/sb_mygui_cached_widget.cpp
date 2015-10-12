//
//  sb_mygui_cached_widget.cpp
//  Sandbox
//
//  Created by Andrey Kunitsyn on 30/01/14.
//
//

#include "sb_mygui_cached_widget.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderManager.h"
#include "sb_math.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayerNode.h"
#include "../sb_mygui_render.h"
#include <sbstd/sb_platform.h>


SB_META_DECLARE_OBJECT(Sandbox::mygui::CachedWidget, MyGUI::Widget)

namespace Sandbox {
    
    namespace mygui {
        
        
        CachedWidget::CachedWidget() {
            m_rt = 0;
            m_texture = 0;
            m_replaced_layer = new MyGUI::SharedLayerNode(0,0);
            m_texture_name = get_type_info()->name;
            char buf[128];
			sb::snprintf(buf, 128, "_%p", this);
            m_texture_name += buf;
        }
        
        CachedWidget::~CachedWidget() {
            delete m_replaced_layer;
        }
        
        void CachedWidget::initialiseOverride() {
            MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate( this, &CachedWidget::frameEntered );
            if (m_texture) {
                setRenderItemTexture(m_texture);
            }
        }
        
        void CachedWidget::shutdownOverride() {
            MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate( this, &CachedWidget::frameEntered );
            if (m_texture) {
                MyGUI::RenderManager::getInstance().destroyTexture(m_texture);
                m_texture = 0;
            }
        }
        
        
        MyGUI::ILayerItem* CachedWidget::getLayerItemByPoint(int _left, int _top) const {
            return Base::getLayerItemByPoint(_left, _top);
        }
        
        const MyGUI::IntCoord& CachedWidget::getLayerItemCoord() const {
            return Base::getLayerItemCoord();
        }
        
        
        void CachedWidget::attachItemToNode(MyGUI::ILayer* _layer, MyGUI::ILayerNode* _node) {
            Base::attachItemToNode(_layer, _node);
        }
        
        void CachedWidget::onWidgetCreated(MyGUI::Widget* _widget) {
            Base::onWidgetCreated(_widget);
            _widget->setInheritsAlpha(false);
        }
        
        void CachedWidget::addChildItem(LayerItem* _item) {
            _item->attachItemToNode(0, m_replaced_layer);
        }
        
        void CachedWidget::removeChildItem(LayerItem* _item) {
            //_item->MyGUI::ILayerItem::detachFromLayer()
        }
        
        void CachedWidget::frameEntered(float dt) {
            MyGUI::IntSize size = getSize();
            size.width = next_pot(size.width);
            size.height = next_pot(size.height);
            bool need_recreate = !m_texture;
            if (!need_recreate) {
                if (size.width > m_texture->getWidth())
                    need_recreate = true;
                if (size.height > m_texture->getHeight())
                    need_recreate = true;
            }
            if (need_recreate) {
                if (m_texture) {
                    MyGUI::RenderManager::getInstance().destroyTexture(m_texture);
                    m_texture = 0;
                }
                
                m_texture = MyGUI::RenderManager::getInstance().createTexture(m_texture_name);
                m_texture->createManual(size.width, size.height, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
                setRenderItemTexture(m_texture);
                
//                MyGUI::ISubWidgetRect* main_rect = getSubWidgetMain();
//                if (main_rect) {
//                    main_rect->_setUVSet(MyGUI::FloatRect(0,0,
//                                                          float(getSize().width)/size.width,
//                                                          float(getSize().height)/size.height));
//                }
            } else {
                if (!m_replaced_layer->isOutOfDate())
                    return;
            }

            if (m_texture) {
                MyGUI::IRenderTarget* rt = m_texture->getRenderTarget();
                if (rt) {
                    rt->begin();
                    rt->getInfo().setOffset(getAbsoluteLeft(), getAbsoluteTop());
                    doRenderToTarget(rt);
                    rt->end();
                }
            }
        }
        
        void CachedWidget::doRenderToTarget(MyGUI::IRenderTarget* rt) {
            m_replaced_layer->renderToTarget(rt, true);
        }

    }
    
}
