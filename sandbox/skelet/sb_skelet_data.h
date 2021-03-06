#ifndef SB_SKELET_DATA_H_INCLUDED
#define SB_SKELET_DATA_H_INCLUDED

#include "sb_transform2d.h"
#include "sb_graphics.h"
#include "sb_event.h"

namespace GHL {
    struct Data;
}

namespace Sandbox {

    class Image;
    typedef sb::intrusive_ptr<Image> ImagePtr;
    
    struct SkeletonNodeFrame {
        Transform2d transform;
        Color   color;
        size_t  image;
        size_t  node;
    };
    
    class Resources;
    
    class SkeletonAnimation : public meta::object {
        SB_META_OBJECT
    private:
        sb::string          m_name;
        SkeletonNodeFrame*  m_data;
        float               m_fps;
        size_t              m_nodes;
        size_t              m_frames;
        sb::map<size_t,sb::vector<EventPtr> > m_frame_events;
    public:
        explicit SkeletonAnimation( const sb::string& name );
        ~SkeletonAnimation();
        SkeletonNodeFrame* AllocData(size_t nodes,size_t frames);
        const sb::string& GetName() const { return m_name; }
        float GetFPS() const { return m_fps; }
        void SetFPS(float fps) { m_fps = fps; }
        size_t GetFrames() const { return m_frames; }
        size_t GetNodesCount() const { return m_nodes; }
        const SkeletonNodeFrame& GetNodeFrame(size_t frame,size_t node) const;
        void Dump() const;
        void AddEvent(size_t frame,const EventPtr& e);
        const sb::vector<EventPtr>& GetFrameEvents(size_t frame) const;
    };
    typedef sb::intrusive_ptr<SkeletonAnimation> SkeletonAnimationPtr;

    struct SkeletonNodeData {
        sb::string name;
        BlendMode blend;
        DrawAttributesPtr attributes;
    };
    
    class FileProvider;
    class TextureProvider;
    
    class SkeletonData;
    typedef sb::intrusive_ptr<SkeletonData> SkeletonDataPtr;
    class SkeletonData : public meta::object {
        SB_META_OBJECT
    public:
    private:
        sb::map<sb::string,SkeletonAnimationPtr>    m_animations;
        sb::vector<ImagePtr>    m_images;
        sb::map<sb::string,size_t> m_images_map;
        
        sb::vector<SkeletonNodeData> m_nodes;
        sb::map<sb::string,EventPtr> m_events;
        
        const EventPtr& GetEvent(const sb::string& name) const;
    public:
        SkeletonData();
        ~SkeletonData();
        
        void AddNode(const SkeletonNodeData& n);
        size_t GetNodesCount() const { return m_nodes.size(); }
        sb::string GetNodeName(size_t idx);
        void SetNodeAttribute(size_t idx, const Sandbox::DrawAttributesPtr& attribute);
        const sb::vector<SkeletonNodeData>& GetNodes() const { return m_nodes; }
        void AddAnimation( const SkeletonAnimationPtr& animation);
        static SkeletonDataPtr Load(const char* file, Resources* resources);
        static SkeletonDataPtr LoadI(const char* file, FileProvider* files,TextureProvider* textures);
        
        const SkeletonAnimationPtr& GetAnimation(const sb::string& name) const;
        const SkeletonNodeData& GetNode(size_t idx) const;
        const ImagePtr& GetImage(size_t index) const;
        void SetImage(size_t index,const ImagePtr& img);
        bool HasAnimation( const sb::string& name ) const;
        void DumpTextures() const;
        void DumpNodes() const;
        void DumpAnimation(const sb::string& name) const;
    };
    
}

#endif /*SB_SKELET_DATA_H_INCLUDED*/
