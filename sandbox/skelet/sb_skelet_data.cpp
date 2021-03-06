#include "sb_skelet_data.h"
#include "sb_resources.h"
#include <ghl_data_stream.h>
#include <ghl_data.h>
#include <pugixml.hpp>
#include "sb_log.h"
#include "utils/sb_base64.h"
#include "sb_data.h"
#include "sb_image.h"
#include "sb_scene_object.h"

SB_META_DECLARE_OBJECT(Sandbox::SkeletonAnimation,Sandbox::meta::object)
SB_META_DECLARE_OBJECT(Sandbox::SkeletonData,Sandbox::meta::object)

namespace Sandbox {
    
    SkeletonAnimation::SkeletonAnimation(const sb::string& name) : m_name(name), m_data(0),m_fps(15.0),m_nodes(0),m_frames(0) {}
    
    SkeletonAnimation::~SkeletonAnimation() {
        delete [] m_data;
    }
    
    SkeletonNodeFrame* SkeletonAnimation::AllocData(size_t nodes,size_t frames) {
        delete [] m_data;
        m_nodes = nodes;
        m_frames = frames;
        m_data = new SkeletonNodeFrame[m_frames*m_nodes];
        return m_data;
    }
    
    void SkeletonAnimation::AddEvent(size_t frame, const EventPtr &e) {
        sb::map<size_t,sb::vector<EventPtr> >::iterator it = m_frame_events.find(frame);
        if (it!=m_frame_events.end()) {
            it->second.push_back(e);
        } else {
            sb::vector<EventPtr> v;
            v.push_back(e);
            m_frame_events[frame] = v;
        }
    }
    static const sb::vector<EventPtr> empty_frame_events;
    const sb::vector<EventPtr>& SkeletonAnimation::GetFrameEvents(size_t frame) const {
        sb::map<size_t,sb::vector<EventPtr> >::const_iterator it = m_frame_events.find(frame);
        if (it!=m_frame_events.end())
            return it->second;
        return empty_frame_events;
    }
    
    const SkeletonNodeFrame& SkeletonAnimation::GetNodeFrame(size_t frame,size_t node) const {
        sb_assert(frame<m_frames);
        sb_assert(node<m_nodes);
        return m_data[frame*m_nodes+node];
    }
    
    void SkeletonAnimation::Dump() const {
        for (size_t frame = 0; frame < m_frames; ++frame ) {
            LogDebug() << "frame " << frame;
            for (size_t node = 0;node < m_nodes; ++node) {
                const SkeletonNodeFrame& n = GetNodeFrame(frame, node);
                LogDebug() << "n " << n.image << " " << n.node << " " << n.color.ToStringRGB() << " " << n.color.a;
            }
        }
    }
    
    static const EventPtr empty_event;
    
    const EventPtr& SkeletonData::GetEvent(const sb::string &name) const {
        sb::map<sb::string, EventPtr>::const_iterator it = m_events.find(name);
        if (it!=m_events.end())
            return it->second;
        return empty_event;
    }
    
    void SkeletonData::AddAnimation(const SkeletonAnimationPtr& animation) {
        sb_assert(animation);
        sb_assert(animation->GetNodesCount()==m_nodes.size());
        if (m_animations.find(animation->GetName())!=m_animations.end()) {
            LogDebug() << "dublicate animation " << animation->GetName();
        }
        m_animations[animation->GetName()] = animation;
    }
    
    union data_read_u {
        float       fvalue;
        GHL::UInt32 uvalue;
        GHL::Byte   data[4];
    };
    
    SkeletonDataPtr SkeletonData::Load(const char *file, Resources* resources) {
        if (!resources)
            return SkeletonDataPtr();
        return LoadI(file,resources,resources);
    }
        
    SkeletonDataPtr SkeletonData::LoadI(const char* filename, FileProvider* files,TextureProvider* textures_provider) {
        GHL::DataStream* ds = files->OpenFile(filename);
        if (!ds) {
            LogError() << "not found skeleton file " << filename;
            return SkeletonDataPtr();
        }
        GHL::Data* data = GHL_ReadAllData(ds);
        ds->Release();
        if (!data)
            return SkeletonDataPtr();
        
        sb::string dir;
        const char* endslash = ::strrchr(filename, '/');
        if (endslash) {
            dir.assign(filename,endslash+1);
        }
        
        pugi::xml_document doc;
        pugi::xml_parse_result parse_res = doc.load_buffer(data->GetData(),
                                                           data->GetSize(),
                                                           pugi::parse_default, pugi::encoding_utf8);
        data->Release();
        if (!parse_res) {
            LogError() << "failed parse " << filename;
            LogError() << parse_res.description();
            return SkeletonDataPtr();
        }
        pugi::xml_node n = doc.document_element();
        pugi::xml_attribute version = n.attribute("version");
      
        if (!version || version.as_int()<3) {
            LogError() << "deprecated version " << filename;
            return SkeletonDataPtr();
        }
        pugi::xml_node textures = n.child("textures");
        pugi::xml_node nodes = n.child("nodes");
        if (!nodes)
            return SkeletonDataPtr();
        pugi::xml_node animations = n.child("animations");
        if (!animations)
            return SkeletonDataPtr();
        size_t nodes_count = nodes.attribute("count").as_uint();
        SkeletonDataPtr res(new SkeletonData());
        for (pugi::xml_node_iterator nit = nodes.begin(); nit!=nodes.end(); ++nit) {
            SkeletonNodeData node;
            node.name = nit->attribute("name").value();
            node.blend = BLEND_MODE_ALPHABLEND;
            if (::strcmp(nit->attribute("blend").value(),"add")==0) {
                node.blend = BLEND_MODE_ADDITIVE;
            } else if (::strcmp(nit->attribute("blend").value(),"mul")==0) {
                node.blend = BLEND_MODE_MULTIPLY;
            } else if (::strcmp(nit->attribute("blend").value(),"screen")==0) {
                node.blend = BLEND_MODE_SCREEN;
            }
            
            res->AddNode(node);
        }
        if (res->GetNodesCount()!=nodes_count) {
            LogError() << "invalid nodes count";
            return SkeletonDataPtr();
        }
        if (textures) {
            for (pugi::xml_node_iterator it = textures.begin(); it!=textures.end(); ++it) {
                const char* file = it->attribute("file").value();
                TexturePtr texture = textures_provider->GetTexture((dir+file).c_str());
                if (!texture) {
                    LogError() << "not found texture " << file;
                    continue;
                }
                texture->SetFiltered(true);
                for (pugi::xml_node_iterator iit = it->begin();iit!=it->end();++iit) {
                    ImagePtr img = ImagePtr(new Image(texture,iit->attribute("x").as_float(),
                                             iit->attribute("y").as_float(),
                                             iit->attribute("w").as_float(),
                                             iit->attribute("h").as_float()));
                    bool r = iit->attribute("r").as_bool();
                    img->SetRotated(r);
                    if (r) {
                        img->SetSize(iit->attribute("h").as_float(),iit->attribute("w").as_float());
                    }
                    size_t idx = iit->attribute("index").as_uint();
                    res->m_images_map[iit->attribute("name").value()] = idx;
                    res->SetImage(idx,img);
                    img->SetHotspot(Sandbox::Vector2f(iit->attribute("hsx").as_float(),iit->attribute("hsy").as_float()));
                }
            }
        }
        
        pugi::xml_node events_n = n.child("events");
        if (events_n) {
            for (pugi::xml_node_iterator it = events_n.begin(); it!=events_n.end(); ++it) {
                EventPtr e(new Event());
                for (pugi::xml_node_iterator jt = it->begin(); jt!=it->end(); ++jt) {
                    if (::strcmp(jt->name(),"string")==0) {
                        e->SetString(jt->attribute("name").value(), jt->attribute("value").value());
                    } else if (::strcmp(jt->name(), "int")==0) {
                        e->SetInt(jt->attribute("name").value(), jt->attribute("value").as_int());
                    }
                }
                res->m_events[it->attribute("name").value()] = e;
            }
        }
       
        
        for (pugi::xml_node_iterator it = animations.begin();it!=animations.end();++it) {
            size_t frames = it->attribute("frames").as_uint();
            if (frames == 0)
                continue;
            SkeletonAnimationPtr anim(new SkeletonAnimation(it->attribute("name").value()));
            anim->SetFPS(it->attribute("fps").as_float());
            
            SkeletonNodeFrame* pdata = anim->AllocData(nodes_count, frames);
            size_t data_size = (sizeof(float)*(4+2)+sizeof(float)+sizeof(GHL::UInt32))*frames*nodes_count;
            GHL::Data* d = 0;
            if (::strcmp(it->attribute("encoding").as_string(),"base64")==0) {
                d = Base64DecodeData(it->child_value());
            } else if (::strcmp(it->attribute("encoding").as_string(),"file")==0) {
                GHL::DataStream* ds = files->OpenFile(it->child_value());
                if (!ds) {
                    LogError() << "not found file " << it->child_value();
                    continue;
                }
                d = GHL_ReadAllData(ds);
                ds->Release();
            } else {
                LogError() << "unsupported encoding " << it->attribute("encoding").as_string();
                continue;
            }
            
            if (!d) {
                continue;
            }
            if (::strcmp(it->attribute("compression").as_string(),"zlib")==0) {
                GHL::UInt32 size = GHL::UInt32(data_size);
                VectorData<GHL::Byte>* dd = new VectorData<GHL::Byte>();
                dd->vector().reserve(size);
                dd->vector().resize(size);
                
                if( GHL_UnpackZlib(d, dd->vector().data(), &size ) && size == dd->GetSize() ) {
                    d->Release();
                    d = dd;
                } else {
                    dd->Release();
                    d->Release();
                    continue;
                }
            } else if (it->attribute("compression")) {
                LogError() << "unsupported compression " << it->attribute("compression").as_string();
                d->Release();
                continue;
            }
            const float* src = reinterpret_cast<const float*>(d->GetData());
            for (size_t i=0;i<frames;++i) {
                for (size_t n=0;n<nodes_count;++n) {
                    pdata->color = Sandbox::Color(*reinterpret_cast<const GHL::UInt32*>(src));
                    ++src;
                    pdata->transform.m.matrix[0] = *src++;
                    pdata->transform.m.matrix[1] = *src++;
                    pdata->transform.m.matrix[2] = *src++;
                    pdata->transform.m.matrix[3] = *src++;
                    pdata->transform.v.x = *src++;
                    pdata->transform.v.y = *src++;
                    pdata->image = *reinterpret_cast<const GHL::UInt16*>(reinterpret_cast<const GHL::Byte*>(src)+0);
                    pdata->node = *reinterpret_cast<const GHL::UInt16*>(reinterpret_cast<const GHL::Byte*>(src)+2);
                    src++;
                    ++pdata;
                }
            }
            pugi::xml_node events = it->child("events");
            if (events) {
                 for (pugi::xml_node_iterator jt = events.begin(); jt!=events.end(); ++jt) {
                     const EventPtr& e = res->GetEvent(jt->attribute("event").value());
                     if (e) {
                         anim->AddEvent(jt->attribute("frame").as_uint(), e);
                     }
                 }
            }
            res->AddAnimation(anim);
            d->Release();
        }
        return res;
    }
    
    static const SkeletonAnimationPtr empty;
    const SkeletonAnimationPtr& SkeletonData::GetAnimation(const sb::string& name) const {
        sb::map<sb::string,SkeletonAnimationPtr>::const_iterator it = m_animations.find(name);
        if (it == m_animations.end())
            return empty;
        return it->second;
    }
    
    static const SkeletonNodeData empty_node = {"nil",BLEND_MODE_ALPHABLEND,DrawAttributesPtr()};
    const SkeletonNodeData& SkeletonData::GetNode(size_t idx) const {
        if (idx < m_nodes.size())
            return m_nodes[idx];
        return empty_node;
    }
    
    SkeletonData::SkeletonData() {
        
    }
    
    SkeletonData::~SkeletonData() {
        
    }
    
    void SkeletonData::AddNode(const SkeletonNodeData& n) {
        m_nodes.push_back(n);
    }
    
    sb::string SkeletonData::GetNodeName(size_t idx) {
        return GetNode(idx).name;
    }
    void SkeletonData::SetNodeAttribute(size_t idx, const Sandbox::DrawAttributesPtr& attribute) {
        if (idx < m_nodes.size()) {
            m_nodes[idx].attributes = attribute;
        }
    }
    
    void SkeletonData::DumpTextures() const {
        size_t idx = 0;
        for (sb::vector<ImagePtr>::const_iterator it = m_images.begin();it!=m_images.end();++it) {
            LogDebug() << "tex " << idx << " " << ((*it) ? ( (*it)->GetTexture()->GetName().c_str() ) : ("null"));
            ++idx;
        }
    }
    
    void SkeletonData::DumpNodes() const {
        size_t idx = 0;
        for (sb::vector<SkeletonNodeData>::const_iterator it = m_nodes.begin();it!=m_nodes.end();++it) {
            LogDebug() << "node " << idx << " " << it->name;
            ++idx;
        }
    }
    
    void SkeletonData::DumpAnimation(const sb::string &name) const {
        SkeletonAnimationPtr animation = GetAnimation(name);
        if (animation) {
            animation->Dump();
        } else {
            LogDebug() << "(empty)";
        }
    }
    
    bool SkeletonData::HasAnimation( const sb::string& name ) const {
        return m_animations.find(name) != m_animations.end();
    }
    static const ImagePtr empty_image;
    const ImagePtr& SkeletonData::GetImage(size_t index) const {
        if (index >= m_images.size()) {
            return empty_image;
        }
        return m_images[index];
    }
    
    void SkeletonData::SetImage(size_t index,const ImagePtr& img) {
        if (index >= m_images.size()) {
            m_images.resize(index+1);
        }
        m_images[index] = img;
    }
    
}
