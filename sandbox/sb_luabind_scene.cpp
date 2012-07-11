//
//  sb_luabind_scene.cpp
//  YinYang
//
//  Created by Андрей Куницын on 7/9/12.
//  Copyright (c) 2012 AndryBlack. All rights reserved.
//

#include "luabind/sb_luabind.h"
#include "meta/sb_meta.h"
#include "sb_scene_object.h"
#include "sb_sprite.h"
#include "sb_clear_scene.h"
#include "sb_fill_rect.h"
#include "sb_background.h"
#include "sb_label.h"
#include "sb_widget.h"
#include "sb_container.h"
#include "sb_container_blend.h"
#include "sb_container_transform.h"
#include "sb_container_color.h"
#include "sb_container_visible.h"
#include "sb_scene.h"

SB_META_DECLARE_KLASS(Sandbox::SceneObject, void)
SB_META_BEGIN_KLASS_BIND(Sandbox::SceneObject)
SB_META_PROPERTY_RW(Visible,GetVisible,SetVisible)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::Sprite, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::Sprite)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Image,GetImage,SetImage)
SB_META_PROPERTY_RW(Pos,GetPos,SetPos)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::ColorizedSprite, Sandbox::Sprite)
SB_META_BEGIN_KLASS_BIND(Sandbox::ColorizedSprite)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Color,GetColor,SetColor)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::ClearScene, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::ClearScene)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Color,GetColor,SetColor)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::FillRect, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::FillRect)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Texture,GetTexture,SetTexture)
SB_META_PROPERTY_RW(Rect,GetRect,SetRect)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::Background, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::Background)
SB_META_CONSTRUCTOR(())
SB_META_METHOD(Load)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::Label, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::Label)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Font,GetFont,SetFont)
SB_META_PROPERTY_RW(Pos,GetPos,SetPos)
SB_META_PROPERTY_RW(Text,GetText,SetText)
SB_META_PROPERTY_RW(Align,GetAlign,SetAlign)
SB_META_END_KLASS_BIND()


SB_META_DECLARE_KLASS(Sandbox::Container, Sandbox::SceneObject)
SB_META_BEGIN_KLASS_BIND(Sandbox::Container)
SB_META_CONSTRUCTOR(())
SB_META_METHOD(Reserve)
SB_META_METHOD(AddObject)
SB_META_METHOD(RemoveObject)
SB_META_METHOD(Clear)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::BlendMode,void);
SB_META_BEGIN_ENUM_BIND(Sandbox::BlendMode)
SB_META_ENUM_ITEM(BLEND_MODE_COPY)
SB_META_ENUM_ITEM(BLEND_MODE_ALPHABLEND)
SB_META_ENUM_ITEM(BLEND_MODE_ADDITIVE)
SB_META_ENUM_ITEM(BLEND_MODE_ADDITIVE_ALPHA)
SB_META_ENUM_ITEM(BLEND_MODE_SCREEN)
SB_META_END_ENUM_BIND()


SB_META_DECLARE_KLASS(Sandbox::ContainerBlend, Sandbox::Container)
SB_META_BEGIN_KLASS_BIND(Sandbox::ContainerBlend)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Mode,GetMode,SetMode)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::ContainerVisible, Sandbox::Container)
SB_META_BEGIN_KLASS_BIND(Sandbox::ContainerVisible)
SB_META_PROPERTY_RW_DEF(Invert)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::Widget, Sandbox::Container)
SB_META_BEGIN_KLASS_BIND(Sandbox::Widget)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Rect,GetRect,SetRect)
SB_META_PROPERTY_RW(Active,GetActive,SetActive)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::TouchButtonWidget, Sandbox::Widget)
SB_META_BEGIN_KLASS_BIND(Sandbox::TouchButtonWidget)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(ActivateEvent,GetActivateEvent,SetActivateEvent)
SB_META_PROPERTY_RW(DeactivateEvent,GetDeactivateEvent,SetDeactivateEvent)
SB_META_PROPERTY_RW(TouchUpInsideEvent,GetTouchUpInsideEvent,SetTouchUpInsideEvent)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::ContainerTransform, Sandbox::Container)
SB_META_BEGIN_KLASS_BIND(Sandbox::ContainerTransform)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Translate,GetTranslate,SetTranslate)
SB_META_PROPERTY_RW(Scale,GetScale,SetScale)
SB_META_PROPERTY_RW(ScaleX,GetScaleX,SetScaleX)
SB_META_PROPERTY_RW(ScaleY,GetScaleY,SetScaleY)
SB_META_PROPERTY_RW(Angle,GetAngle,SetAngle)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::ContainerColor, Sandbox::Container)
SB_META_BEGIN_KLASS_BIND(Sandbox::ContainerColor)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RW(Color,GetColor,SetColor)
SB_META_PROPERTY_RW(Alpha,GetAlpha,SetAlpha)
SB_META_END_KLASS_BIND()



SB_META_DECLARE_KLASS(Sandbox::Scene, void)
SB_META_BEGIN_KLASS_BIND(Sandbox::Scene)
SB_META_PROPERTY_RW(Root,GetRoot,SetRoot)
SB_META_END_KLASS_BIND()

namespace Sandbox {
    
    void register_scene( lua_State* lua ) {
        luabind::Class<SceneObject>(lua);
        luabind::Class<Sprite>(lua);
        luabind::Class<ColorizedSprite>(lua);
        luabind::Class<ClearScene>(lua);
        luabind::Class<FillRect>(lua);
        luabind::Class<Background>(lua);
        luabind::Class<Label>(lua);
        luabind::Class<Container>(lua);
        luabind::Class<Widget>(lua);
        luabind::Class<TouchButtonWidget>(lua);
        luabind::Enum<BlendMode>(lua);
        luabind::Class<ContainerBlend>(lua);
        luabind::Class<ContainerTransform>(lua);
        luabind::Class<ContainerColor>(lua);
        luabind::Class<ContainerVisible>(lua);
        luabind::ExternClass<Scene>(lua);
    }

}