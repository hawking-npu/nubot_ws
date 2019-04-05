// Generated by gencpp from file nubot_control/DebugInfo.msg
// DO NOT EDIT!


#ifndef NUBOT_CONTROL_MESSAGE_DEBUGINFO_H
#define NUBOT_CONTROL_MESSAGE_DEBUGINFO_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace nubot_control
{
template <class ContainerAllocator>
struct DebugInfo_
{
  typedef DebugInfo_<ContainerAllocator> Type;

  DebugInfo_()
    : d1(0.0)
    , d2(0.0)  {
    }
  DebugInfo_(const ContainerAllocator& _alloc)
    : d1(0.0)
    , d2(0.0)  {
  (void)_alloc;
    }



   typedef float _d1_type;
  _d1_type d1;

   typedef float _d2_type;
  _d2_type d2;





  typedef boost::shared_ptr< ::nubot_control::DebugInfo_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nubot_control::DebugInfo_<ContainerAllocator> const> ConstPtr;

}; // struct DebugInfo_

typedef ::nubot_control::DebugInfo_<std::allocator<void> > DebugInfo;

typedef boost::shared_ptr< ::nubot_control::DebugInfo > DebugInfoPtr;
typedef boost::shared_ptr< ::nubot_control::DebugInfo const> DebugInfoConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::nubot_control::DebugInfo_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::nubot_control::DebugInfo_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace nubot_control

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'nubot_control': ['/home/jensen/nubot_ws/src/nubot/nubot_control/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::nubot_control::DebugInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::nubot_control::DebugInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_control::DebugInfo_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nubot_control::DebugInfo_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_control::DebugInfo_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nubot_control::DebugInfo_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::nubot_control::DebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "1c78ad82c247783d0c3aa95ce147b845";
  }

  static const char* value(const ::nubot_control::DebugInfo_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x1c78ad82c247783dULL;
  static const uint64_t static_value2 = 0x0c3aa95ce147b845ULL;
};

template<class ContainerAllocator>
struct DataType< ::nubot_control::DebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "nubot_control/DebugInfo";
  }

  static const char* value(const ::nubot_control::DebugInfo_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::nubot_control::DebugInfo_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32 d1\n\
float32 d2\n\
";
  }

  static const char* value(const ::nubot_control::DebugInfo_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::nubot_control::DebugInfo_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.d1);
      stream.next(m.d2);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct DebugInfo_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nubot_control::DebugInfo_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::nubot_control::DebugInfo_<ContainerAllocator>& v)
  {
    s << indent << "d1: ";
    Printer<float>::stream(s, indent + "  ", v.d1);
    s << indent << "d2: ";
    Printer<float>::stream(s, indent + "  ", v.d2);
  }
};

} // namespace message_operations
} // namespace ros

#endif // NUBOT_CONTROL_MESSAGE_DEBUGINFO_H
