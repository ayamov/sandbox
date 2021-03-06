//
//  sb_mygui_bind.cpp
//  sr-osx
//
//  Created by Andrey Kunitsyn on 24/11/13.
//  Copyright (c) 2013 Andrey Kunitsyn. All rights reserved.
//

#include "sb_mygui_bind.h"
#include "sb_lua_context.h"
#include "luabind/sb_luabind.h"
#include "luabind/sb_luabind_function.h"
#include "../sb_lua.h"

#include "MyGUI_LayoutManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_Align.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_ControllerManager.h"

#include "MyGUI_TextBox.h"
#include "MyGUI_Window.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ScrollBar.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_Window.h"

#include "MyGUI_IResource.h"

#include "MyGUI_TileRect.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_ImageBox.h"

#include "MyGUI_MultiListBox.h"
#include "MyGUI_MultiListItem.h"


#include "MyGUI_ComboBox.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_ScrollView.h"

#include "MyGUI_ProgressBar.h"
#include "MyGUI_SharedLayer.h"
#include "MyGUI_OverlappedLayer.h"
#include "MyGUI_ResourceLayout.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_PolygonalSkin.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_ControllerPosition.h"
#include "MyGUI_ControllerEdgeHide.h"
#include "MyGUI_ControllerRepeatClick.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_LanguageManager.h"

#include "sb_image.h"

#include "MyGUI_CommonStateInfo.h"

#include "widgets/sb_mygui_scene_widget.h"
#include "widgets/sb_mygui_mask_image.h"
//#include "font/sb_mygui_ft_font.h"
//#include "font/sb_mygui_ft_font_ol.h"
//#include "font/sb_mygui_multipass_font.h"
//#include "font/sb_mygui_combine_font.h"

#include "widgets/sb_mygui_scroll_area.h"

#include "sb_mygui_render.h"
#include "sb_mygui_widget_render.h"
#include "sb_mygui_animated_layer.h"
#include "sb_mygui_bind_helpers.h"
#include "sb_mygui_gui.h"

#include "sb_utf.h"

namespace Sandbox {
    
    namespace luabind {
        
        template <>
        struct stack<MyGUI::UString> {
            static void push( lua_State* L, const MyGUI::UString& val ) {
                stack<const char*>::push(L, val.asUTF8_c_str());
            }
            static MyGUI::UString get( lua_State* L, int idx ) {
                return MyGUI::UString(stack<const char*>::get(L, idx));
            }
        };
        template <>
        struct stack<const MyGUI::UString&> : stack<MyGUI::UString> {};
        
        
        template <class T>
        struct stack<MyGUI::types::TPoint<T> > {
            static void push( lua_State* L, const MyGUI::types::TPoint<T>& val ) {
                stack_help<MyGUI::types::TPoint<T>, false>::push(L, val);
            }
            static MyGUI::types::TPoint<T> get( lua_State* L, int idx ) {
                if (lua_istable(L, idx)) {
                    MyGUI::types::TPoint<T> res;
                    lua_rawgeti(L, idx, 1);
                    res.left = lua_tonumber(L, -1);
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 2);
                    res.top = lua_tonumber(L, -1);
                    lua_pop(L, 1);
                    return  res;
                }
                return stack_help<MyGUI::types::TPoint<T>, false>::get(L, idx);
            }
        };
        template <class T>
        struct stack<const MyGUI::types::TPoint<T>&> : stack<MyGUI::types::TPoint<T> > {};
        
        template <>
        struct stack<MyGUI::IntSize> {
            static void push( lua_State* L, const MyGUI::IntSize& val ) {
                stack_help<MyGUI::IntSize, false>::push(L, val);
            }
            static MyGUI::IntSize get( lua_State* L, int idx ) {
                if (lua_istable(L, idx)) {
                    MyGUI::IntSize res;
                    lua_rawgeti(L, idx, 1);
                    res.width = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 2);
                    res.height = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    return  res;
                }
                return stack_help<MyGUI::IntSize, false>::get(L, idx);
            }
        };
        template <>
        struct stack<const MyGUI::IntSize&> : stack<MyGUI::IntSize> {};
        
        template <>
        struct stack<MyGUI::IntRect> {
            static void push( lua_State* L, const MyGUI::IntRect& val ) {
                stack_help<MyGUI::IntRect, false>::push(L, val);
            }
            static MyGUI::IntRect get( lua_State* L, int idx ) {
                if (lua_istable(L, idx)) {
                    MyGUI::IntRect res;
                    lua_rawgeti(L, idx, 1);
                    res.left = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 2);
                    res.top = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 3);
                    res.right = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 4);
                    res.bottom = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    return  res;
                }
                return stack_help<MyGUI::IntRect, false>::get(L, idx);
            }
        };
        template <>
        struct stack<const MyGUI::IntRect&> : stack<MyGUI::IntRect> {};
        
        template <>
        struct stack<MyGUI::IntCoord> {
            static void push( lua_State* L, const MyGUI::IntCoord& val ) {
                stack_help<MyGUI::IntCoord, false>::push(L, val);
            }
            static MyGUI::IntCoord get( lua_State* L, int idx ) {
                if (lua_istable(L, idx)) {
                    MyGUI::IntCoord res;
                    lua_rawgeti(L, idx, 1);
                    res.left = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 2);
                    res.top = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 3);
                    res.width = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    lua_rawgeti(L, idx, 4);
                    res.height = float(lua_tonumber(L, -1));
                    lua_pop(L, 1);
                    return  res;
                }
                return stack_help<MyGUI::IntCoord, false>::get(L, idx);
            }
        };
        template <>
        struct stack<const MyGUI::IntCoord&> : stack<MyGUI::IntCoord> {};
                
        template <>
        struct stack<MyGUI::Any> {
            static void push( lua_State* L, const MyGUI::Any& val ) {
                if (val.getType()==typeid(int))
                    stack<int>::push(L, *val.castType<int>());
                else if (val.getType()==typeid(float))
                    stack<float>::push(L, *val.castType<float>());
                else if (val.getType()==typeid(double))
                    stack<float>::push(L, *val.castType<double>());
                else if (val.getType()==typeid(std::string))
                    stack<std::string>::push(L, *val.castType<std::string>());
                else if (val.getType()==typeid(bool))
                    stack<bool>::push(L, *val.castType<bool>());
                else
                    lua_pushnil(L);
            }
            static MyGUI::Any get( lua_State* L, int idx ) {
                int type = lua_type(L, idx);
                switch (type){
                    case LUA_TBOOLEAN:
                        return MyGUI::Any(stack<bool>::get(L, idx));
                    case LUA_TNUMBER:
                        return MyGUI::Any(stack<double>::get(L, idx));
                    case LUA_TSTRING:
                        return MyGUI::Any(stack<std::string>::get(L, idx));
                }
                return MyGUI::Any(MyGUI::Any::Null);
            }
        };
        
        template <>
        struct stack<MyGUI::Colour> {
            static void push( lua_State* L, const MyGUI::Colour& val ) {
                stack_help<MyGUI::Colour, false>::push(L, val);
            }
            static MyGUI::Colour get( lua_State* L, int idx ) {
                int type = lua_type(L, idx);
                if (type == LUA_TSTRING)
                    return MyGUI::Colour::parse(lua_tostring(L, idx));
                return stack_help<MyGUI::Colour, false>::get(L, idx);
            }
        };
        template <>
        struct stack<const MyGUI::Colour&> : stack<MyGUI::Colour> {};
        
        template <>
        struct stack<MyGUI::Align> {
            static void push( lua_State* L, const MyGUI::Align& val ) {
                stack_help<MyGUI::Align, false>::push(L, val);
            }
            static MyGUI::Align get( lua_State* L, int idx ) {
                int type = lua_type(L, idx);
                if (type == LUA_TSTRING)
                    return MyGUI::Align::parse(lua_tostring(L, idx));
                return stack_help<MyGUI::Align, false>::get(L, idx);
            }
        };
        template <>
        struct stack<const MyGUI::Align&> : stack<MyGUI::Align> {};
        
        
        template <>
        struct stack<MyGUI::MouseButton> {
            static void push( lua_State* L, const MyGUI::MouseButton& val ) {
                stack<int>::push(L, val.getValue());
            }
            static MyGUI::MouseButton get( lua_State* L, int idx ) {
                return MyGUI::MouseButton(static_cast<MyGUI::MouseButton::Enum>(stack<int>::get(L, idx)));
            }
        };
        
        template <>
        struct stack<const MyGUI::MouseButton&> : stack<MyGUI::MouseButton> {};
        
        template <>
        struct stack<MyGUI::WidgetStyle> {
            static void push( lua_State* L, const MyGUI::WidgetStyle& val ) {
                stack<int>::push(L, val.getValue());
            }
            static MyGUI::WidgetStyle get( lua_State* L, int idx ) {
                return MyGUI::WidgetStyle(static_cast<MyGUI::WidgetStyle::Enum>(stack<int>::get(L, idx)));
            }
        };
        
        template <>
        struct stack<const MyGUI::WidgetStyle&> : stack<MyGUI::WidgetStyle> {};

    }
}



SB_META_DECLARE_KLASS(MyGUI::Align, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::Align)
SB_META_CONSTRUCTOR(())
SB_META_PROPERTY_RO(isHCenter, isHCenter)
SB_META_PROPERTY_RO(isVCenter, isVCenter)
SB_META_PROPERTY_RO(isCenter, isCenter)
SB_META_PROPERTY_RO(isHStretch, isHStretch)
SB_META_PROPERTY_RO(isLeft, isLeft)
SB_META_PROPERTY_RO(isRight, isRight)
SB_META_PROPERTY_RO(isTop, isTop)
SB_META_PROPERTY_RO(isBottom, isBottom)
SB_META_PROPERTY_RO(isVStretch, isVStretch)
SB_META_PROPERTY_RO(isStretch, isStretch)
SB_META_PROPERTY_RO(isDefault, isDefault)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::IntPoint, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IntPoint)
SB_META_CONSTRUCTOR((int,int))
SB_META_PROPERTY(left)
SB_META_PROPERTY(top)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::FloatPoint, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::FloatPoint)
SB_META_CONSTRUCTOR((float,float))
SB_META_PROPERTY(left)
SB_META_PROPERTY(top)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::IntSize, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IntSize)
SB_META_CONSTRUCTOR((int,int))
SB_META_PROPERTY(width)
SB_META_PROPERTY(height)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::IntCoord, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IntCoord)
SB_META_CONSTRUCTOR((int,int,int,int))
SB_META_PROPERTY(left)
SB_META_PROPERTY(top)
SB_META_PROPERTY(width)
SB_META_PROPERTY(height)
SB_META_PROPERTY_RO(right, right)
SB_META_PROPERTY_RO(bottom, bottom)
SB_META_PROPERTY_RO(point, point)
SB_META_PROPERTY_RO(size, size)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::IntRect, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IntRect)
SB_META_CONSTRUCTOR((int,int,int,int))
SB_META_PROPERTY(left)
SB_META_PROPERTY(top)
SB_META_PROPERTY(right)
SB_META_PROPERTY(bottom)
SB_META_PROPERTY_RO(width, width)
SB_META_PROPERTY_RO(height, height)
SB_META_STATIC_METHOD(parse)
SB_META_END_KLASS_BIND()

static int create_colour(lua_State* L) {
    if (lua_isstring(L, 2)) {
        Sandbox::luabind::stack<MyGUI::Colour>::push(L, MyGUI::Colour::parse(lua_tostring(L, 2)));
    } else {
        if (lua_gettop(L)>4) {
            Sandbox::luabind::stack<MyGUI::Colour>::push(L, MyGUI::Colour(lua_tonumber(L, 2),
                                                                          lua_tonumber(L, 3),
                                                                          lua_tonumber(L, 4),
                                                                          lua_tonumber(L, 5)));
        } else {
            Sandbox::luabind::stack<MyGUI::Colour>::push(L, MyGUI::Colour(lua_tonumber(L, 2),
                                                                          lua_tonumber(L, 3),
                                                                          lua_tonumber(L, 4)));
        }
    }
    return 1;
}

SB_META_DECLARE_KLASS(MyGUI::Colour, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::Colour)
bind( constructor(&create_colour) );
SB_META_PROPERTY(red)
SB_META_PROPERTY(green)
SB_META_PROPERTY(blue)
SB_META_PROPERTY(alpha)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::IBDrawItemInfo, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IBDrawItemInfo)
SB_META_PROPERTY(index)
SB_META_PROPERTY(update)
SB_META_PROPERTY(select)
SB_META_PROPERTY(active)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::ICroppedRectangle, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::ICroppedRectangle)

SB_META_PROPERTY_RW(position, getPosition, setPosition)
SB_META_PROPERTY_RW(size, getSize, setSize)
SB_META_PROPERTY_RW(coord, getCoord, setCoord)
SB_META_PROPERTY_RO(left, getLeft)
SB_META_PROPERTY_RO(right, getRight)
SB_META_PROPERTY_RO(top, getTop)
SB_META_PROPERTY_RO(bottom, getBottom)
SB_META_PROPERTY_RO(width, getWidth)
SB_META_PROPERTY_RO(height, getHeight)
SB_META_PROPERTY_RO(absolutePosition, getAbsolutePosition)
SB_META_PROPERTY_RO(absoluteRect, getAbsoluteRect)
SB_META_PROPERTY_RO(absoluteLeft, getAbsoluteLeft)
SB_META_PROPERTY_RO(absoluteTop, getAbsoluteTop)

SB_META_END_KLASS_BIND()


SB_META_DECLARE_KLASS(MyGUI::MouseButton,void);
SB_META_ENUM_BIND(MyGUI::MouseButton,namespace MyGUI,
                  SB_META_ENUM_MEMBER_ITEM(Left,MouseButton::Left)
                  SB_META_ENUM_MEMBER_ITEM(Right,MouseButton::Right)
                  SB_META_ENUM_MEMBER_ITEM(Middle,MouseButton::Middle))


SB_META_DECLARE_KLASS(MyGUI::IObject, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::IObject)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::WidgetStyle,void);
SB_META_ENUM_BIND(MyGUI::WidgetStyle,namespace MyGUI,
                  SB_META_ENUM_MEMBER_ITEM(Child,WidgetStyle::Child)
                  SB_META_ENUM_MEMBER_ITEM(Popup,WidgetStyle::Popup)
                  SB_META_ENUM_MEMBER_ITEM(Overlapped,WidgetStyle::Overlapped))

//SB_META_DECLARE_KLASS(MyGUI::WidgetStyle, void)
//SB_META_BEGIN_KLASS_BIND(MyGUI::WidgetStyle)
//SB_META_CONSTRUCTOR((MyGUI::WidgetStyle::Enum))
//SB_META_STATIC_METHOD(parse)
//SB_META_END_KLASS_BIND()

static bool widget_isUserString(const MyGUI::Widget* w,const char* key) {
    return w->isUserString(key);
}
static sb::string widget_getUserString(const MyGUI::Widget* w,const char* key) {
    return w->getUserString(key);
}
static void widget_setUserString( MyGUI::Widget* w,const char* key,const char* value) {
    w->setUserString(key,value);
}
static void widget_updateChilds( MyGUI::Widget* w) {
    if (w) {
        w->_updateChilds();
    }
}
static void widget_updateSkinChilds( MyGUI::Widget* w) {
    if (w) {
        w->_updateSkinChilds();
    }
}
static void widget_upLayerItem( MyGUI::Widget* w) {
    if (w) {
        w->upLayerItem();
    }
}
static void widget_destroyAllChilds( MyGUI::Widget* w) {
    if (w) {
        while (w->getChildCount() != 0)
            MyGUI::WidgetManager::getInstance().destroyWidget(w->getChildAt(0));
    }
}
static bool widget_isMyChild( MyGUI::Widget* w, MyGUI::Widget* ch) {
    if (w && ch) {
        do {
            if (w == ch)
                return true;
            ch = ch->getParent();
            if (!ch) return false;
        } while (true);
    }
    return false;
}

static sb::string widget_getLayerName( MyGUI::Widget* w) {
    if (w) {
        MyGUI::ILayer* l = w->getLayer();
        if (l) {
            return l->getName();
        }
    }
    return "";
}

static bool widget_getInheritsPick( const MyGUI::Widget* w ) {
    return w->getInheritsPick();
}
static void widget_setInheritsPick( MyGUI::Widget* w, bool i) {
    w->setInheritsPick(i);
}
static bool widget_isTypeOf( const MyGUI::Widget* w , const char* name ) {
    if (!w) return false;
    return w->isTypeNameOf(name);
}

SB_META_DECLARE_OBJECT(MyGUI::Widget, MyGUI::ICroppedRectangle)
SB_META_BEGIN_KLASS_BIND(MyGUI::Widget)
SB_META_PROPERTY_RO(name, getName)
SB_META_PROPERTY_RW(align, getAlign,setAlign)
SB_META_PROPERTY_RW(visible,getVisible,setVisible)
SB_META_PROPERTY_RW(alpha, getAlpha, setAlpha)
SB_META_PROPERTY_RW(inheritsAlpha, getInheritsAlpha, setInheritsAlpha)
SB_META_PROPERTY_RW(enabled, getEnabled, setEnabled)
SB_META_PROPERTY_RO(parent, getParent)
SB_META_PROPERTY_RO(clientWidget, getClientWidget)
SB_META_PROPERTY_WO(colour, setColour)
SB_META_PROPERTY_RW(alpha, getAlpha,setAlpha)
SB_META_PROPERTY_RW(depth, getDepth,setDepth)
SB_META_PROPERTY_RW(needMouse, getNeedMouseFocus, setNeedMouseFocus)
SB_META_PROPERTY_RW(needKey, getNeedKeyFocus, setNeedKeyFocus)
bind( property_rw( "inheritsPick", &widget_getInheritsPick, &widget_setInheritsPick ) );
SB_META_PROPERTY_RW(inheritsState, getInheritsState, setInheritsState)

bind(method("eventMouseButtonClick", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetVoid,
            &MyGUI::WidgetInput::eventMouseButtonClick>::lua_func));

bind(method("eventMouseButtonPressed", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetFloatFloatButton,
            &MyGUI::WidgetInput::eventMouseButtonPressed>::lua_func));

bind(method("eventMouseButtonReleased", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetFloatFloatButton,
            &MyGUI::WidgetInput::eventMouseButtonReleased>::lua_func));

bind(method("eventMouseDrag", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetFloatFloatButton,
            &MyGUI::WidgetInput::eventMouseDrag>::lua_func));

bind(method("eventKeySetFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetWidget,
            &MyGUI::WidgetInput::eventKeySetFocus>::lua_func));

bind(method("eventKeyLostFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetWidget,
            &MyGUI::WidgetInput::eventKeyLostFocus>::lua_func));

bind(method("eventRootKeyChangeFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetBool,
            &MyGUI::WidgetInput::eventRootKeyChangeFocus>::lua_func));

bind(method("eventMouseSetFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetWidget,
            &MyGUI::WidgetInput::eventMouseSetFocus>::lua_func));

bind(method("eventMouseLostFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetWidget,
            &MyGUI::WidgetInput::eventMouseLostFocus>::lua_func));

bind(method("eventRootMouseChangeFocus", delegate_bind<MyGUI::Widget,
            MyGUI::WidgetInput,
            MyGUI::EventHandle_WidgetBool,
            &MyGUI::WidgetInput::eventRootMouseChangeFocus>::lua_func));

SB_META_METHOD(detachFromWidget)
SB_META_METHOD(attachToWidget)

SB_META_METHOD(findWidget)
SB_META_METHOD(findSkinWidget)

SB_META_METHOD(getChildCount)
SB_META_METHOD(getChildAt)

SB_META_METHOD(setProperty)
SB_META_METHOD(changeWidgetSkin)

bind( method( "isTypeOf" , &widget_isTypeOf ) );
bind( method( "isUserString" , &widget_isUserString ) );
bind( method( "getUserString" , &widget_getUserString ) );
bind( method( "setUserString" , &widget_setUserString ) );
bind( method( "updateChilds" , &widget_updateChilds ) );
bind( method( "updateSkinChilds" , &widget_updateSkinChilds ) );
bind( method( "destroyAllChilds" , &widget_destroyAllChilds ) );
bind( method( "isMyChild" , &widget_isMyChild ) );
bind( method( "getLayerName", &widget_getLayerName));
bind( method( "upLayerItem", &widget_upLayerItem));
bind(method("createWidget", static_cast<MyGUI::Widget*(MyGUI::Widget::*)(const std::string&, const std::string&, const MyGUI::IntCoord&, MyGUI::Align, const std::string& _name)>(&MyGUI::Widget::createWidgetT)));
bind(method("createWidgetS", static_cast<MyGUI::Widget*(MyGUI::Widget::*)(MyGUI::WidgetStyle,
                                                                          const std::string&,
                                                                          const std::string&,
                                                                          const MyGUI::IntCoord&,
                                                                          MyGUI::Align,
                                                                          const std::string& _layer,
                                                                          const std::string& _name)>(&MyGUI::Widget::createWidgetT)));

SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ISubWidget, MyGUI::ICroppedRectangle)
SB_META_DECLARE_OBJECT(MyGUI::ISubWidgetRect, MyGUI::ISubWidget)
SB_META_DECLARE_OBJECT(MyGUI::ISubWidgetText, MyGUI::ISubWidget)


static void set_image_proxy(MyGUI::ImageBox* ib,const Sandbox::ImagePtr& img) {
    if (!img) {
        ib->setImageTexture("");
    } else {
        ib->setImageTexture(img->GetTexture()->GetName());
        ib->setImageRect(MyGUI::IntRect(img->GetTextureX(),
                                        img->GetTextureY(),
                                        img->GetTextureX()+img->GetTextureW(),
                                        img->GetTextureY()+img->GetTextureH()));
    }
}

SB_META_DECLARE_OBJECT(MyGUI::ImageBox, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::ImageBox)
SB_META_METHOD(setImageInfo)
SB_META_METHOD(setImageRect)
SB_META_METHOD(setImageTexture)
bind(method("setImage", &set_image_proxy));
SB_META_END_KLASS_BIND()

//
//static void TextBox_setCaptionWithReplacing( MyGUI::TextBox* tb, const MyGUI::UString& s) {
//    tb->setCaptionWithReplacing(s);
//}
//static const MyGUI::UString& TextBox_getCaption( const MyGUI::TextBox* tb) {
//    return tb->getCaption();
//}

SB_META_DECLARE_OBJECT(MyGUI::TextBox, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::TextBox)
SB_META_PROPERTY_RO(textSize, getTextSize)
bind(property_rw("caption", &MyGUI::TextBox::getCaption, &MyGUI::TextBox::setCaption));
bind(property_wo("captionWithTags",&MyGUI::TextBox::setCaptionWithReplacing));
SB_META_PROPERTY_RW(fontName, getFontName, setFontName)
SB_META_PROPERTY_RW(fontHeight, getFontHeight, setFontHeight)
SB_META_PROPERTY_RW(textAlign, getTextAlign, setTextAlign)
SB_META_PROPERTY_RW(textColour, getTextColour, setTextColour)
SB_META_END_KLASS_BIND()


SB_META_DECLARE_OBJECT(MyGUI::Button, MyGUI::TextBox)
SB_META_BEGIN_KLASS_BIND(MyGUI::Button)
SB_META_PROPERTY_RW(stateSelected, getStateSelected, setStateSelected)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::MenuItem, MyGUI::Button)

SB_META_DECLARE_OBJECT(MyGUI::Window, MyGUI::TextBox)
SB_META_BEGIN_KLASS_BIND(MyGUI::Window)
SB_META_PROPERTY_RW(movable, getMovable, setMovable)
SB_META_PROPERTY_RW(autoAlpha, getAutoAlpha, setAutoAlpha)
SB_META_METHOD(setVisibleSmooth)
SB_META_METHOD(destroySmooth)
bind(method("eventWindowButtonPressed", delegate_bind<MyGUI::Window,
            MyGUI::Window,
            MyGUI::EventHandle_WindowPtrCStringRef,
            &MyGUI::Window::eventWindowButtonPressed>::lua_func));
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ScrollBar, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::ScrollBar)
SB_META_PROPERTY_RW(scrollPosition, getScrollPosition, setScrollPosition)
SB_META_PROPERTY_RW(scrollRange, getScrollRange, setScrollRange)
bind(method("eventScrollChangePosition", delegate_bind<MyGUI::ScrollBar,
            MyGUI::ScrollBar,
            MyGUI::EventHandle_ScrollBarPtrSizeT,
            &MyGUI::ScrollBar::eventScrollChangePosition>::lua_func));
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::MenuControl, MyGUI::Widget)

SB_META_DECLARE_OBJECT(MyGUI::PopupMenu, MyGUI::MenuControl)

SB_META_DECLARE_OBJECT(MyGUI::EditBox, MyGUI::TextBox)
SB_META_BEGIN_KLASS_BIND(MyGUI::EditBox)
SB_META_PROPERTY_RO(textLength, getTextLength)
SB_META_PROPERTY_RW(maxTextLength, getMaxTextLength, setMaxTextLength)
SB_META_PROPERTY_RW(overflowToTheLeft, getOverflowToTheLeft, setOverflowToTheLeft)
bind(method("eventEditSelectAccept", delegate_bind<MyGUI::EditBox,
            MyGUI::EditBox,
            MyGUI::EventHandle_EditPtr,
            &MyGUI::EditBox::eventEditSelectAccept>::lua_func));
bind(method("eventEditTextChange", delegate_bind<MyGUI::EditBox,
            MyGUI::EditBox,
            MyGUI::EventHandle_EditPtr,
            &MyGUI::EditBox::eventEditTextChange>::lua_func));
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ComboBox, MyGUI::EditBox)

SB_META_DECLARE_OBJECT(MyGUI::TabItem, MyGUI::TextBox)

SB_META_DECLARE_OBJECT(MyGUI::MenuBar, MyGUI::MenuControl)

SB_META_DECLARE_OBJECT(MyGUI::ListBox, MyGUI::Widget)

SB_META_DECLARE_OBJECT(MyGUI::DDContainer, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::DDContainer)
SB_META_PROPERTY_RW(needDragDrop, getNeedDragDrop, setNeedDragDrop)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ItemBox, MyGUI::DDContainer)
SB_META_BEGIN_KLASS_BIND(MyGUI::ItemBox)
SB_META_PROPERTY_RW(indexSelected, getIndexSelected, setIndexSelected)
bind(method("requestCreateWidgetItem", delegate_bind<MyGUI::ItemBox,
            MyGUI::ItemBox,
            MyGUI::EventHandle_ItemBoxPtrWidgetPtr,
            &MyGUI::ItemBox::requestCreateWidgetItem>::lua_func));
bind(method("requestCoordItem", delegate_bind<MyGUI::ItemBox,
            MyGUI::ItemBox,
            MyGUI::EventHandle_ItemBoxPtrIntCoordRefBool,
            &MyGUI::ItemBox::requestCoordItem>::lua_func));

SB_META_METHOD(addItem)
SB_META_METHOD(getIndexByWidget)
SB_META_METHOD(getWidgetByIndex)
SB_META_METHOD(redrawItemAt)
SB_META_METHOD(redrawAllItems)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ScrollView, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::ScrollView)
SB_META_PROPERTY_RW(canvasSize,getCanvasSize,setCanvasSize)
SB_META_PROPERTY_RO(contentSize, getContentSize)
SB_META_PROPERTY_RO(viewSize, getViewSize)
SB_META_PROPERTY_RW(visibleVScroll, isVisibleVScroll, setVisibleVScroll)
SB_META_PROPERTY_RW(visibleHScroll, isVisibleHScroll, setVisibleHScroll)
SB_META_END_KLASS_BIND()


SB_META_DECLARE_OBJECT(MyGUI::TabControl, MyGUI::Widget)

SB_META_DECLARE_OBJECT(MyGUI::ProgressBar, MyGUI::Widget)
SB_META_BEGIN_KLASS_BIND(MyGUI::ProgressBar)
SB_META_PROPERTY_RW(progressPosition, getProgressPosition, setProgressPosition)
SB_META_PROPERTY_RW(progressRange, getProgressRange, setProgressRange)
SB_META_PROPERTY_WO(trackColour, setTrackColour)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::MultiListBox, MyGUI::Widget)

SB_META_DECLARE_OBJECT(MyGUI::MultiListItem, MyGUI::TextBox)


SB_META_DECLARE_OBJECT(MyGUI::ISerializable, MyGUI::IObject)
SB_META_BEGIN_KLASS_BIND(MyGUI::ISerializable)
SB_META_END_KLASS_BIND()
//static int mygui_ifont_get_string_width( MyGUI::IFont* font, const char* str) {
//    if (!font || !str) return 0;
//    int w = 0;
//    while(*str) {
//        Sandbox::UTF32Char ch = 0;
//        str = Sandbox::get_char(str,ch);
//        MyGUI::GlyphInfo* glyph = font->getGlyphInfo(-1,ch);
//        if (glyph)
//            w += glyph->bearingX + glyph->advance;
//    }
//    return w;
//}

SB_META_DECLARE_OBJECT(MyGUI::IResource, MyGUI::ISerializable)
//SB_META_DECLARE_OBJECT(MyGUI::IFont, MyGUI::IResource)
//SB_META_BEGIN_KLASS_BIND(MyGUI::IFont)
//SB_META_METHOD(getDefaultHeight)
//bind( method( "getStringWidth" , &mygui_ifont_get_string_width ) );
//SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::IStateInfo, MyGUI::ISerializable)

SB_META_DECLARE_OBJECT(MyGUI::ILayer, MyGUI::ISerializable)
SB_META_BEGIN_KLASS_BIND(MyGUI::ILayer)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ILayerNode, MyGUI::IObject)

SB_META_DECLARE_OBJECT(MyGUI::LayerNode, MyGUI::ILayerNode)

SB_META_DECLARE_OBJECT(MyGUI::SharedLayer, MyGUI::ILayer)
SB_META_DECLARE_OBJECT(MyGUI::OverlappedLayer, MyGUI::ILayer)
SB_META_BEGIN_KLASS_BIND(MyGUI::OverlappedLayer)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::SharedLayerNode, MyGUI::ILayerNode)

SB_META_DECLARE_OBJECT(MyGUI::TileRect, MyGUI::ISubWidgetRect)


SB_META_DECLARE_OBJECT(MyGUI::SubSkin, MyGUI::ISubWidgetRect)
SB_META_DECLARE_OBJECT(MyGUI::MainSkin, MyGUI::SubSkin)



//static int ResourceTrueTypeFont_setCharImage(lua_State* L) {
//    Sandbox::mygui::ResourceTrueTypeFont* self = Sandbox::luabind::stack<Sandbox::mygui::ResourceTrueTypeFont*>::get(L, 1);
//    MyGUI::Char char_code = Sandbox::luabind::stack<MyGUI::Char>::get(L, 2);
//    if (lua_isstring(L, 3)) {
//        self->setCharImage(char_code, Sandbox::luabind::stack<sb::string>::get(L, 3),
//                           Sandbox::luabind::stack<MyGUI::IntCoord>::get(L, 4),
//                           Sandbox::luabind::stack<MyGUI::IntPoint>::get(L, 5),
//                           Sandbox::luabind::stack<float>::get(L, 6));
//    } else {
//        self->setCharImage(char_code, Sandbox::luabind::stack<Sandbox::ImagePtr>::get(L, 3),
//                           Sandbox::luabind::stack<float>::get(L, 4));
//    }
//    return 0;
//}
//
//SB_META_DECLARE_OBJECT(Sandbox::mygui::ResourceTrueTypeFont, MyGUI::IFont)
//SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::ResourceTrueTypeFont)
//bind(method("setCharImage", &ResourceTrueTypeFont_setCharImage));
//SB_META_END_KLASS_BIND()
//
//SB_META_DECLARE_OBJECT(Sandbox::mygui::ResourceTrueTypeFontOutline, Sandbox::mygui::ResourceTrueTypeFont)
//SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::ResourceTrueTypeFontOutline)
//SB_META_END_KLASS_BIND()

//SB_META_DECLARE_OBJECT(Sandbox::mygui::ResourceMultipassFont, MyGUI::IFont)
//SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::ResourceMultipassFont)
//SB_META_END_KLASS_BIND()
//
//SB_META_DECLARE_OBJECT(Sandbox::mygui::ResourceCombineFont, MyGUI::IFont)
//SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::ResourceCombineFont)
//SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ResourceLayout, MyGUI::IResource)
SB_META_DECLARE_OBJECT(MyGUI::ResourceSkin, MyGUI::IResource)


SB_META_DECLARE_OBJECT(MyGUI::RotatingSkin, MyGUI::ISubWidgetRect)
SB_META_DECLARE_OBJECT(MyGUI::PolygonalSkin, MyGUI::ISubWidgetRect)

SB_META_DECLARE_OBJECT(MyGUI::ControllerItem, MyGUI::IObject)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerItem)
bind(method("eventPreAction", delegate_bind<MyGUI::ControllerItem,
            MyGUI::ControllerItem,
            MyGUI::EventHandle_WidgetPtrControllerItemPtr,
            &MyGUI::ControllerItem::eventPreAction>::lua_func));
bind(method("eventPostAction", delegate_bind<MyGUI::ControllerItem,
            MyGUI::ControllerItem,
            MyGUI::EventHandle_WidgetPtrControllerItemPtr,
            &MyGUI::ControllerItem::eventPostAction>::lua_func));
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ControllerPosition, MyGUI::ControllerItem)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerPosition)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ControllerFadeAlpha, MyGUI::ControllerItem)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerFadeAlpha)
SB_META_PROPERTY_WO(alpha, setAlpha)
SB_META_PROPERTY_WO(coef, setCoef)
SB_META_PROPERTY_WO(enabled, setEnabled)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ControllerEdgeHide, MyGUI::ControllerItem)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerEdgeHide)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::ControllerRepeatClick, MyGUI::ControllerItem)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerRepeatClick)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_OBJECT(MyGUI::SubSkinStateInfo, MyGUI::IStateInfo)
SB_META_DECLARE_OBJECT(MyGUI::RotatingSkinStateInfo, MyGUI::IStateInfo)
SB_META_DECLARE_OBJECT(MyGUI::TileRectStateInfo, MyGUI::IStateInfo)

SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::WidgetRender)
SB_META_CONSTRUCTOR((MyGUI::IntSize))
SB_META_PROPERTY_RO(image, getImage)
SB_META_METHOD(render)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::mygui::RenderManager, void)
SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::RenderManager)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_METHOD(renderLayout)
SB_META_PROPERTY_WO(context, setContext)
SB_META_END_KLASS_BIND()



SB_META_DECLARE_KLASS(MyGUI::LayoutManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::LayoutManager)
SB_META_METHOD(loadLayout)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::LayerManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::LayerManager)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_METHOD(attachToLayerNode)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::ResourceManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::ResourceManager)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_METHOD(load)
SB_META_METHOD(findByName)
SB_META_METHOD(addResource)
SB_META_METHOD(removeByName)
SB_META_METHOD(removeResource)
SB_META_PROPERTY_RO(categoryName, getCategoryName)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::LanguageManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::LanguageManager)
SB_META_STATIC_METHOD(getInstancePtr)
bind( static_method( "getOnlyText" , &MyGUI::TextIterator::getOnlyText ) );
SB_META_METHOD(replaceTags)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::FactoryManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::FactoryManager)
SB_META_STATIC_METHOD(getInstancePtr)
bind(method("createObject", static_cast<MyGUI::IObject*(MyGUI::FactoryManager::*)(const std::string& _category, const std::string& _type)>(&MyGUI::FactoryManager::createObject)));
SB_META_END_KLASS_BIND()


SB_META_DECLARE_KLASS(MyGUI::ControllerManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::ControllerManager)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_METHOD(createItem)
SB_META_METHOD(addItem)
SB_META_METHOD(removeItem)
SB_META_END_KLASS_BIND()

SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::AnimatedLayer)
SB_META_END_KLASS_BIND()

static int gui_find_widget_proxy(lua_State* L) {
    MyGUI::Gui* self = Sandbox::luabind::stack<MyGUI::Gui*>::get(L, 1);
    MyGUI::Widget* w = 0;
    if (lua_gettop(L)>2) {
        w = self->findWidgetT(Sandbox::luabind::stack<sb::string>::get(L,2),
                              Sandbox::luabind::stack<sb::string>::get(L,3), false);
    } else {
        w = self->findWidgetT(Sandbox::luabind::stack<sb::string>::get(L,2),false);
    }
    Sandbox::luabind::stack<MyGUI::Widget*>::push(L, w);
    return 1;
}


SB_META_DECLARE_KLASS(MyGUI::Gui, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::Gui)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_METHOD(destroyWidget)
SB_META_METHOD(destroyAllChildWidget)
bind(method("createWidget", static_cast<MyGUI::Widget*(MyGUI::Gui::*)(const std::string&, const std::string&, const MyGUI::IntCoord&, MyGUI::Align, const std::string& _layer, const std::string& _name)>(&MyGUI::Gui::createWidgetT)));
bind( method( "findWidget" , &gui_find_widget_proxy ) );
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(Sandbox::mygui::GUI, MyGUI::Gui)
SB_META_BEGIN_KLASS_BIND(Sandbox::mygui::GUI)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_END_KLASS_BIND()

SB_META_DECLARE_KLASS(MyGUI::InputManager, void)
SB_META_BEGIN_KLASS_BIND(MyGUI::InputManager)
SB_META_STATIC_METHOD(getInstancePtr)
SB_META_PROPERTY_RO(keyFocusWidget, getKeyFocusWidget)
SB_META_PROPERTY_RO(mouseFocusWidget, getMouseFocusWidget)
SB_META_METHOD(getTopModalWidget)
SB_META_METHOD(addWidgetModal)
SB_META_METHOD(removeWidgetModal)
SB_META_METHOD(setKeyFocusWidget)
SB_META_METHOD(getMousePosition)
bind(method("resetKeyFocusWidget",
            static_cast<void(MyGUI::InputManager::*)()>(&MyGUI::InputManager::resetKeyFocusWidget)));
SB_META_END_KLASS_BIND()






namespace Sandbox {
    namespace mygui {
        
        void register_skin();
        void register_widgets();
        void unregister_skin();
        void unregister_widgets();
        
        
        void register_widgets(lua_State* L);
        
        void register_mygui( lua_State* lua ) {
            {
                luabind::Namespace(lua,"MyGUI")
                    ("ITEM_NONE",MyGUI::ITEM_NONE);
            }
            
            luabind::ExternClass<MyGUI::IObject>(lua);
            luabind::ExternClass<MyGUI::ISerializable>(lua);
            
            luabind::RawClass<MyGUI::WidgetStyle>(lua);
            //luabind::Enum<MyGUI::WidgetStyle::Enum>(lua);
            
            luabind::RawClass<MyGUI::MouseButton>(lua);
            
            luabind::RawClass<MyGUI::Align>(lua);
            //luabind::RawClass<MyGUI::WidgetStyle>(lua);
            luabind::RawClass<MyGUI::IntPoint>(lua);
            luabind::RawClass<MyGUI::FloatPoint>(lua);
            luabind::RawClass<MyGUI::IntSize>(lua);
            luabind::RawClass<MyGUI::IntCoord>(lua);
            luabind::RawClass<MyGUI::IntRect>(lua);
            luabind::RawClass<MyGUI::Colour>(lua);
            luabind::RawClass<MyGUI::IBDrawItemInfo>(lua);
            
            luabind::ExternClass<MyGUI::ICroppedRectangle>(lua);
            luabind::ExternClass<MyGUI::Widget>(lua);
            luabind::ExternClass<MyGUI::LayoutManager>(lua);
            luabind::ExternClass<MyGUI::LayerManager>(lua);
            luabind::ExternClass<MyGUI::ResourceManager>(lua);
            luabind::ExternClass<MyGUI::FactoryManager>(lua);
            luabind::ExternClass<MyGUI::Gui>(lua);

            luabind::ExternClass<MyGUI::TextBox>(lua);
            luabind::ExternClass<MyGUI::Button>(lua);
            luabind::ExternClass<MyGUI::ImageBox>(lua);
            luabind::ExternClass<MyGUI::Window>(lua);
            luabind::ExternClass<MyGUI::DDContainer>(lua);
            luabind::ExternClass<MyGUI::ScrollView>(lua);
            luabind::ExternClass<MyGUI::ItemBox>(lua);
            luabind::ExternClass<MyGUI::ProgressBar>(lua);
            luabind::ExternClass<MyGUI::ScrollBar>(lua);
            
            luabind::ExternClass<MyGUI::EditBox>(lua);
            
            luabind::Class<Sandbox::mygui::WidgetRender>(lua);
            
//            luabind::ExternClass<MyGUI::IFont>(lua);
//            //luabind::ExternClass<Sandbox::mygui::ResourceTrueTypeFont>(lua);
//            //luabind::ExternClass<Sandbox::mygui::ResourceTrueTypeFontOutline>(lua);
//            luabind::ExternClass<Sandbox::mygui::ResourceMultipassFont>(lua);
//            luabind::ExternClass<Sandbox::mygui::ResourceCombineFont>(lua);
//            
            luabind::ExternClass<Sandbox::mygui::RenderManager>(lua);
            luabind::ExternClass<MyGUI::InputManager>(lua);
            luabind::ExternClass<MyGUI::LanguageManager>(lua);
            
            luabind::ExternClass<MyGUI::ControllerManager>(lua);
            
            luabind::ExternClass<MyGUI::ControllerItem>(lua);
            luabind::ExternClass<MyGUI::ControllerPosition>(lua);
            luabind::ExternClass<MyGUI::ControllerFadeAlpha>(lua);
            luabind::ExternClass<MyGUI::ControllerEdgeHide>(lua);
            luabind::ExternClass<MyGUI::ControllerRepeatClick>(lua);
            
            
            luabind::ExternClass<MyGUI::ILayer>(lua);
            luabind::ExternClass<MyGUI::OverlappedLayer>(lua);
            luabind::ExternClass<AnimatedLayer>(lua);
            
            luabind::ExternClass<GUI>(lua);
            
            register_widgets(lua);
            
            
        }
                
        
        void register_factory() {
            MyGUI::FactoryManager::getInstance().registerFactory<AnimatedLayer>(MyGUI::LayerManager::getInstance().getCategoryName());
            register_skin();
            register_widgets();
        }
        
        void unregister_factory() {
            
            unregister_skin();
            unregister_widgets();
            
            MyGUI::FactoryManager::getInstance().unregisterFactory<AnimatedLayer>(MyGUI::LayerManager::getInstance().getCategoryName());
        }

    }
}

